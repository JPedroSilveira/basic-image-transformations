#include "Image.h"

const string DEFAULT_SAVE_PATH = "C:/Users/55519/Documents/Git/processador-de-imagens/";

Vec3b horizontalFlipFilter(Mat3b src, Mat3b dst, int row, int col)
{
    return src(row, src.cols - 1 - col);
}

Vec3b verticalFlipFilter(Mat3b src, Mat3b dst, int row, int col)
{
    return src(src.rows - 1 - row, col);
}

namespace type 
{
    void Image::load(string path) 
    {
        if (!path.empty()) 
        {
            this->image = imread(path, IMREAD_COLOR);
        }
    }

    bool Image::write(string filename) {
        if (!this->empty()) 
        {
            return imwrite(filename, this->image);
        }
        return false;
    }

    Mat Image::get() 
    {
        return this->image;
    }

    void Image::set(Mat image) 
    {
        this->image = image;
    }

    bool Image::empty() 
    {
        return this->image.empty();
    }

    void Image::horizontalFlip() 
    {
        flip(horizontalFlipFilter);
    }

    void Image::verticalFlip() 
    {
        flip(verticalFlipFilter);
    }

    void Image::flip(Vec3b(*func)(Mat3b src, Mat3b dst, int row, int col)) 
    {
        const Mat3b src = image;
        Mat3b dst(src.rows, src.cols);
        for (int row = 0; row < dst.rows; ++row) 
        {
            for (int col = 0; col < dst.cols; ++col) 
            {
                const Vec3b srcUnit = func(src, dst, row, col);
                memcpy(&dst(row, col), &srcUnit, sizeof(srcUnit));
            }
        }
        this->set(dst);
    }

    void Image::toGrayScale() 
    {
        const Mat src = image;
        Mat dst(src.rows, src.cols, CV_8UC1);
        for (int row = 0; row < dst.rows; ++row) 
        {
            for (int col = 0; col < dst.cols; ++col) 
            {
                const Vec3b srcColors = src.at<Vec3b>(row, col);
                const int blue = srcColors(0);
                const int green = srcColors(1);
                const int red = srcColors(2);

                dst.at<uchar>(row, col) = (uchar) (0.299 * red + 0.587 * green  + 0.114 * blue);
            }
        }
        this->set(dst);
    }
}