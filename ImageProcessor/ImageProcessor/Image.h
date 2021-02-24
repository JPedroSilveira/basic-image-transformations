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
        Image();
        void load(string path);
        bool write(string filename);
        Mat get();
        void set(Mat image);
        bool isEmpty();
        bool isGrayScale();
        void horizontalFlip();
        void verticalFlip();
        void applyGrayScaleFilter();
        void applyQuantizeFilter(int colors);
        int calculateHistogram(int histogram[256]);
        void applyBrightness(int value);
        void applyContrast(int value);
        void applyNegativeFilter();
        void applyHistogramEqualizationFilter();
        void applyHistogramMatchingFilter(Image* target);
        void normalizeHistogram(int target, int histogram[256], int maxHistogramValue);
        void calculateCumulativeHistogram(int* cumulativeHistogram, int* histogram);
        void applyEasterEggOne(int row, int column);
        void applyZoomOut(int x, int y);
        void applyZoomIn();
    private:
        Mat data;
        bool grayScale;
        void flip(Vec3b (*func)(Mat3b src, Mat3b dst, int row, int col));
        int filterNewChannelValue(int value);
        int getGrayValueFromRGB(int r, int g, int b);
    };
}