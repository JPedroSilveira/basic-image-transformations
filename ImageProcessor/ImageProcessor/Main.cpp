#include "Main.h"

using namespace cv;

namespace view
{
	wxBEGIN_EVENT_TABLE(Main, wxFrame)
		EVT_BUTTON(10001, onOpenButtonClick)
		EVT_BUTTON(10002, onSaveButtonClick)
	wxEND_EVENT_TABLE()

	Main::Main() : wxFrame(nullptr, wxID_ANY, "Image Processor", wxPoint(30,30), wxSize(800,450))
	{
		openButton = new wxButton(this, 10001, "Open image", wxPoint(10, 10), wxSize(360, 50));
		saveButton = new wxButton(this, 10002, "Save image", wxPoint(410, 10), wxSize(360, 50));
		logListBox = new wxListBox(this, wxID_ANY, wxPoint(10, 70), wxSize(760, 300));
		fileDialog = new wxFileDialog(this, "Select an image", "", "", "JPG files(*.jpg;*.jpeg) | *.jpg;*.jpeg", wxFD_OPEN);
		dirDialog = new wxDirDialog(this, "Select a directory", "");
		originalImage = new ImageFile();
		filenameUtil = new FilenameUtil();
	}

	Main::~Main()
	{
	}

	void Main::onOpenButtonClick(wxCommandEvent& evt)
	{
		if (fileDialog->ShowModal() != wxID_CANCEL) {
			const std::string image_path = fileDialog->GetPath().ToStdString();
			originalImage->load(image_path);
			if (!originalImage->empty()) {
				imshow("Original Image", originalImage->get());
				logListBox->AppendString("Image loaded: " + image_path);
			}
			else 
			{
				logListBox->AppendString("Error loading image: " + image_path);
			}
		}

		evt.Skip();
	}

	void Main::onSaveButtonClick(wxCommandEvent& evt)
	{
		if (!originalImage->empty()) {
			if (dirDialog->ShowModal() != wxID_CANCEL) {
				const std::string save_path = dirDialog->GetPath().ToStdString();
				const string finalPath = save_path + filenameUtil->generateAutoJPGName();
				const bool success = originalImage->write(finalPath);
				if (success) {
					logListBox->AppendString("Image saved: " + finalPath);
				}
				else
				{
					logListBox->AppendString("Error saving image: " + finalPath);
				}
			}
		}

		evt.Skip();
	}
}