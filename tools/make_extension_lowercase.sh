#!/bin/bash

# Loop through all files in the current directory
for file in *; do
    # Check if it's a regular file (not a directory)
    if [[ -f "$file" ]]; then
        # Extract the filename without extension
        filename="${file%.*}"
        # Extract the extension
        extension="${file##*.}"

        # Convert the extension to lowercase using parameter expansion
        lowercase_extension="${extension,,}"

        # Check if the extension was actually uppercase and needs changing
        if [[ "$extension" != "$lowercase_extension" ]]; then
            new_file="${filename}.${lowercase_extension}"
            # Rename the file
            # mv "$file" "$new_file"
            echo "Renamed: $file -> $new_file"
        fi
    fi
done
