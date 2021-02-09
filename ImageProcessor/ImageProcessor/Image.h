#pragma once

#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;

namespace type 
{
    class Image
    {
    public:
        void load(string path);
        bool write(string filename);
        Mat get();
        void set(Mat image);
        bool empty();
        void horizontalFlip();
        void verticalFlip();
        void toGrayScale();
        void quantizeGrayScaleImage(int colors);
    private:
        Mat image;
        bool isGrayScale;
        void flip(Vec3b (*f)(Mat3b src, Mat3b dst, int row, int col));
    };
}