import cv2
import os
import argparse

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--data_dir', type=str, default='news.avi', help='Directory for storing input data')
    FLAGS, unparsed = parser.parse_known_args()

        frame_number
        keyframes=
    for i in os.listdir("./data/source/keyframe/"+FLAGS+"/"):
        i=cv2.imread(i,0)
