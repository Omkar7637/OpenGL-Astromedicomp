#include<stdio.h>

int main(void)
{
  //function declarations
  void PrintCUDADeviceProperties(void);

  //code
  PrintCUDADeviceProperties();
}

void PrintCUDADeviceProperties(void)
{
  //code
  printf("CUDA INFORMATION: \n");
  printf("===================================================================================================\n");

  cudaError_t ret_cuda_rt;
  int dev_count;
  ret_cuda_rt = cudaGetDeviceCount(&dev_count);
  if(ret_cuda_rt != cudaSuccess)
  {
    printf("CUDA Runtime API Error - cudaGetDeviceCount() failed due to %s \n", cudaGetErrorString(ret_cuda_rt));
  } 
  else if(dev_count == 0)
  {
    printf("There is NO CUDA supported device on this system. \n");
    return;
  }else
  {
   printf("Total number of CUDA supporting GPU Device/ Device on this system : %d\n", dev_count);
   for(int i = 0; i< dev_count; i++)
   {
     cudaDeviceProp dev_prop;
     int driverVersion = 0, runtimeVersion = 0;

     ret_cuda_rt = cudaGetDeviceProperties(&dev_prop, i);
     if(ret_cuda_rt != cudaSuccess)
     {
       printf("%s in %s at line %d\n", cudaGetErrorString(ret_cuda_rt), __FILE__, __LINE__);
       return;
     }
     printf("\n");
     cudaDriverGetVersion(&driverVersion);
     cudaRuntimeGetVersion(&runtimeVersion);

     printf("******************** CUDA DRIVER AND RUNTIME INFORMATION *******************\n");
     printf("=============================================================================\n");
     printf("CUDA Driver Version                    : %d.%d\n", driverVersion / 1000, (driverVersion % 100) /10);
     printf("CUDA Runtime Version                    : %d.%d\n", runtimeVersion / 1000, (runtimeVersion % 100) /10);
     printf("=============================================================================\n");
     printf("********************* GPU DEVICE GENERAL INFORMATION ************************ \n");
     printf("==============================================================================\n");
     printf("GPU DEVICE NUMBER                                                  :%d\n", i);
     printf("GPU DEVICE NUMBER                                                  :%s\n", dev_prop.name);
     printf("GPU Device Compute Capability                                      :%d.%d\n", dev_prop.major, dev_prop.minor);
     printf("GPU Device Clock Rate                                              :%d\n", dev_prop.clockRate);
     printf("GPU Device Type                                                    : ");
     if(dev_prop.integrated)
          printf("Integrated (On-Board)\n");
     else
          printf("Discrete (Card)\n");

    printf("\n");
    printf("***********************GPU DEVICE MEMORY INFORMATION***************************\n");
    printf("===============================================================================\n");
    printf("GPU Device Total Memory                                                    : %.0f GB = %.0f MB = %llu Bytes\n", ((float)dev_prop.totalGlobalMem / 1048576.0f) / 1024.0f, (float)dev_prop.totalGlobalMem / 1048576.0f, (unsigned long long)dev_prop.totalGlobalMem);
    printf("GPU Device Constant Memory                                                 : %lu Bytes\n", (unsigned long)dev_prop.totalConstMem);
    printf("GPU Dvice Shared Memory Per SMProcessor                                    : %lu\n", (unsigned long)dev_prop.sharedMemPerBlock);
    printf("\n");
    printf("******************* GPU DEVICE DRIVER INFORMATION ******************************\n");
    printf("================================================================================\n");
    printf("GPU Device has ECC support                                                 : %s\n", dev_prop.ECCEnabled ? "Enabled" : "Disabled");
    #if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)   
    printf("GPU Device CUDA Driver Mode (TCC or WDDM)                                  : %s\n", dev_prop.tccDriver ? "TCC (Tesla Compute Cluster Driver)" : "WDDM (Windows Display Driver Model)");
    #endif
    printf("***************************************************************************************************************************************************************************************************************************************************************\n");
   
   }
  }
}