// Compatibility with CUDA to allow some functions to be called both from the
// host and the device, if nvcc is used to compile.
#pragma once
#ifndef SINI_CUDA_COMPAT_H
#define SINI_CUDA_COMPAT_H

#ifdef __CUDACC__
#define SINI_CUDA_COMPAT inline __host__ __device__
#define SINI_CUDA_KERNEL __global__
#else
#define SINI_CUDA_COMPAT inline
#endif

// Allows for device/host specific CUDA code
#if defined(__CUDACC__) && defined(__CUDA_ARCH__)
#define SINI_CUDA_DEVICE
#endif

#endif // SINI_CUDA_COMPAT_H
