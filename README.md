# Shovel-sweeper
Before proceeding you will need to install cmake on your machine
## Building:
Build it with cmake as usual:
``` 
mkdir build && cd build && cmake .. && cmake --build .
```

In the "build" folder you will find an executable, run it with these options:

## Usage:
`shovel-sweeper.exe -h <height> -w <width> -m <mines>`

### Options:
- `-h <height>`  
  Set the height of the grid.

- `-w <width>`  
  Set the width of the grid.

- `-m <mines>`  
  Set the number of mines.

## Dependencies
- [SFML 2.6.1](https://www.sfml-dev.org/) (licensed under zlib/png license)
