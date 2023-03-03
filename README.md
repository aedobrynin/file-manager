# File manager

Written in C with ncurses library. Shows file sizes and file type (dir/symlink/regular file).

## Build and Run
Depends on cmake, make and ncurses. Tested with GCC 9.4.0 on Ubuntu 20.04.
```
mkdir build
cd build
cmake ..
make
./file-manager
```

## Usage
* Use arrow keys to go up/down
* Use D to delete file or directory
* Use H to toggle hidden files
* Use backspace to close
* Use C to select file for copy
* Use X to select file for cut
* Use V in the directory where you want to put the copy of selected file (the original file deletes if it was a cut).
