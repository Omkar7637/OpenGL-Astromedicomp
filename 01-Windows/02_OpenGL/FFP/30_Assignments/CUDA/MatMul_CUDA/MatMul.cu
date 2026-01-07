// header files
// standard headers

#include<stdio.h>

// cuda headers

#include<cuda.h>
#include"helper_timer.h"

// macros

#define BLOCK_WIDTH 64

// global variables 

int *hostA = NULL;
int *hostB = NULL;
int *hostC = NULL;
int *gold = NULL;

int *deviceA = NULL;
int *deviceB = NULL;
int *deviceC = NULL;

float timeOnCPU = 0.0f;
float timeOnGPU = 0.0f;

__global__ void matMulGPU(int* A , int* B , int* C , int numARows , int numAColumns , int numBColumns , int numCColumns)
{
    // code
    
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int column = blockIdx.x * blockDim.x + threadIdx.x;

    if((row < numARows) && (column < numBColumns))
    {
        int value = 0.0f;
        for(int k = 0; k < numAColumns ; k++)
        {
            int a = A[row * numAColumns + k];
            int b = B[k * numBColumns + column];
            value += a*b;
        }
        C[row * numCColumns + column] = value;

    }
}

int main(int argc , char *argv[])
{
    // function declaration

    void InitA(int *data , int  , int);
    void InitB(int *data , int  , int);
    void matMulCPU(int* , int* , int* , int , int , int , int);
    void cleanup();

    // variable declaration

    int numARows = BLOCK_WIDTH;
    int numAColumns = BLOCK_WIDTH;
    int numBRows = BLOCK_WIDTH;
    int numBColumns = BLOCK_WIDTH;

    int numCRows = numARows;
    int numCColumns = numBColumns;

    int numGoldRows = numARows;
    int numGoldColumns = numBColumns;

    int sizeA = numARows * numAColumns * sizeof(int);
    int sizeB = numBRows * numBColumns * sizeof(int);
    int sizeC = numCRows * numCColumns * sizeof(int);
    int sizeGold = numGoldRows * numGoldColumns * sizeof(int);

    cudaError_t result = cudaSuccess;

    // code 
    // host memory allocation

    hostA = (int*)malloc(sizeA);
    if(hostA == NULL)
    {
        printf("Host Memory allocation is failed for hostA array. \n");
        cleanup();
        exit(EXIT_FAILURE); 
    }

    hostB = (int*)malloc(sizeB);
    if(hostB == NULL)
    {
        printf("Host Memory allocation is failed for hostB array. \n");
        cleanup();
        exit(EXIT_FAILURE); 
    }

    hostC = (int*)malloc(sizeC);
    if(hostC == NULL)
    {
        printf("Host Memory allocation is failed for hostC array. \n");
        cleanup();
        exit(EXIT_FAILURE); 
    }

    gold = (int*)malloc(sizeGold);
    if(gold == NULL)
    {
        printf("Host Memory allocation is failed for gold array. \n");
        cleanup();
        exit(EXIT_FAILURE); 
    }

    // printing matrix dimensions and sizes
    printf("The Dimensions Of Matrix 'hostA' Are : %d x %d :\n", numARows , numAColumns);
    printf("The Dimensions Of Matrix 'hostB' Are : %d x %d :\n", numBRows , numBColumns);
    printf("The Dimensions Of Matrix 'hostC' Are : %d x %d :\n", numCRows , numCColumns);
    printf("The Dimensions Of Matrix 'gold' Are : %d x %d :\n", numGoldRows , numGoldColumns);

    printf("Size Of Matrix hostA = %d \n", sizeA);
    printf("Size Of Matrix hostB = %d \n", sizeB);
    printf("Size Of Matrix hostC = %d \n", sizeC);
    printf("Size Of Matrix gold = %d \n", sizeGold);

    // fill source matrices

    InitA(hostA , numARows , numAColumns);
    InitB(hostB , numBRows , numBColumns);

    // device memory allocation

    result = cudaMalloc((void**)&deviceA , sizeA);
    if(result != cudaSuccess)
    {
        printf("Device Memory allocation is failed for deviceA array. \n");
        cleanup();
        exit(EXIT_FAILURE); 
    }

    result = cudaMalloc((void**)&deviceB , sizeB);
    if(result != cudaSuccess)
    {
        printf("Device Memory allocation is failed for deviceB array. \n");
        cleanup();
        exit(EXIT_FAILURE); 
    }

    result = cudaMalloc((void**)&deviceC , sizeC);
    if(result != cudaSuccess)
    {
        printf("Device Memory allocation is failed for deviceC array. \n");
        cleanup();
        exit(EXIT_FAILURE); 
    }

    // copy data from host matrices into device matrices

    result = cudaMemcpy(deviceA , hostA, sizeA , cudaMemcpyHostToDevice);
    if(result != cudaSuccess)
    {
        printf("Device to host data copy is faild for deviceA matrix \n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    result = cudaMemcpy(deviceB , hostB, sizeB , cudaMemcpyHostToDevice);
    if(result != cudaSuccess)
    {
        printf("Device to host data copy is faild for deviceB matrix \n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    // cuda kernel configration

    dim3 dimGrid = dim3(ceil((int)numBColumns/(int)BLOCK_WIDTH) ,ceil((int)numARows/(int)BLOCK_WIDTH) ,1);
    dim3 dimBlock = dim3(BLOCK_WIDTH , BLOCK_WIDTH , 1);

    // CUDA Kernel for Matrix MultiPlication

    StopWatchInterface* timer = NULL;
    sdkCreateTimer(&timer);
    sdkStartTimer(&timer);

	matMulGPU <<<dimGrid, dimBlock >>>(deviceA,deviceB,deviceC,numARows,numAColumns,numBColumns,numCColumns);

    sdkStopTimer(&timer);
    timeOnGPU = sdkGetTimerValue(&timer);
    sdkDeleteTimer(&timer);
    timer = NULL;

    // copy data from device matrix into host matrix

    result = cudaMemcpy(hostC , deviceC , sizeC , cudaMemcpyDeviceToHost);
    if(result != cudaSuccess)
    {
        printf("Device To Host Data Copy is failed for hostC matrix \n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    // matrix multiplication on host

    matMulCPU(hostA , hostB , hostC , numARows , numAColumns , numBColumns , numCColumns);

    // comparison

    int breakValue = 1;
    bool bAccuracy = true;
    for(int i = 0; i<numCRows * numCColumns; i++)
    {
        int val1 = gold[i];
        int val2 = hostC[i];
        if(val1 != val2)
        {
            bAccuracy = false;
            breakValue = i;
            break;
        }
    }

    char str[128];
    if(bAccuracy == false)
    {
        sprintf(str , "Comparision of CPU and GPU Matrix Multiplication is not accurate at array index %d", breakValue);
    }
    else
    {
        sprintf(str , "Comparision of CPU and GPU Matrix Multiplication is accurate");
    }

    printf("Time Taken for Matrix Multiplication on CPU = %.6f \n" , timeOnCPU);
    printf("Time Taken for Matrix Multiplication on GPU = %.6f \n" , timeOnGPU);
    printf("%s\n" , str);   

    // cleanup 

    cleanup();

    return 0;

}

void InitA(int *data , int row , int col)
{
    int num = 1;
    //code

    for(int i = 0; i<row;i++)
    {
        for(int j = 0; j<col;j++)
        {
            *(data + i * col + j) = num;
            num++;
        }
    }
}

void InitB(int *data , int row , int col)
{
    int num = BLOCK_WIDTH;
    //code

    for(int i = 0; i<row;i++)
    {
        for(int j = 0; j<col;j++)
        {
            *(data + i * col + j) = num;
            num--;
        }
    }
}


void matMulCPU(int* A , int* B , int* C , int numARows , int numAColumns , int numBColumns , int numCColumns)
{
    // code
    StopWatchInterface* timer = NULL;
    sdkCreateTimer(&timer);
    sdkStartTimer(&timer);

    for(int i = 0; i<numARows;i++)
    {
        for(int j = 0; j<numBColumns;j++)
        {
            
                int value = 0.0f;
                for(int k = 0; k<numAColumns;k++)
                {
                    int a = A[i * numAColumns + k];
                    int b = B[i * numBColumns + j];
                    value += a*b;
                }
                C[i * numCColumns + j] = value;

            }
        }

        sdkStopTimer(&timer);
        timeOnCPU = sdkGetTimerValue(&timer);
        sdkDeleteTimer(&timer);
        timer = NULL;
}

void cleanup(void)
{
    // code
    if(deviceC)
    {
        cudaFree(deviceC);
        deviceC = NULL;
    }

    if(deviceB)
    {
        cudaFree(deviceB);
        deviceB = NULL;
    }

    if(deviceA)
    {
        cudaFree(deviceA);
        deviceA = NULL;
    }

    if(gold)
    {
        free(gold);
        gold = NULL;
    }

    if(hostC)
    {
        free(hostC);
        hostC = NULL;
    }
    if(hostB)
    {
        free(hostB);
        hostB = NULL;
    }
    if(hostA)
    {
        free(hostA);
        hostA = NULL;
    }



}