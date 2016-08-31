#Background Extractor for Video Files#

**Author:** Joe Polin

##Description##

This library extracts a static background image from an input video. 

##Dependencies##

This was developed and successfully run on:
- Ubuntu 14.04 (3.13.0-48-generic)

Be sure the following packages are installed:
- CMake 2.8 or newer
- OpenCV (TODO-version)
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
    
  - Refer to [this link](https://github.com/Microsoft/cpprestsdk/wiki) for more information on building

##Building##

Source is written in C++ and can be compiled using CMake and Make. There are shell scripts in the top level directory (**make_debug.sh** and **make_rel.sh**) to compile the respective builds. The builds are intentionally placed outside of the source tree, and the executables can be acccessed in their respective build directory (**debug** or **release**). See below for usage.

##Usage##

There are 3 ways to leverage the library:

1. Local (wrapper for library):

```bash
cd polinBackgroundExtractor/
sh make_rel.sh # If not already compiled
cd release/wrapper/
./bgExtractorWrapper ../../sample_videos/ring_sample_1.mp4 output.jpg
```
