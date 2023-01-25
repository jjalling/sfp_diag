cmake_minimum_required(VERSION 3.0)
if (POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif ()

project(sfp_diag
VERSION 0.1
LANGUAGES CXX)

add_subdirectory(src)
