#pragma once

#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

namespace type {
    class ImageFile
    {
    public:
        void load(string path);
        bool write(string filename);
        Mat get();
        void set(Mat image);
        bool empty();
    private:
        Mat image;
    };
}