import os
from pathlib import Path

def lowercase_extensions_recursive(directory):
    """
    Recursively changes file extensions to lowercase within a given directory.

    Args:
        directory (str): The path to the starting directory.
    """
    for root, _, files in os.walk(directory):
        for filename in files:
            file_path = Path(root) / filename
            if file_path.is_file():  # Ensure it's a file, not a broken link or directory
                # Split the filename into name and extension
                name, ext = os.path.splitext(filename)

                # Check if the extension is already lowercase
                if ext and ext != ext.lower():
                    new_ext = ext.lower()
                    new_filename = name + new_ext
                    new_file_path = Path(root) / new_filename

                    try:
                        os.rename(file_path, new_file_path)
                        print(f"Renamed: {file_path.name} -> {new_file_path.name}")
                    except OSError as e:
                        print(f"Error renaming {file_path.name}: {e}")

if __name__ == "__main__":
    target_directory = input("Enter the directory path to process: ")

    if os.path.isdir(target_directory):
        lowercase_extensions_recursive(target_directory)
        print("Extension lowercasing complete.")
    else:
        print(f"Error: '{target_directory}' is not a valid directory.")
