import random
import os

# docker run --volume $(pwd):$(pwd) -w $(pwd) amd_vega  make -f Makefile

def generateMakefile(number, directory):
    build_rules = "\n".join(
        [
            f"\t$(CC) $(HIPOPTS) trial_{i}.cpp -o trial_{i} $(CFLAGS) $(LDFLAGS)"
            for i in range(number)
        ]
    )
    
    makefile_content = f"""
HIPOPTS = --amdgpu-target=gfx900,gfx906 --save-temps

HIP_PATH ?= /opt/rocm/hip

CC := $(HIP_PATH)/bin/hipcc

release:
{build_rules}

clean:
\trm -f *.o *~ $(EXE)

run:
\t./$(EXE)

profile:
\trocprof ./$(EXE)

events:
\trocprof --events elapsed_cycles_sm ./$(EXE)
"""

    with open(f"{directory}/Makefile", "w") as makefile:
        makefile.write(makefile_content)

def generate(pattern, patternAddr, fileName):
    # the first 10 instructions
    head20 = '\"s_waitcnt vmcnt(0) & lgkmcnt(0)\\n\\t\" \n\t\t\"buffer_wbinvl1\\n\\t\" \n\t\t\"flat_load_dwordx2 %[out0], %[in1] glc\\n\\t\"'
    tail = '"s_waitcnt vmcnt(0) & lgkmcnt(0)\\n\\t\" \n\t\t\"s_nop 0\\n\\t\"' 
    instructions20 = "\n\n\t\t".join(
        [
            f'\"s_waitcnt vmcnt(0) & lgkmcnt(0)\\n\\t\" \n\t\t"flat_load_dwordx2 %[out{i}], %[in{i + 1}] glc\\n\\t\"'
            for i in range(2, 20, 2)
        ]
    )
    final20 = head20 + "\n\n\t\t" + instructions20 + "\n\n\t\t" + tail

    outReg20 = ", ".join(
        [f'[out{i}]"=v"(a)' for i in range(0, 20, 2)]
    )

    inReg20 = ", ".join(
        [f'[in{i * 2 + 1}]"v"((uint64_t *)&arr[{patternAddr[i]}])' for i in range(0, 10)]
    )

    # /////////////////////// #
    # the second 10 instructions
    head40 = '\"s_waitcnt vmcnt(0) & lgkmcnt(0)\\n\\t\" \n\t\t\"buffer_wbinvl1\\n\\t\" \n\t\t\"flat_load_dwordx2 %[out20], %[in21] glc\\n\\t\"'
    instructions40 = "\n\n\t\t".join(
        [
            f'\"s_waitcnt vmcnt(0) & lgkmcnt(0)\\n\\t\" \n\t\t\"flat_load_dwordx2 %[out{i}], %[in{i + 1}] glc\\n\\t\"'
            for i in range(22, 40, 2)
        ]
    )
    final40 = head40 + "\n\n\t\t" + instructions40 + "\n\n\t\t" + tail
    
    outReg40 = ", ".join(
        [f'[out{i}]"=v"(b)' for i in range(20, 40, 2)]
    )

    inReg40 = ", ".join(
        [f'[in{i * 2 + 1}]"v"((uint64_t *)&arr[{patternAddr[i]}])' for i in range(10, 20)]
    )

    # /////////////////////// #
    # the third 10 instructions
    head60 = '\"s_waitcnt vmcnt(0) & lgkmcnt(0)\\n\\t\" \n\t\t\"buffer_wbinvl1\\n\\t\" \n\t\t\"flat_load_dwordx2 %[out40], %[in41] glc\\n\\t\"'
    instructions60 = "\n\n\t\t".join(
        [
            f'\"s_waitcnt vmcnt(0) & lgkmcnt(0)\\n\\t\" \n\t\t\"flat_load_dwordx2 %[out{i}], %[in{i + 1}] glc\\n\\t\"'
            for i in range(42, 60, 2)
        ]
    )
    final60 = head60 + "\n\n\t\t" + instructions60 + "\n\n\t\t" + tail
    
    outReg60 = ", ".join(
        [f'[out{i}]"=v"(c)' for i in range(40, 60, 2)]
    )

    inReg60 = ", ".join(
        [f'[in{i * 2 + 1}]"v"((uint64_t *)&arr[{patternAddr[i]}])' for i in range(20, 30)]
    )

    # contents written into the cpp file
    cpp = f"""
#include "hip/hip_runtime.h"
#define HCC_ENABLE_PRINTF
#include <stdio.h>
#include <stdlib.h>

#define CACHE_ENTRIES 4194304

// kernel code
// Access pattern: {', '.join(pattern)}
// Access count: {len(pattern)}

__global__ void kernel(int * arr) {{
    uint64_t a = 0, b = 0, c = 0;
    
    asm volatile(
        {final20}
        : {outReg20}
        : {inReg20}
        : "memory");

    asm volatile(
        {final40}
        : {outReg40}
        : {inReg40}
        : "memory");

    asm volatile(
        {final60}
        : {outReg60}
        : {inReg60}
        : "memory");
}}

int main(){{
    int *arr = (int *)calloc(sizeof(int), CACHE_ENTRIES);
    int *arr_g;

    hipMallocManaged(&arr_g, CACHE_ENTRIES*sizeof(int));
    hipMemcpy(arr_g, arr, CACHE_ENTRIES*sizeof(int), hipMemcpyHostToDevice);

    hipLaunchKernelGGL(kernel, dim3(1), dim3(1), 0, 0, arr_g);

    hipFree(arr_g);
    free(arr);
    return 0;
}}
    """

    # file generation
    with open(fileName, "w") as file:
        file.write(cpp)

directory = "./target"
os.makedirs(directory, exist_ok=True)
print(f"Directory {directory} is automatically generated.")

with open("./accessPatterns.txt", 'r') as file:
    id = 0
    for line in file:
        '''
        if 'After the manual verification' in line:
            break
        '''
        if not line.strip():
            continue

        if '#' not in line:
            # accessess list stores the all cache accesses
            accesses = [item.strip() for item in line.strip().split(',')]

            # create the access addresses based on the access patterns
            dic = {}                                # dictionary tracking the starting address of each cache line
            offset = [0] * 60                                       
            for i in range(60):
                dic[f"c_{i}"] = i * 65536           # associate each cache line label with the corresponding address

            accessed = []                           # cache lines that are in the cache set
            patternAddr = []                        # access address
            
            for acc in accesses:
                if acc not in accessed:
                    patternAddr.append(dic[acc])
                    accessed.append(acc)
                else:
                    index = int(acc.split('_')[1])
                    offset[index] += 1
                    patternAddr.append(dic[acc] + offset[index])
                    
            generate(accesses, patternAddr, f'{directory}/trial_{id}.cpp')    # generate the testbench
            id += 1
    generateMakefile(id, directory)                                          # generate the Makefile