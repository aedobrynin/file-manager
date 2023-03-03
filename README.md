# File manager

Written in C with ncurses library. Shows file sizes and file type (dir/symlink/regular file).

## Build
Depends on cmake, make and ncurses. Tested with GCC 9.4.0 on Ubuntu 20.04.
```
mkdir build
cd build
cmake ..
make
```

## Usage
* Use arrow keys to go up/down
* Use D to delete file or directory
* Use H to toggle hidden files
* Use backspace to close
