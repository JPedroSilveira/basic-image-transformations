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