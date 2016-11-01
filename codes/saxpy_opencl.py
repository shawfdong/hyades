#!/usr/bin/env python
# -*- coding: utf-8 -*-
 
import pyopencl as cl
import numpy as np
 
# initialize vectors
N = 20480
a = np.random.rand(1).astype(np.float32)
x = np.random.rand(N).astype(np.float32)
y = np.random.rand(N).astype(np.float32)
 
ctx = cl.create_some_context()
queue = cl.CommandQueue(ctx)
 
mf = cl.mem_flags
dx = cl.Buffer(ctx, mf.READ_ONLY | mf.COPY_HOST_PTR, hostbuf=x)
dy = cl.Buffer(ctx, mf.READ_WRITE | mf.COPY_HOST_PTR, hostbuf=y)
 
# define the kernel
prg = cl.Program(ctx, """
__kernel void saxpy(float a, __global float *x, __global float *y)
{
    int i = get_global_id(0);
    y[i] += a*x[i];
}
""").build()
 
# launch the kernel function
prg.saxpy(queue, (N,), (64,), a[0], dx, dy)
 
y_gpu = np.empty_like(y)
cl.enqueue_copy(queue, y_gpu, dy)
 
# verify the results
y_host = a * x + y
m = np.amax( np.fabs( (y_gpu-y_host)/y_host ) )
print m
if m < 1e-6:
    print 'Success!'
else:
    print 'Failure!'
