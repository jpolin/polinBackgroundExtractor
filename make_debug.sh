#/bin/bash

# Build debug

rm -rf debug
mkdir debug
cd debug
cmake .. -DCMAKE_BUILD_TYPE=DEBUG
make
