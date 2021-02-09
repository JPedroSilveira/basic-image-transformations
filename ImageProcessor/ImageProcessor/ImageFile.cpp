#include "ImageFile.h"

const string DEFAULT_SAVE_PATH = "C:/Users/55519/Documents/Git/processador-de-imagens/";

namespace type {
    void ImageFile::load(string path) {
        if (!path.empty()) {
            this->image = imread(path, IMREAD_COLOR);
        }
    }

    bool ImageFile::write(string filename) {
        if (!this->empty()) {
            return imwrite(filename, this->image);
        }
        return false;
    }

    Mat ImageFile::get() {
        return this->image;
    }

    void ImageFile::set(Mat image) {
        this->image = image;
    }

    bool ImageFile::empty() {
        return this->image.empty();
    }
}