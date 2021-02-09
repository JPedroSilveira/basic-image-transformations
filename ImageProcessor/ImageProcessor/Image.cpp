#include "Image.h"

const string DEFAULT_SAVE_PATH = "C:/Users/55519/Documents/Git/processador-de-imagens/";

namespace type {
    void Image::load(string path) {
        if (!path.empty()) {
            this->image = imread(path, IMREAD_COLOR);
        }
    }

    bool Image::write(string filename) {
        if (!this->empty()) {
            return imwrite(filename, this->image);
        }
        return false;
    }

    Mat Image::get() {
        return this->image;
    }

    void Image::set(Mat image) {
        this->image = image;
    }

    bool Image::empty() {
        return this->image.empty();
    }

    void Image::horizontalFlip() {
        const Mat3b src = image;
        Mat3b dst(src.rows, src.cols);
        for (int row = 0; row < dst.rows; ++row) {
            for (int col = 0; col < dst.cols; ++col) {
                const Vec3b srcUnit = src(row, src.cols - 1 - col);
                memcpy(&dst(row, col), &srcUnit, sizeof(srcUnit));
            }
        }
        this->set(dst);
    }
}