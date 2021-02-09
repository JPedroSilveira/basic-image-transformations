#pragma once

#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

namespace type {
    class Image
    {
    public:
        void load(string path);
        bool write(string filename);
        Mat get();
        void set(Mat image);
        bool empty();
        //flip(src, dst, 1)
        void horizontalFlip();
        //flip(src, dst, 0)
        void verticalFlip();
    private:
        Mat image;
        void flip(Vec3b (*f)(Mat3b src, Mat3b dst, int row, int col));
    };
}