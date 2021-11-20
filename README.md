# Access Control List

## Introduction
Access list control is a scalable, offline software solution developed in a library-like, headers-only maner which is capable to hold and manage various types of resources.
It is implemented by following the unique ownership principle, which means consumed by access. So if there is an attepmt to get/pop a resource then it will be invalidated.

### High level design components
The project consists of the following components:
- acl, is the main module which is responsible for managing the subjects and their resources
- subject, a module which defines subject abstraction 
- resource, a module which defines resource abstraction
- access_level, a module which encapsulates the access level informtion of the specific resource 
- exception  

## Tech stack and dependencies

Text analyzer uses the following stack:

- C++
- bash script
- CMake
- boost unit test module
- stl

## Environment
As a build system CMake has been used and was written so it could be compiled and executed on linux and windows environments.

## Build
### Plannd 
Providing Dockerfile/Docker image for testing in an isolated environment, currently there is an issue ralted with os image version that's not fixed in this initial version!
In order to build the project manually, please follow to bellow described steps:
```
$ cd [project directory]
$ ./build.sh -t [debug/release] -c [clean build]
Mandatory arguments:
-t, use it to define the type of the build debug/release

Optional arguments:
-c, specifies whether the project should be built on top of cleaned cmake environment
```
After having this done the executable file could be found in the ```./bin``` area of current project root directory.

## Tests
Boost unit test framework has been used for tests development. Currently there are 11 tests which is by far less than a full coverage. A lot more tests are required to cover existing functionality.

In order to run the unit test:
```
$ ./bin/access_list_unit_tests
```
