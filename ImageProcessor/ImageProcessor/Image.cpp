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
        if (this->isEmpty()) return;

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

    void Image::applyEasterEggOne(int rows, int columns)
    {
        if (this->isEmpty()) return;

        const Mat src = this->get();
        Mat3b dst(rows, columns);

        vector<vector<int>> redMatrix(rows, vector<int>(columns));
        vector<vector<int>> greenMatrix(rows, vector<int>(columns));
        vector<vector<int>> blueMatrix(rows, vector<int>(columns));
        vector<int> count(rows);

        for (int row = 0; row < src.rows; row = row + rows)
        {
            for (int column = 0; column < src.cols; column = column + columns)
            {
                for (int dstRow = 0; dstRow < dst.rows && dstRow + row < src.rows; dstRow++)
                {
                    for (int dstColumn = 0; dstColumn < dst.cols && dstColumn + column < src.cols; dstColumn++)
                    {
                        const Vec3b srcColors = src.at<Vec3b>(row + dstRow, column + dstColumn);
                        const int r = srcColors(2);
                        const int g = srcColors(1);
                        const int b = srcColors(0);

                        redMatrix[dstRow][dstColumn] = redMatrix[dstRow][dstColumn] + r;
                        greenMatrix[dstRow][dstColumn] = greenMatrix[dstRow][dstColumn] + g;
                        blueMatrix[dstRow][dstColumn] = blueMatrix[dstRow][dstColumn] + b;
                    }
                    count[dstRow]++;
                }
            }
        }

        for (int row = 0; row < dst.rows; row++)
        {
            for (int col = 0; col < dst.cols; col++)
            {
                const int newR = redMatrix[row][col] / count[row];
                const int newG = greenMatrix[row][col] / count[row];
                const int newB = blueMatrix[row][col] / count[row];

                dst.at<Vec3b>(row, col)[2] = this->filterNewChannelValue(newR);
                dst.at<Vec3b>(row, col)[1] = this->filterNewChannelValue(newG);
                dst.at<Vec3b>(row, col)[0] = this->filterNewChannelValue(newB);
            }
        }

        this->set(dst);
    }

    void Image::applyZoomOut(int x, int y)
    {
        if (this->isEmpty()) return;

        const Mat src = this->get();
        const int newXSize = floor(src.rows / x);
        const int newYSize = floor(src.cols / y);
        if (newXSize < 2) return;
        if (newYSize < 2) return;

        Mat3b dst(newXSize, newYSize);

        int srcCol;
        int srcRow = 0;
        for (int dstRow = 0; dstRow < dst.rows; dstRow++)
        {
            srcCol = 0;
            for (int dstCol = 0; dstCol < dst.cols; dstCol++)
            {
                int rValue = 0;
                int gValue = 0;
                int bValue = 0;
                int count = 0;
                for (int xi = 0; xi < x && (srcRow + xi) < src.rows; xi++)
                {
                    for (int yi = 0; yi < y && (srcCol + yi) < src.cols; yi++)
                    {
                        const Vec3b srcColors = src.at<Vec3b>(srcRow + xi, srcCol + yi);

                        rValue += srcColors(2);
                        gValue += srcColors(1);
                        bValue += srcColors(0);
                        count++;
                    }
                }
                srcCol+=x;

                if (count > 0) {
                    dst.at<Vec3b>(dstRow, dstCol)[2] = floor(rValue / count);
                    dst.at<Vec3b>(dstRow, dstCol)[1] = floor(gValue / count);
                    dst.at<Vec3b>(dstRow, dstCol)[0] = floor(bValue / count);
                }
            }
            srcRow+=y;

            if (srcRow > src.rows) {
                srcRow = 0;
            }
        }

        this->set(dst);
    }

    void Image::applyZoomIn()
    {
        if (this->isEmpty()) return;

        const Mat src = this->get();
        const int newXSize = src.rows * 2 - 1;
        const int newYSize = src.cols * 2 - 1;

        if (newXSize > 4000) return;
        if (newYSize > 4000) return;

        Mat3b dst(newXSize, newYSize);

        int srcRow = 0;
        int srcCol = 0;

        for (int dstRow = 0; dstRow < dst.rows; dstRow++) // First step with existent rows
        {
            if (dstRow % 2 != 0) { // First step analizes only original rows
                srcRow++;
                continue;
            };
            srcCol = 0;

            for (int dstCol = 0; dstCol < dst.cols; dstCol++)
            {
                if (dstCol % 2 == 0) // In original columns
                {
                    const Vec3b srcColors = src.at<Vec3b>(srcRow, srcCol);

                    dst.at<Vec3b>(dstRow, dstCol)[2] = srcColors(2);
                    dst.at<Vec3b>(dstRow, dstCol)[1] = srcColors(1);
                    dst.at<Vec3b>(dstRow, dstCol)[0] = srcColors(0);
                        
                    srcCol++;
                }
                else // Between original columns
                {
                    const Vec3b srcLeftColors = src.at<Vec3b>(srcRow, srcCol);
                    const Vec3b srcRightColors = src.at<Vec3b>(srcRow, srcCol - 1);

                    dst.at<Vec3b>(dstRow, dstCol)[2] = (srcLeftColors(2) + srcRightColors(2))/2;
                    dst.at<Vec3b>(dstRow, dstCol)[1] = (srcLeftColors(1) + srcRightColors(1))/2;
                    dst.at<Vec3b>(dstRow, dstCol)[0] = (srcLeftColors(0) + srcRightColors(0))/2;
                }
            }
        }

        srcRow = 0;
        for (int dstRow = 0; dstRow < dst.rows; dstRow++) // Second step fills new empty rows between original pixels
        {
            if (dstRow % 2 == 0) { // Second step analizes only new rows
                srcRow++;
                continue;
            };
            srcCol = 0;

            for (int dstCol = 0; dstCol < dst.cols; dstCol++)
            {
                if (dstCol % 2 != 0) // Second analize only new rows where top and bottom pixels are from original image
                {
                    srcCol++;
                    continue;
                }

                const Vec3b srcTopColors = src.at<Vec3b>(srcRow - 1, srcCol);
                const Vec3b srcBottomColors = src.at<Vec3b>(srcRow, srcCol);

                dst.at<Vec3b>(dstRow, dstCol)[2] = (srcTopColors(2) + srcBottomColors(2)) / 2;
                dst.at<Vec3b>(dstRow, dstCol)[1] = (srcTopColors(1) + srcBottomColors(1)) / 2;
                dst.at<Vec3b>(dstRow, dstCol)[0] = (srcTopColors(0) + srcBottomColors(0)) / 2;
            }
            if (dstRow % 2 == 0) {
                srcRow++;
            }
            srcCol = 0;
        }

        for (int dstRow = 0; dstRow < dst.rows; dstRow++) // Third step fills new empty rows between generated pixels
        {
            if (dstRow % 2 == 0)  continue; // Third step analizes only new rows

            for (int dstCol = 0; dstCol < dst.cols; dstCol++)
            {
                if (dstCol % 2 == 0) continue; // Third analize only new rows where top and bottom pixels are generated pixels

                const Vec3b dstTopColors = dst.at<Vec3b>(dstRow, dstCol - 1);
                const Vec3b dstBottomColors = dst.at<Vec3b>(dstRow, dstCol + 1);
                const Vec3b dstLeftColors = dst.at<Vec3b>(dstRow - 1, dstCol);
                const Vec3b dstRightColors = dst.at<Vec3b>(dstRow + 1, dstCol);
                const Vec3b dstTopLeftColors = dst.at<Vec3b>(dstRow - 1, dstCol - 1);
                const Vec3b dstTopRightColors = dst.at<Vec3b>(dstRow - 1, dstCol + 1);
                const Vec3b dstBottomLeftColors = dst.at<Vec3b>(dstRow + 1, dstCol - 1);
                const Vec3b dstBottomRightColors = dst.at<Vec3b>(dstRow + 1, dstCol + 1);

                dst.at<Vec3b>(dstRow, dstCol)[2] = (dstTopColors(2) + dstBottomColors(2) + dstLeftColors(2) + dstRightColors(2) +
                    dstTopLeftColors(2) + dstTopRightColors(2) + dstBottomLeftColors(2) + dstBottomRightColors(2)) / 8;
                dst.at<Vec3b>(dstRow, dstCol)[1] = (dstTopColors(1) + dstBottomColors(1) + dstLeftColors(1) + dstRightColors(1) +
                    dstTopLeftColors(1) + dstTopRightColors(1) + dstBottomLeftColors(1) + dstBottomRightColors(1)) / 8;
                dst.at<Vec3b>(dstRow, dstCol)[0] = (dstTopColors(0) + dstBottomColors(0) + dstLeftColors(0) + dstRightColors(0) +
                    dstTopLeftColors(0) + dstTopRightColors(0) + dstBottomLeftColors(0) + dstBottomRightColors(0)) / 8;
            }
            if (dstRow % 2 == 0) {
                srcRow++;
            }
            srcCol = 0;
        }

        this->set(dst);
    }
}