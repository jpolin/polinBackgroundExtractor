#Background Extractor for Video Files#

**Author:** Joe Polin

##Description##

###Overview###

  This repository contains a library for extracting a static background image from a stationary video file (.mp4). It also offers 3 ways to leverage this library:
  
  - Dynamic linking from C++ executable to library
  - Local server that offers a REST API interface to upload video and download image
  - Web page that makes ajax calls which conform to REST interface
  
###Background subtraction###

  The background extraction is handled by the OpenCV function ```BackgroundSubtractorMOG2(...)```. The implementation is based on the [work of Zoran Zivkovic](http://www.zoranz.net/PUBLICATIONS.html). The main features of the algorithm are:
  - Uses a self-varying number of mixed gaussian models to determine the value for each background pixel
  - Has the option for detecting shadows and returning them in the foreground mask (not relevant for background extraction)
  
  
  The algorithm currently relies on the following assumptions:
  
  - The camera is not moving relative to the background
  - The algorithm, will not necessarily compensate for intensity changes, gradual or otherwise

##Setting Up and Running Examples##
  
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

###Building###

Source is written in C++ and can be compiled using CMake and Make. There are shell scripts in the top level directory (**make_debug.sh** and **make_rel.sh**) to compile the respective builds. 

```bash
cd polinBackgroundExtractor # root directory of repo
sh make_rel.sh # or make_debug.sh
cd release # or debug
```

The builds are intentionally placed outside of the source tree, and the executables can be acccessed in their respective build directory (**debug** or **release**). 

##Usage##

Once the codebase has been built, there are 3 ways to leverage the library. Each method has a corresponding shell script in the root directory.

1. Link executable to library:

  In your C++ application:
  
  ```C++
  #include "bgExtractor.hpp"
  .
  .
  .
  bgExtractor extractor;
  extractor.loadVideoFile(inputFile);
  extractor.analyzeFile(visualize_output_flag);
  extractor.writeBackgroundToFile(outputFile);
  ```
  
  For a complete example, see [extractor_main.cpp](/extractor/extractor_main.cpp). To run this example, run:
  
  ```bash
  cd polinBackgroundExtractor
  sh run_extractor_executable.sh
  ```

  Linking your exectuable to this library will provide better performance than the following 2 methods; however, we will see that using the REST interface will offer other advantages.
  
2. Use REST API on a local server:

  First, the server must be started by calling the executable generated from [server_main.cpp](server/server_main.cpp) which accepts, as command-line arguments, the port and path to the data folder. It can be started more conevniently using the shell script:
  
  ```bash
  cd polinBackgroundExtractor
  sh run_server.sh # Arguments set in file
  ```
  
  Since the server uses a [restful interface](https://en.wikipedia.org/wiki/Representational_state_transfer), the developer is not tied to a specific language. For instance, a background extraction can be done using command line arguments that talk to the server:
  
  ```bash
  # run_REST_client_demo.sh
  #TODO: Copy CURL commands into here
  ```
  
  These commands are included in [run_RESdT_client_demo.sh](/run_REST_client_demo.sh).
  
  A further advantage of using the RESTful server is that it is completely asynchronous and can handle multiple calls at the same time (TODO--make sure this is true!).
  
3. Use web interface:

  This could more accurately be described as an extension of the previous point since it also leverages the RESTful server interface. To use this, first start the server as described in the previous point. Then, simply open [www/video_extractor.html](/www/video_extractor.html) in your chosen web browser.
  
##Design##
  
  
