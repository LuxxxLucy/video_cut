# UI design

## major components

> and how to display them

## buttons and their coresspond usage

对于VideoCutOut系统，其输入的内容有video sequence，key frames， Local color model 输出内容是 video object 因此我们需要系统功能可以通过菜单导入video，并且主界面是一个可以显示整个video，并且有控制条可以拖动， 使得画面到达视频制定的时间，然后主界面有一个按钮用来确定key frames，点击确定后，弹出一个新界面，显示这个key frames， 然后新界面侧面有各种工具条，可以对于keyframe的 foreground/background进行划分，然后还有一个按钮点击确定。然后主界面有一 button用来进行转换视频

=======
UI设计中主要有以下按钮：

菜单栏
“导入原视频”--用户通过文件资源管理器导入视频，记录文件路径
“保存关键帧”--将用户处理过的keyframe保存在一个文件夹中，文件名为视频名_xxx(.bmp) xxx表示第几帧，扩展名初定为.bmp
“运行3D Graph cut”--运行3D Graph Cut算法
“运行local refinement”--运行2D Graph Cut 算法，（待定）
“导入背景视频” -- 用户通过文件资源管理器导入视频，记录文件路径
“导入video object” -- 用户通过文件资源管理器导入video object，记录文件路径
"视频融合" -- 用户将video object放置在合适的位置后，融合视频

工具栏：
“勾选轮廓”--用户在keyframe上勾画前景和背景的分界
“区域框选”-- 选取local refinement的window
"橡皮擦"之类的图片处理工具

note:
1.按照论文，每10帧选取一个关键帧。
2.准备使用Qt4进行设计，之前Qt编程较少，目前找到Qt4做图像处理应用的教程，准备学着做
3.暂时使用opencv的videoCapture读取.avi格式的视频，之后可以考虑使用ffmpeg和opencv读取更多格式的视频。
=======

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
