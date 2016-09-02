#!/bin/bash          

# How to use restful API

# Options
port = 6000
data_folder = "data_bash_test"
input_file = ring_sample_2
input_file_dir = sample_files

# Start server
./release/server/bgExtractionServer $port $data_folder


# Delete/create empty data folder
rm -rf $data_folder
mkdir $data_folder

# Ask for an ID
curl -i -H "Accept: application/json" -H "Content-Type: application/json" -X GET http://localhost:$port/new_id

# Write file
curl http://localhhost:6000/$ID/$input_file --upload-file $input_file_dir/$input_file

# Ask for background image

# Ask again (will be much faster--already buffered)
