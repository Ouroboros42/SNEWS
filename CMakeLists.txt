cmake_minimum_required(VERSION 3.27)
project(SNEWS LANGUAGES CUDA CXX)

set(CMAKE_CUDA_STANDARD 17)


set_target_properties(cuda PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON)

SET(CMAKE_CUDA_COMPILER C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v12.3/bin/nvcc.exe)
