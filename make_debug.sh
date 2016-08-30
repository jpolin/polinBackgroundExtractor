#/bin/bash

# Build debug

rm -rf debug
mkdir debug
cd debug
cmake .. -DCMAKE_RELEASE_TYPE=DEBUG
make
