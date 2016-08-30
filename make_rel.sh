#/bin/bash

# Build debug

rm -rf release
mkdir release
cd release
cmake .. -DCMAKE_RELEASE_TYPE=Release
make
