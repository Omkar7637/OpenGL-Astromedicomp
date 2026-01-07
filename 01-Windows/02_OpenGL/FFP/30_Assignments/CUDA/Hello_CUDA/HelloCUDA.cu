// header files
// standard headers

#include<stdio.h>

// cuda headers

#include<cuda.h>

// global variables

const int iNumberOfArrayElements = 5;

float* hostInput1 = NULL;
float* hostInput2 = NULL;
float* hostOutput = NULL;

float* deviceInput1 = NULL;
float* deviceInput2 = NULL;
float* deviceOutput = NULL;

// cuda kernel

__global__ void vecAddGPU(float* in1 , float* in2 , float* out , int len)
{
    // code
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if(i<len)
    {
        out[i] = in1[i] + in2[i];
    }

}

// entry point function

int main(void)
{
    // function declaration
    void cleanup(void);

    // variable declaration

    int size = iNumberOfArrayElements * sizeof(float);
    cudaError_t result = cudaSuccess;

    // code
    // host memory allocation
    hostInput1 = (float*)malloc(size);
    if(hostInput1 == NULL)
    {
        printf("Host Memory allocation is failed for hostInput1 array. \n");
        cleanup();
        exit(EXIT_FAILURE); 
    }

    hostInput1 = (float*)malloc(size);
    if(hostInput1 == NULL)
    {
        printf("Host Memory allocation is failed for hostInput1 array. \n");
        cleanup();
        exit(EXIT_FAILURE); 
    }

    hostInput2 = (float*)malloc(size);
    if(hostInput2 == NULL)
    {
        printf("Host Memory allocation is failed for hostInput2 array. \n");
        cleanup();
        exit(EXIT_FAILURE); 
    }

    hostOutput = (float*)malloc(size);
    if(hostOutput == NULL)
    {
        printf("Host Memory allocation is failed for hostOutput array. \n");
        cleanup();
        exit(EXIT_FAILURE); 
    }

    // filling values into host arrays

    hostInput1[0] = 101.0;
    hostInput1[1] = 102.0;
    hostInput1[2] = 103.0;
    hostInput1[3] = 104.0;
    hostInput1[4] = 105.0;

    hostInput2[0] = 201.0;
    hostInput2[1] = 202.0;
    hostInput2[2] = 203.0;
    hostInput2[3] = 204.0;
    hostInput2[4] = 205.0;

    // device memory allocation

    result = cudaMalloc((void**)&deviceInput1 ,size);
    if(result != cudaSuccess)
    {
        printf("Device memory allocation is failed for deviceInput1 array. ");
    }

    result = cudaMalloc((void**)&deviceInput2 ,size);
    if(result != cudaSuccess)
    {
        printf("Device memory allocation is failed for deviceInput2 array. ");
    }

    result = cudaMalloc((void**)&deviceOutput ,size);
    if(result != cudaSuccess)
    {
        printf("Device memory allocation is failed for deviceOutput array. ");
    }

    // copy data from host arrays into device arrays

    result = cudaMemcpy(deviceInput1 , hostInput1 , size , cudaMemcpyHostToDevice);
    if(result != cudaSuccess)
    {
        printf("Host to device data copy is faild for deviceInput1 array \n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    result = cudaMemcpy(deviceInput2 , hostInput2 , size , cudaMemcpyHostToDevice);
    if(result != cudaSuccess)
    {
        printf("Host to device data copy is faild for deviceInput2 array \n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    dim3 dimGrid = dim3(iNumberOfArrayElements , 1, 1);
    dim3 dimBlock = dim3(1,1,1);

    // CUDA kernel for vector Addition
    vecAddGPU <<<dimGrid , dimBlock>>> (deviceInput1 , deviceInput2 , deviceOutput , iNumberOfArrayElements);

    // copy data from array into host array

    result = cudaMemcpy(hostOutput , deviceOutput , size , cudaMemcpyDeviceToHost);
    if(result != cudaSuccess)
    {
        printf("Device To Host Data Copy is failed for hostOutput array \n");
        cleanup();
        exit(EXIT_FAILURE);
    }


    // vector addition on host

    for(int i = 0; i<iNumberOfArrayElements; i++)
    {
        printf("%f + %f = %f\n" , hostInput1[i] , hostInput2[i] , hostOutput[i]);
    }

    cleanup();

    return 0;
}

void cleanup(void)
{
    // code

    if(deviceOutput)
    {
        cudaFree(deviceOutput);
        deviceOutput = NULL;
    }

    if(deviceInput2)
    {
        cudaFree(deviceInput2);
        deviceInput2 = NULL;
    }

    if(deviceInput1)
    {
        cudaFree(deviceInput1);
        deviceInput1 = NULL;
    }

    if(hostOutput)
    {
        cudaFree(hostOutput);
        hostOutput = NULL;
    }

    if(hostInput2)
    {
        cudaFree(hostInput2);
        hostInput2 = NULL;
    }

    if(hostInput1)
    {
        cudaFree(hostInput1);
        hostInput1 = NULL;
    }

}