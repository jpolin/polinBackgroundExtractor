#Background Extractor for Video Files#

**Author:** Joe Polin

##Description##

###Overview###
This repository contains a library for extracting a static background image from a stationary video file (.mp4). It also offers 3 ways to leverage this library:

- Dynamic linking from C++ executable to library
- Local server that offers a REST API interface to upload video and download image
- Web page that makes ajax calls which conform to REST interface

###Background subtraction###
The background extraction is handled by the OpenCV function ```BackgroundSubtractorMOG2(...)```. The implementation is based on the work of Zoran Zivkovic. The main features of the algorithm are:
- Uses a self-varying number of mixed gaussian models to determine the value for each background pixel
- Has the option for detecting shadows and returning them in the foreground mask (not relevant for background extraction)

##Getting started##

###System Dependencies###

This was developed and successfully run on:
- Ubuntu 14.04 (3.13.0-48-generic)

Be sure the following packages are installed:
- CMake 2.8 or newer
- OpenCV 2.4
- [cpprestsdk](https://github.com/Microsoft/cpprestsdk)
  - Included as git submodule (must run ```git submodule init && git submodule update``` after cloning)
  - Requires **libboost-all-dev** and **libssl-dev**
  - Since this is a large library, it is compiled independently from the rest of the code base:
  
    ```bash
    cd server/cpprestsdk/Release/
    mkdir build
    cd build
    cmake ..
    ```
    
  - Refer to [this link](https://github.com/Microsoft/cpprestsdk/wiki) for more information on building cpprestsdk

##Building##

Source is written in C++ and can be compiled using CMake and Make. There are shell scripts in the top level directory (**make_debug.sh** and **make_rel.sh**) to compile the respective builds. 

```bash
cd polinBackgroundExtractor # root directory of repo
sh make_rel.sh # or make_debug.sh
cd release # or debug
```



The builds are intentionally placed outside of the source tree, and the executables can be acccessed in their respective build directory (**debug** or **release**). 



##Usage##

There are 3 ways to leverage the library:

1. Local (wrapper for library):

```bash
cd polinBackgroundExtractor/
sh make_rel.sh # If not already compiled
cd release/wrapper/
./bgExtractorWrapper ../../sample_videos/ring_sample_1.mp4 output.jpg
```
