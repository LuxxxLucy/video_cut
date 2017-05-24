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
- **目前有两种选择**（by 张伟锋）
- 一种是只支持.avi格式，因为用opencv的videoCapture只能读取和处理.avi的视频，其他格式得依赖ffmpeg的命令行进行转换，转换之后是能播放的，但是能不能被opencv读出来还不知道，如果觉得时间不够，可以先做.avi的格式，把工作重点放在算法的实现上。
- 第二种使用ffmpeg对视频进行解码，解码出视频流之后，将YUV420p的帧转换为BGR编码，再把它转化成opencv能处理的mat类型，这里有一个参考链接http://www.jianshu.com/p/6ef3c18d61b0   里面提供了详细的代码，不过还没跑，不知道能不能运行。这种方式不必过多考虑视频的格式，因为ffmpeg能解码绝大多数的视频，应该是比较通用的方法，跑通代码所需的时间不确定。
