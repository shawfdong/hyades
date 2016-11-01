#!/usr/bin/env python
# -*- coding: utf-8 -*-
 
from numbapro import cuda
import numpy as np
 
# define the kernel
@cuda.jit('void(float64[:], float64, float64[:], float64[:])')
def daxpy(yd, a, x, y):
    i = cuda.grid(1)   # equals to threadIdx.x + blockIdx.x * blockDim.x
    yd[i] = a * x[i] + y[i]
 
# initialize vectors
N = 20480
a = np.random.rand(1).astype(np.float64)
x = np.random.rand(N).astype(np.float64)
y = np.random.rand(N).astype(np.float64)
yd = np.zeros_like(x)
 
# launch the kernel function
daxpy[N/64, 64](yd, a[0], x, y)
 
# verify the results
yh = a * x + y
if m < 1e-12:
    print 'Success!'
else:
    print 'Failure!'
