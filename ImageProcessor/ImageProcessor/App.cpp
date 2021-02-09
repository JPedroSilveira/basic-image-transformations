#include "App.h"

namespace app 
{
    wxIMPLEMENT_APP(App);

    App::App()
    {
    }

    App::~App()
    {
    }
    bool App::OnInit() 
    {
        mainFrame = new Main();
        mainFrame->Show();

        return true;
    }
}

/*
int main()
{
    std::string image_path = samples::findFile("starry_night.jpg");
    ImageFile file(image_path);
    file.load();
    if (file.empty()) {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return 1;
    }
    imshow("Display window 1", file.get());
    imshow("Display window 2", file.get());
    int k = waitKey(0);
    if (k == 's')
    {
        file.write("starry_night.jpg");
    }
    return 0;
}
*/