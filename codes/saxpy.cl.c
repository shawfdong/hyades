#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#define N 2048
 
int main(void)
{
  float *x, *y;
  size_t size = N*sizeof(float);
  x = (float *)malloc(size);
  y = (float *)malloc(size);
 
  // initialize x and y
  srand(time(NULL));
  float a = (float)random() / RAND_MAX;
  int i;
  for (i=0; i<N; i++)
    x[i] = (float)random() / RAND_MAX;
  for (i=0; i<N; i++)
    y[i] = (float)random() / RAND_MAX;
 
  // Get the platform ID
  cl_platform_id platform;
  clGetPlatformIDs(1, &platform, NULL);
 
  // Get the first GPU device associated with the platform
  cl_device_id device;
  clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
 
  // Create an OpenCL context for the GPU device
  cl_context context;
  context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
 
  // Create Command Queue
  cl_command_queue cmd_queue;
  cmd_queue = clCreateCommandQueue(context, device, 0, NULL);
 
  // Create Memory Buffers on device
  cl_mem dx = clCreateBuffer(context, CL_MEM_READ_ONLY, size, NULL, NULL);
  cl_mem dy = clCreateBuffer(context, CL_MEM_READ_WRITE, size, NULL, NULL);
 
  // Write to buffer objects from host memory
  clEnqueueWriteBuffer(cmd_queue, dx, CL_FALSE, 0, size, x, 0, NULL, NULL);
  clEnqueueWriteBuffer(cmd_queue, dy, CL_FALSE, 0, size, y, 0, NULL, NULL);
 
  // Build program object and set up kernel arguments
    const char* source =
      "__kernel void saxpy(float a, __global float *x, __global float *y)\n"
      "{\n"
      "  int i = get_global_id(0);\n"
      "  y[i] += a*x[i];\n"
      "}\n";
 
    cl_program program = clCreateProgramWithSource(context, 1, &source, NULL, NULL);
    clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    cl_kernel kernel = clCreateKernel(program, "saxpy", NULL);
    clSetKernelArg(kernel, 0, sizeof(float), (void *)&a);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&dx);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&dy);
 
    // Set number of work-items in a work-group
    size_t localWorkSize = 64;
    int numWorkGroups = (N + localWorkSize - 1) / localWorkSize;
    size_t globalWorkSize = numWorkGroups * localWorkSize;
    clEnqueueNDRangeKernel(cmd_queue, kernel, 1, NULL, &globalWorkSize, &localWorkSize, 0, NULL, NULL);
 
    // Read from buffer object to host memory
    float *d;
    d = (float *)malloc(size);
    clEnqueueReadBuffer(cmd_queue, dy, CL_TRUE, 0, size, d, 0, NULL, NULL);
 
    // Verify the results
    float m = -1.;
    float tmp;
    for (i=0; i<N; i++) {
      y[i] += a * x[i];
      tmp = fabsf( (d[i]-y[i])/y[i] );
      if ( tmp > m ) m = tmp;
    }
 
    // Clean up
    clReleaseMemObject(dx);
    clReleaseMemObject(dy);
    clReleaseCommandQueue(cmd_queue);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseContext(context);
 
    free(x);
    free(y);
    free(d);
 
    if ( m < 1E-12 ) {
      printf("Success!\n");
      return 0;
    }
    else {
      printf("Failure!\n");
      return 1;
    }
}
