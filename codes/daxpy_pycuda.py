#!/usr/bin/env python
# -*- coding: utf-8 -*-
 
import pycuda.driver as cuda
import pycuda.autoinit
from pycuda.compiler import SourceModule
import numpy as np
 
# define the kernel
mod = SourceModule("""
__global__ void daxpy(double *yd, double a, double *x, double *y)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    yd[i] = a * x[i] + y[i];
}
""")
 
daxpy = mod.get_function("daxpy")
 
# initialize vectors
N = 20480
a = np.random.rand(1).astype(np.float64)
x = np.random.rand(N).astype(np.float64)
y = np.random.rand(N).astype(np.float64)
yd = np.zeros_like(x)
 
# launch the kernel function
daxpy(cuda.Out(yd), a[0], cuda.In(x), cuda.In(y),
      block=(N/64,1,1), grid=(64,1,1))
 
# verify the results
yh = a * x + y
m = np.amax( np.fabs( (yd-yh)/yh ) )
if m < 1e-12:
    print 'Success!'
else:
    print 'Failure!'
