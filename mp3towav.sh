#!/bin/bash
file="$1"
output="$2"

if [[ $file == "" || $output == "" ]]; then
    echo ERROR: Two arguments required!
    echo "Usage: "
    echo "mp3towav <input_file.mp3> <output_file.wav>"
else 
    ffmpeg -i "$file" -bufsize 64k -ar 44100 -ac 2 "$output"
fi