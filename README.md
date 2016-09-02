#Background Extractor for Video Files#

**Author:** Joe Polin

##Description##

###Overview###

  This repository contains a library for extracting a static background image from a stationary video file (.mp4). It also offers 2 ways to leverage this library:
  
  - Dynamic linking from C++ executable to library
  - A combination of a (locally hosted) server and web page; the two communicate using REST commands (eg GET, POST)
  
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
- If you're running the same, you can skip to the end of this section where I list all the commands you need to run in one shot

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

On my machine (Ubuntu 14.04), here are all of the commands I need to run:

```bash

# ** clone repository **
cd polinBackgroundExtractor
git submodule init
git submodule update
cd server/cpprestsdk/Release
mkdir build
cd build
cmake ..
make # Takes a while

# Back to root of repository
cd ../../../../
sh make_rel.sh
```

###Dev environment###

I did most of my C++ development in Eclipse. The .project file is included in this directory if you wish to import the project.

##Usage##

Once the codebase has been built, there are 2 ways to leverage the library. 

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
  cd polinBackgroundExtractor # root dir
  sh run_extractor_executable.sh
  ```

  Linking your exectuable to this library will provide better performance than the following 2 methods; however, we will see that using the REST interface will offer other advantages.
  
2. Use a web page and local server

  First, the server must be started by calling the executable generated from [server_main.cpp](server/server_main.cpp) which accepts, as command-line arguments, the port and path to the data folder. It can be started more conevniently using the shell script:
  
  ```bash
  cd polinBackgroundExtractor # root dir
  sh run_server.sh # Arguments set in file
  ```
  
  Since the server uses a [restful interface](https://en.wikipedia.org/wiki/Representational_state_transfer), the developer is not tied to a specific language. Most popular languages support the sending of (at least simple) REST commands, and one of the most popular it HTTP/Javascript (which is what my example uses). Simply open [www/video_extractor.html](/www/video_extractor.html)(your local copy, though) in your chosen web browser.
  
  **NOTE:** Due to a bug that I didn't have time to work through (specifics below), you can only upload the files that are in the **polingBackgroundExtractor/sample_videos** directory. I'm having trouble transferring the files with a multi-part POST request, so I'm cheating by using the filename that gets passed to me and checking there.
  
##Design (by class)##
  
###Background Extraction ([bgExtractor.hpp](/extractor/bgExtractor.hpp))###

I investigated a number of methods for extracting the background, including more- and less-complicated versions of the Mixture of Gaussians model. Ultimately, the OpenCV function (BackgroundSubtractorMOG2) proved adequate, so I focused on packaging it up and delivering it in a more self-contained package. The inputs and outputs are purely file paths which provides 2 benefits:

- Clients calling the methods directly in the library do not need to include or link to OpenCV
- Since all requests coming into the server are already in the form of file paths, this makes for minimal work required by the server

Another feature I added was a frame buffer limit. Since this can be fairly computationally expensive to iterate through every frame, there is a max number of frames that the algorithm will consider. Furthermore, it will spread these "frames of interest" evenly throughout the clip as best it can. Intuitively, this should make sure that a moving object that resides in the video for only the first 30% of the clip won't have an unfair influence.

There are some other enhancements I would make to this class given time:
- Provide a more conveneint way for measuring progress
- Account for gradual changes in light
- Determine when there are forefround objects and automatically have them buffered for the client

###Server ([server.hpp](/server/server.hpp))###

I still believe that designing a RESTful server that can be hosted locally was a good decision given its support in many languages, including javascript. In essence, running the server locally and accessing via webpage is similar to having a front-end for my library. At the same time, it could be deployed as a cloud service with little modification to the architecture (it would certainly need to be extended though; see below).

However, implementing the server in C++ proved more daunting than I anticipated (even with the help of the cpprestsdk library). In particular, writing the callback for a multi-part PUT command proved too complicated for this time frame. To simulate the proper funtionality, I am simply passing file paths between the server and webpage. Obviously, if this server were not being hosted on the same machine as the browser, it would not work. This is currently the largest weakness in the platform.

An important design feature of the server is that the client must first request a unique ID to be associated with their upload/download. Once this ID is returned, the client is approved to upload their video file. This allows the server to make sure that two clients uploading videos with the same name (but different content) don't get crossed. In the data folder, these ID's are used as folder names, and each directory holds the .mp4 and .jpg files. Thus, the background extractor will not be re-run if multiple requests for the same file are made.

Going forward, I would like make the following improvements and extensions to the server:
- Properly implement the POST (or PUT) callback to handle multi-part requests ([this library](https://github.com/webappsdk/granada) looks promising)
- Install more robust error checking (and helpful feedback) on faulty requests
- Generate ID's more intelligently (check for existing folders/ID's at startup)
- Implement DEL request to remove folder (and ID) when client closes session

###Large scale deployent###

In the future, if this were to be deployed as a full cloud service, the architecture would need to be further extended. Although I personally don't have much first-hand experience in scaling a cloud-based application, I did my best to design the framework with this ultimate goal in mind. Given the knowledge I have now, my next steps would look something like:

- Install a container system, such as Docker, where the containers run the bgExtractor code
- The Docker daemon would send paths of uploaded files to the extractors running in the containers
- When the container finished, it would return a path to the background file (or an error code) to be delivered to the client

Disclaimer: I haven't used containers or Docker before

###Sources###

Some resources that I depended on during development:

http://mariusbancila.ro/blog/2013/08/19/full-fledged-client-server-example-with-cpprest-sdk-110/
http://docs.opencv.org/3.1.0/d1/dc5/tutorial_background_subtraction.html
https://github.com/Microsoft/cpprestsdk/blob/master/Release/samples/CasaLens/casalens.cpp
http://stackoverflow.com/questions/14978411/http-post-and-get-using-curl-in-linux




