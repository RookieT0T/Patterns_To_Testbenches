
#include "hip/hip_runtime.h"
#define HCC_ENABLE_PRINTF
#include <stdio.h>
#include <stdlib.h>

#define CACHE_ENTRIES 4194304

// kernel code
// Access pattern: c_0, c_1, c_2, c_3, c_4, c_5, c_6, c_7, c_8, c_9, c_10, c_11, c_12, c_13, c_14, c_15, c_15, c_15, c_15, c_15, c_15, c_15, c_15, c_15, c_15, c_15, c_15, c_15, c_15, c_15
// Access count: 30

__global__ void kernel(int * arr) {
    uint64_t a = 0, b = 0, c = 0;
    
    asm volatile(
        "s_waitcnt vmcnt(0) & lgkmcnt(0)\n\t" 
		"buffer_wbinvl1\n\t" 
		"flat_load_dwordx2 %[out0], %[in1] glc\n\t"

		"s_waitcnt vmcnt(0) & lgkmcnt(0)\n\t" 
		"flat_load_dwordx2 %[out2], %[in3] glc\n\t"

		"s_waitcnt vmcnt(0) & lgkmcnt(0)\n\t" 
		"flat_load_dwordx2 %[out4], %[in5] glc\n\t"

		"s_waitcnt vmcnt(0) & lgkmcnt(0)\n\t" 
		"flat_load_dwordx2 %[out6], %[in7] glc\n\t"

		"s_waitcnt vmcnt(0) & lgkmcnt(0)\n\t" 
		"flat_load_dwordx2 %[out8], %[in9] glc\n\t"

		"s_waitcnt vmcnt(0) & lgkmcnt(0)\n\t" 
		"flat_load_dwordx2 %[out10], %[in11] glc\n\t"

		"s_waitcnt vmcnt(0) & lgkmcnt(0)\n\t" 
		"flat_load_dwordx2 %[out12], %[in13] glc\n\t"

		"s_waitcnt vmcnt(0) & lgkmcnt(0)\n\t" 
		"flat_load_dwordx2 %[out14], %[in15] glc\n\t"

		"s_waitcnt vmcnt(0) & lgkmcnt(0)\n\t" 
		"flat_load_dwordx2 %[out16], %[in17] glc\n\t"

		"s_waitcnt vmcnt(0) & lgkmcnt(0)\n\t" 
		"flat_load_dwordx2 %[out18], %[in19] glc\n\t"

		"s_waitcnt vmcnt(0) & lgkmcnt(0)\n\t" 
		"s_nop 0\n\t"
        : [out0]"=v"(a), [out2]"=v"(a), [out4]"=v"(a), [out6]"=v"(a), [out8]"=v"(a), [out10]"=v"(a), [out12]"=v"(a), [out14]"=v"(a), [out16]"=v"(a), [out18]"=v"(a)
        : [in1]"v"((uint64_t *)&arr[0]), [in3]"v"((uint64_t *)&arr[65536]), [in5]"v"((uint64_t *)&arr[131072]), [in7]"v"((uint64_t *)&arr[196608]), [in9]"v"((uint64_t *)&arr[262144]), [in11]"v"((uint64_t *)&arr[327680]), [in13]"v"((uint64_t *)&arr[393216]), [in15]"v"((uint64_t *)&arr[458752]), [in17]"v"((uint64_t *)&arr[524288]), [in19]"v"((uint64_t *)&arr[589824])
        : "memory");

    asm volatile(
        "s_waitcnt vmcnt(0) & lgkmcnt(0)\n\t" 
		"buffer_wbinvl1\n\t" 
		"flat_load_dwordx2 %[out20], %[in21] glc\n\t"

		"s_waitcnt vmcnt(0) & lgkmcnt(0)\n\t" 
		"flat_load_dwordx2 %[out22], %[in23] glc\n\t"

		"s_waitcnt vmcnt(0) & lgkmcnt(0)\n\t" 
		"flat_load_dwordx2 %[out24], %[in25] glc\n\t"

		"s_waitcnt vmcnt(0) & lgkmcnt(0)\n\t" 
		"flat_load_dwordx2 %[out26], %[in27] glc\n\t"

		"s_waitcnt vmcnt(0) & lgkmcnt(0)\n\t" 
		"flat_load_dwordx2 %[out28], %[in29] glc\n\t"

		"s_waitcnt vmcnt(0) & lgkmcnt(0)\n\t" 
		"flat_load_dwordx2 %[out30], %[in31] glc\n\t"

		"s_waitcnt vmcnt(0) & lgkmcnt(0)\n\t" 
		"flat_load_dwordx2 %[out32], %[in33] glc\n\t"

		"s_waitcnt vmcnt(0) & lgkmcnt(0)\n\t" 
		"flat_load_dwordx2 %[out34], %[in35] glc\n\t"

		"s_waitcnt vmcnt(0) & lgkmcnt(0)\n\t" 
		"flat_load_dwordx2 %[out36], %[in37] glc\n\t"

		"s_waitcnt vmcnt(0) & lgkmcnt(0)\n\t" 
		"flat_load_dwordx2 %[out38], %[in39] glc\n\t"

		"s_waitcnt vmcnt(0) & lgkmcnt(0)\n\t" 
		"s_nop 0\n\t"
        : [out20]"=v"(b), [out22]"=v"(b), [out24]"=v"(b), [out26]"=v"(b), [out28]"=v"(b), [out30]"=v"(b), [out32]"=v"(b), [out34]"=v"(b), [out36]"=v"(b), [out38]"=v"(b)
        : [in21]"v"((uint64_t *)&arr[655360]), [in23]"v"((uint64_t *)&arr[720896]), [in25]"v"((uint64_t *)&arr[786432]), [in27]"v"((uint64_t *)&arr[851968]), [in29]"v"((uint64_t *)&arr[917504]), [in31]"v"((uint64_t *)&arr[983040]), [in33]"v"((uint64_t *)&arr[983041]), [in35]"v"((uint64_t *)&arr[983042]), [in37]"v"((uint64_t *)&arr[983043]), [in39]"v"((uint64_t *)&arr[983044])
        : "memory");

    asm volatile(
        "s_waitcnt vmcnt(0) & lgkmcnt(0)\n\t" 
		"buffer_wbinvl1\n\t" 
		"flat_load_dwordx2 %[out40], %[in41] glc\n\t"

		"s_waitcnt vmcnt(0) & lgkmcnt(0)\n\t" 
		"flat_load_dwordx2 %[out42], %[in43] glc\n\t"

		"s_waitcnt vmcnt(0) & lgkmcnt(0)\n\t" 
		"flat_load_dwordx2 %[out44], %[in45] glc\n\t"

		"s_waitcnt vmcnt(0) & lgkmcnt(0)\n\t" 
		"flat_load_dwordx2 %[out46], %[in47] glc\n\t"

		"s_waitcnt vmcnt(0) & lgkmcnt(0)\n\t" 
		"flat_load_dwordx2 %[out48], %[in49] glc\n\t"

		"s_waitcnt vmcnt(0) & lgkmcnt(0)\n\t" 
		"flat_load_dwordx2 %[out50], %[in51] glc\n\t"

		"s_waitcnt vmcnt(0) & lgkmcnt(0)\n\t" 
		"flat_load_dwordx2 %[out52], %[in53] glc\n\t"

		"s_waitcnt vmcnt(0) & lgkmcnt(0)\n\t" 
		"flat_load_dwordx2 %[out54], %[in55] glc\n\t"

		"s_waitcnt vmcnt(0) & lgkmcnt(0)\n\t" 
		"flat_load_dwordx2 %[out56], %[in57] glc\n\t"

		"s_waitcnt vmcnt(0) & lgkmcnt(0)\n\t" 
		"flat_load_dwordx2 %[out58], %[in59] glc\n\t"

		"s_waitcnt vmcnt(0) & lgkmcnt(0)\n\t" 
		"s_nop 0\n\t"
        : [out40]"=v"(c), [out42]"=v"(c), [out44]"=v"(c), [out46]"=v"(c), [out48]"=v"(c), [out50]"=v"(c), [out52]"=v"(c), [out54]"=v"(c), [out56]"=v"(c), [out58]"=v"(c)
        : [in41]"v"((uint64_t *)&arr[983045]), [in43]"v"((uint64_t *)&arr[983046]), [in45]"v"((uint64_t *)&arr[983047]), [in47]"v"((uint64_t *)&arr[983048]), [in49]"v"((uint64_t *)&arr[983049]), [in51]"v"((uint64_t *)&arr[983050]), [in53]"v"((uint64_t *)&arr[983051]), [in55]"v"((uint64_t *)&arr[983052]), [in57]"v"((uint64_t *)&arr[983053]), [in59]"v"((uint64_t *)&arr[983054])
        : "memory");
}

int main(){
    int *arr = (int *)calloc(sizeof(int), CACHE_ENTRIES);
    int *arr_g;

    hipMallocManaged(&arr_g, CACHE_ENTRIES*sizeof(int));
    hipMemcpy(arr_g, arr, CACHE_ENTRIES*sizeof(int), hipMemcpyHostToDevice);

    hipLaunchKernelGGL(kernel, dim3(1), dim3(1), 0, 0, arr_g);

    hipFree(arr_g);
    free(arr);
    return 0;
}
    