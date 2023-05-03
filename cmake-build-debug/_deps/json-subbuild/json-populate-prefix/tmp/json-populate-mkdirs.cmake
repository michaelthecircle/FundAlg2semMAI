# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/admin/Documents/labs irb/lab 5/zad 1/cmake-build-debug/_deps/json-src"
  "/Users/admin/Documents/labs irb/lab 5/zad 1/cmake-build-debug/_deps/json-build"
  "/Users/admin/Documents/labs irb/lab 5/zad 1/cmake-build-debug/_deps/json-subbuild/json-populate-prefix"
  "/Users/admin/Documents/labs irb/lab 5/zad 1/cmake-build-debug/_deps/json-subbuild/json-populate-prefix/tmp"
  "/Users/admin/Documents/labs irb/lab 5/zad 1/cmake-build-debug/_deps/json-subbuild/json-populate-prefix/src/json-populate-stamp"
  "/Users/admin/Documents/labs irb/lab 5/zad 1/cmake-build-debug/_deps/json-subbuild/json-populate-prefix/src"
  "/Users/admin/Documents/labs irb/lab 5/zad 1/cmake-build-debug/_deps/json-subbuild/json-populate-prefix/src/json-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/admin/Documents/labs irb/lab 5/zad 1/cmake-build-debug/_deps/json-subbuild/json-populate-prefix/src/json-populate-stamp/${subDir}")
endforeach()
