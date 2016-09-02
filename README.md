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

Once the codebase has been built, there are 3 ways to leverage the library. 

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
  cd polinBackgroundExtractor # root fir
  sh run_extractor_executable.sh
  ```

  Linking your exectuable to this library will provide better performance than the following 2 methods; however, we will see that using the REST interface will offer other advantages.
  
2. Use REST API on a local server:

  First, the server must be started by calling the executable generated from [server_main.cpp](server/server_main.cpp) which accepts, as command-line arguments, the port and path to the data folder. It can be started more conevniently using the shell script:
  
  ```bash
  cd polinBackgroundExtractor # root dir
  sh run_server.sh # Arguments set in file
  ```
  
  Since the server uses a [restful interface](https://en.wikipedia.org/wiki/Representational_state_transfer), the developer is not tied to a specific language. For instance, a background extraction can be done using command line arguments that talk to the server:
  
  ```bash
  # run_REST_client_demo.sh
  #TODO: Copy CURL commands into here
  ```
  
  These commands are included in [run_RESdT_client_demo.sh](/run_REST_client_demo.sh).
  
  A further advantage of using the RESTful server is that it is completely asynchronous and can handle multiple calls at the same time (I confirmed that the server would handle a second PUT request even if a first one was still being handled).
  
3. Use web interface:

  This could more accurately be described as an extension of the previous point since it also leverages the RESTful server interface. To use this, first start the server as described in the previous point. Then, simply open [www/video_extractor.html](/www/video_extractor.html)(your local copy) in your chosen web browser.
  
  **NOTE:** Due to a bug that I'm still working through (specifics below), you can only upload the files that are in the **polingBackgroundExtractor/sample_videos** directory. I'm having trouble transferring the files with a PUT request, so currently I'm cheating by using the filename that gets passed to me and checking there.
  
##Design (by class)##
  
###Background Extraction ([bgExtractor](/extractor/bgExtractor.hpp))###

I investigated a number of methods for extracting the background, including more- and less-complicated versions of the Mixture of Gaussians model. Ultimately, the OpenCV function (BackgroundSubtractorMOG2) proved adequate, so I focused on packaging it up and delivering it in a more self-contained package. The inputs and outputs are purely file paths which provides 2 benefits:

- Clients calling the methods directly in the library do not need to include or link to OpenCV
- Since all requests coming into the server are already in the form of file paths, this makes for minimal work required by the server

There are some enhancements I would make to this class given time:
- Provide a more conveneint way for measuring progress
- Account for gradual changes in light
- Determine when there are forefround objects and automatically have them buffered for the client

###Server ([server.hpp](/server/server.hpp))###

I still believe that designing a RESTful server that can be hosted locally was a good decision given its support in many languages, including javascript. In essence, running the server locally and accessing via webpage is similar to having a front-end for my library. At the same time, it could be deployed as a cloud service with little modification.

However, implementing the server in C++ proved more daunting than I anticipated (even with the help of the cpprestsdk library). In particular, writing the callback for a multi-part PUT command proved too complicated for this time frame. To simulate the proper funtionality, I am simply passing file paths between the server and webpage. Obviously, if this server were not being hosted on the same machine as the browser, it would not work. This is currently the largest weakness in the platform.

Further, I ran into a number of other issues, such as Cross Origin Requests (CORs) due to the fact that the server was not being hosted on port 80. Apparently, when browsers (such as Google Chrome) make CORs, they encapsulate the PUT request inside of an OPTION request. Thus, the server currently interprets these as the same (admittedly, this is a short-term hack).

An important design feature of the server is that the client must first request a unique ID to be associated with their upload/download. Once this ID is returned, the client is approved to upload their video file. This allows the server to make sure that two clients uploading videos with the same name (but different content) don't get crossed. In the data folder, these ID's are used as folder names, and each directory holds the .mp4 and .jpg files. Thus, the background extractor will not be re-run if multiple requests for the same file are made.

Going forward, I would like make the following improvements and extensions to the server:
- Properly implement the PUT callback to handle multi-part requests ([this library](https://github.com/webappsdk/granada) looks promising)
- Install more robust error checking (and helpful feedback) on faulty requests
- Generate ID's more intelligently (check for existing folders/ID's at startup)
- Implement DEL request to remove folder (and ID) when client closes session

###Large scale deployent###

In the future, if this were to be deployed as a full cloud service, the architecture would need to be further extended. Although I personally don't have much first-hand experience in scaling a cloud-based application, I did my best to design the framework with this ultimate goal in mind. Given the knowledge I have now, my first attempt would look something like:

- Install a container system, such as Docker, where each container had a version of the bgExtractor executable running
- The Docker daemon would send paths of uploaded files to the extractors running in the containers
- When the container finished, it would return a path to the background file (or an error code) to be delivered to the client

Disclaimer: I haven't used containers or Docker before


