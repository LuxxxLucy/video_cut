# UI design

## major components

> and how to display them

## buttons and their coresspond usage

对于VideoCutOut系统，其输入的内容有video sequence，key frames， Local color model 输出内容是 video object 因此我们需要系统功能可以通过菜单导入video，并且主界面是一个可以显示整个video，并且有控制条可以拖动， 使得画面到达视频制定的时间，然后主界面有一个按钮用来确定key frames，点击确定后，弹出一个新界面，显示这个key frames， 然后新界面侧面有各种工具条，可以对于keyframe的 foreground/background进行划分，然后还有一个按钮点击确定。然后主界面有一 button用来进行转换视频

## 前端界面的与后端的对接。

1. video cut

(默认配置，分辨率相等，所有帧都变换同一个偏移量)

```
  cutVideo( std::string sourceVideoFilename, std::string keyFramesDirectoryName,std::string resultFilename)
```

1. video paste

> **默认配置**:分辨率相等，所有帧都变换同一个偏移量)

```
  composeVideo( std::string cuttedVideoFilename, std::string backVideoFilename, std::pair <int,int> offset,std::string resultFilename)
```
