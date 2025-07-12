#!/bin/bash

# Loop through all files ending with .C (case-sensitive)
for file in *._c_; do
  # Check if the file exists (to handle cases where no .C files are found)
  if [ -f "$file" ]; then
    # Construct the new filename by replacing .C with .c
    new_file="${file%._c_}.c"
    # Rename the file
    mv "$file" "$new_file"
    echo "Renamed '$file' to '$new_file'"
  fi
done
