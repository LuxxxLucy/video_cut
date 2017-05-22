# settings

> Meta setting for this project

## programing language and platform

language: `C++`

libraries: `opencv`, we decide to use opencv mat as our basic data structure

## install and usage

you will need to install following libraries in you machine

- **g++**: (our compiler of c++ language)
- **CMake**: config and make
- **opencv**

run following command to make

```
cd <project directory>
mkdir exe
cmake .
make
```

if every thing goes well, you shall see a executable "exe/ReadImage". You shoud run

```
exe/ReadImage data/lena512color.bmp
```

to see whether you have successfully install the compiling envrioment

## video format

> **other kind of video**: may be we could write some scripts(using ffmpeg or some thing else) to convert other fromats into our standard ones.
