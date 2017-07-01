#include <iostream>
#include "VideoCut.h"

int main(int argc, char const *argv[]) {
    std::string video_path="./data/news.avi";
    // std::string video_path=this->fileFull;
    std::string keyframes_path="./data/source/keyframe/news.avi";
    // std::string keyframes_path=this->centerWidget->getSavePath();
    VideoCut::doVideoCut_real(video_path,keyframes_path);
    return 0;
}
