#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <wx/wx.h>
#include "Main.h"

using namespace cv;
using namespace view;

namespace app {
    class App : public wxApp
    {
    public:
        App();
        ~App();
    public:
        virtual bool OnInit();
    private:
        Main* mainFrame = nullptr;
    };
}
