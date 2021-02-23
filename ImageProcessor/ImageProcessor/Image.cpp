#include "Image.h"
#include <opencv2/imgproc.hpp>

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
    Image::Image()
    {
        this->grayScale = false;
    }

    void Image::load(string path) 
    {
        if (!path.empty()) 
        {
            this->set(imread(path, IMREAD_COLOR));
        }
    }

    bool Image::write(string filename) {
        if (!this->isEmpty()) 
        {
            return imwrite(filename, this->get());
        }
        return false;
    }

    Mat Image::get() 
    {
        return this->data;
    }

    void Image::set(Mat image) 
    {
        this->data = image;
        this->grayScale = false;
    }

    bool Image::isEmpty() 
    {
        return this->get().empty();
    }

    bool Image::isGrayScale()
    {
        return this->grayScale;
    }

    void Image::applyGrayScaleFilter()
    {
        if (this->isGrayScale()) return;

        const Mat src = this->get();
        Mat3b dst(src.rows, src.cols);
        for (int row = 0; row < dst.rows; ++row)
        {
            for (int col = 0; col < dst.cols; ++col)
            {
                const Vec3b srcColors = src.at<Vec3b>(row, col);
                const int blue = srcColors(0);
                const int green = srcColors(1);
                const int red = srcColors(2);

                const int grayValue = this->getGrayValueFromRGB(red, green, blue);

                dst.at<Vec3b>(row, col)[0] = grayValue;
                dst.at<Vec3b>(row, col)[1] = grayValue;
                dst.at<Vec3b>(row, col)[2] = grayValue;
            }
        }
        this->set(dst);
        this->grayScale = true;
    }

    void Image::horizontalFlip() 
    {
        this->flip(horizontalFlipFilter);
    }

    void Image::verticalFlip() 
    {
        this->flip(verticalFlipFilter);
    }

    void Image::flip(Vec3b(*func)(Mat3b src, Mat3b dst, int row, int col)) 
    {
        if (this->isEmpty()) return;

        const Mat3b src = this->get();
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

    void Image::applyQuantizeFilter(int colors)
    {
        if (this->isEmpty()) return;

        this->applyGrayScaleFilter();

        const float bandSize = 256 / colors;
        const Mat src = this->get();
        Mat3b dst(src.rows, src.cols);
        for (int row = 0; row < dst.rows; ++row)
        {
            for (int col = 0; col < dst.cols; ++col)
            {
                const Vec3b srcColors = src.at<Vec3b>(row, col);
                const int grayValue = srcColors(0);
                const int quantizedValue = round(grayValue / bandSize) * bandSize;

                dst.at<Vec3b>(row, col)[0] = quantizedValue;
                dst.at<Vec3b>(row, col)[1] = quantizedValue;
                dst.at<Vec3b>(row, col)[2] = quantizedValue;
            }
        }
        this->set(dst);
    }

    int Image::calculateHistogram(int histogram[256])
    {        
        if (this->isEmpty()) return 0;

        int maxHistogramValue = 0;
        
        for (int i = 0; i < 256; i++)
        {
            histogram[i] = 0;
        }

        const Mat src = this->get();

        for (int row = 0; row < src.rows; ++row)
        {
            for (int col = 0; col < src.cols; ++col)
            {
                const Vec3b srcColors = src.at<Vec3b>(row, col);
                const int blue = srcColors(0);
                const int green = srcColors(1);
                const int red = srcColors(2);

                const int grayValue = this->getGrayValueFromRGB(red, green, blue);

                histogram[grayValue] += 1;
                if (histogram[grayValue] > maxHistogramValue) {
                    maxHistogramValue = histogram[grayValue];
                }
            }
        }

        return maxHistogramValue;
    }

    void Image::applyBrightness(int value) 
    {
        if (this->isEmpty()) return;

        if (value < -255) return;
        if (value > 255) return;

        const Mat src = this->get();
        Mat3b dst(src.rows, src.cols);

        for (int row = 0; row < src.rows; ++row)
        {
            for (int col = 0; col < src.cols; ++col)
            {
                const Vec3b srcColors = src.at<Vec3b>(row, col);
                const int newR = srcColors(2) + value;
                const int newG = srcColors(1) + value;
                const int newB = srcColors(0) + value;
                
                dst.at<Vec3b>(row, col)[2] = this->filterNewChannelValue(newR);
                dst.at<Vec3b>(row, col)[1] = this->filterNewChannelValue(newG);
                dst.at<Vec3b>(row, col)[0] = this->filterNewChannelValue(newB);
            }
        }

        this->set(dst);
    }

    void Image::applyContrast(int value)
    {
        if (this->isEmpty()) return;

        if (value <= 0) return;
        if (value > 255) return;

        const Mat src = this->get();
        Mat3b dst(src.rows, src.cols);

        for (int row = 0; row < src.rows; ++row)
        {
            for (int col = 0; col < src.cols; ++col)
            {
                const Vec3b srcColors = src.at<Vec3b>(row, col);
                const int newR = srcColors(2) * value;
                const int newG = srcColors(1) * value;
                const int newB = srcColors(0) * value;

                dst.at<Vec3b>(row, col)[2] = this->filterNewChannelValue(newR);
                dst.at<Vec3b>(row, col)[1] = this->filterNewChannelValue(newG);
                dst.at<Vec3b>(row, col)[0] = this->filterNewChannelValue(newB);
            }
        }

        this->set(dst);
    }

    void Image::applyNegativeFilter() {
        if (this->isEmpty()) return;

        const Mat src = this->get();
        Mat3b dst(src.rows, src.cols);

        for (int row = 0; row < src.rows; ++row)
        {
            for (int col = 0; col < src.cols; ++col)
            {
                const Vec3b srcColors = src.at<Vec3b>(row, col);
                const int newR = 255 - srcColors(2);
                const int newG = 255 - srcColors(1);
                const int newB = 255 - srcColors(0);

                dst.at<Vec3b>(row, col)[2] = this->filterNewChannelValue(newR);
                dst.at<Vec3b>(row, col)[1] = this->filterNewChannelValue(newG);
                dst.at<Vec3b>(row, col)[0] = this->filterNewChannelValue(newB);
            }
        }

        this->set(dst);
    }

    int Image::filterNewChannelValue(int value) 
    {
        return value < 0 ? 0 : value > 255 ? 255 : value;
    }

    void Image::applyHistogramEqualizationFilter()
    {
        if (this->isEmpty()) return;

        const Mat src = this->get();
        Mat3b dst(src.rows, src.cols);

        int histogram[256];
        int cumulativeHistogram[256];

        this->calculateHistogram(histogram);

        this->calculateCumulativeHistogram(cumulativeHistogram, histogram);

        for (int row = 0; row < src.rows; ++row)
        {
            for (int col = 0; col < src.cols; ++col)
            {
                const Vec3b srcColors = src.at<Vec3b>(row, col);
                const int newR = cumulativeHistogram[srcColors(2)];
                const int newG = cumulativeHistogram[srcColors(1)];
                const int newB = cumulativeHistogram[srcColors(0)];

                dst.at<Vec3b>(row, col)[2] = this->filterNewChannelValue(newR);
                dst.at<Vec3b>(row, col)[1] = this->filterNewChannelValue(newG);
                dst.at<Vec3b>(row, col)[0] = this->filterNewChannelValue(newB);
            }
        }

        this->set(dst);
    }

    int Image::getGrayValueFromRGB(int r, int g, int b)
    {
        return 0.114 * b + 0.587 * g + 0.299 * r;
    }

    void Image::applyHistogramMatchingFilter(Image* target)
    {
        const Mat src = this->get();
        Mat3b dst(src.rows, src.cols);
        const int normalizeRows = 256;
        int matchingHistogram[256];
        int sourceHistogram[256];
        int targetHistogram[256];
        int sourceCumulativeHistogram[256];
        int targetCumulativeHistogram[256];
        int normalizedTargetCumulativeHistogram[256];

        int sourceMaxValue = this->calculateHistogram(sourceHistogram);
        int targetMaxValue = target->calculateHistogram(targetHistogram);

        this->calculateCumulativeHistogram(sourceCumulativeHistogram, sourceHistogram);
        target->calculateCumulativeHistogram(targetCumulativeHistogram, targetHistogram);

        memcpy(normalizedTargetCumulativeHistogram, targetCumulativeHistogram, sizeof(int) * 256);

        this->normalizeHistogram(normalizeRows, sourceCumulativeHistogram, sourceMaxValue);
        this->normalizeHistogram(normalizeRows, normalizedTargetCumulativeHistogram, targetMaxValue);

        for (int i = 0; i < 256; i++)
        {
            const int sourceCumulativeValue = sourceCumulativeHistogram[i];
            int closestCumulativeValueInTarget = 512;
            int targetPosition;
            int distance;

            for (int ti = 0; ti < 256; ti++) 
            {
                distance = abs(sourceCumulativeValue - normalizedTargetCumulativeHistogram[ti]);
                if (distance < closestCumulativeValueInTarget) {
                    closestCumulativeValueInTarget = distance;
                    targetPosition = ti;
                }
            }

            matchingHistogram[i] = targetCumulativeHistogram[targetPosition];
        }

        for (int row = 0; row < src.rows; ++row)
        {
            for (int col = 0; col < src.cols; ++col)
            {
                const Vec3b srcColors = src.at<Vec3b>(row, col);
                const int newR = matchingHistogram[srcColors(2)];
                const int newG = matchingHistogram[srcColors(1)];
                const int newB = matchingHistogram[srcColors(0)];

                dst.at<Vec3b>(row, col)[2] = this->filterNewChannelValue(newR);
                dst.at<Vec3b>(row, col)[1] = this->filterNewChannelValue(newG);
                dst.at<Vec3b>(row, col)[0] = this->filterNewChannelValue(newB);
            }
        }

        this->set(dst);
    }

    void Image::normalizeHistogram(int targetRows, int histogram[256], int maxHistogramValue)
    {
        for (int i = 0; i < 256; i++)
        {
            histogram[i] = ((double)histogram[i] / maxHistogramValue) * targetRows;
        }
    }

    void Image::calculateCumulativeHistogram(int* cumulativeHistogram, int* histogram)
    {
        const float histogramAlpha = 255.0 / (this->get().rows * this->get().cols);

        cumulativeHistogram[0] = histogramAlpha * histogram[0];

        for (int i = 1; i < 256; i++)
        {
            cumulativeHistogram[i] = cumulativeHistogram[i - 1] + histogramAlpha * histogram[i];
        }
    }
}