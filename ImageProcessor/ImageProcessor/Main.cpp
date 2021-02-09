#include "Main.h"

using namespace cv;

namespace view
{
	wxBEGIN_EVENT_TABLE(Main, wxFrame)
		EVT_BUTTON(10001, onOpenButtonClick)
		EVT_BUTTON(10002, onSaveButtonClick)
		EVT_BUTTON(10003, onFlipHButtonClick)
		EVT_BUTTON(10004, onFlipVButtonClick)
	wxEND_EVENT_TABLE()

	Main::Main() : wxFrame(nullptr, wxID_ANY, "Image Processor", wxPoint(30,30), wxSize(800,450))
	{
		openButton = new wxButton(this, 10001, "Open image", wxPoint(10, 10), wxSize(360, 50));
		saveButton = new wxButton(this, 10002, "Save image", wxPoint(410, 10), wxSize(360, 50));
		flipHButton = new wxButton(this, 10003, "Flip Horizontally", wxPoint(10, 70), wxSize(175, 30));
		flipVButton = new wxButton(this, 10004, "Flip Vertically", wxPoint(195, 70), wxSize(175, 30));
		imageDimTextCtrl = new wxTextCtrl(this, wxID_ANY, "", wxPoint(10, 110), wxSize(360, 20), wxTE_READONLY);
		logListBox = new wxListBox(this, wxID_ANY, wxPoint(10, 140), wxSize(760, 250));
		fileDialog = new wxFileDialog(this, "Select an image", "", "", "JPG files(*.jpg;*.jpeg) | *.jpg;*.jpeg", wxFD_OPEN);
		dirDialog = new wxDirDialog(this, "Select a directory", "");
		originalImageFile = new Image();
		processedImageFile = new Image();
		filenameUtil = new FilenameUtil();
	}

	Main::~Main()
	{
	}

	void Main::onOpenButtonClick(wxCommandEvent& evt)
	{
		if (fileDialog->ShowModal() != wxID_CANCEL) {
			loadOriginalImageFromDialog();
			if (!originalImageFile->empty()) {
				loadProcessedImageFromOriginal();
				
				updateOriginalImageView();
				updateProcessedImageView();

				log("Image loaded");
			}
			else 
			{
				log("Error loading image");
			}
		}

		evt.Skip();
	}

	void Main::onSaveButtonClick(wxCommandEvent& evt)
	{
		if (!originalImageFile->empty()) {
			if (dirDialog->ShowModal() != wxID_CANCEL) {
				const std::string save_path = dirDialog->GetPath().ToStdString();
				const string finalPath = save_path + filenameUtil->generateAutoJPGName();
				const bool success = originalImageFile->write(finalPath);
				if (success) {
					log("Image saved: " + finalPath);
				}
				else
				{
					log("Error saving image: " + finalPath);
				}
			}
		}

		evt.Skip();
	}

	void Main::onFlipHButtonClick(wxCommandEvent& evt) 
	{
		processedImageFile->horizontalFlip();
		updateProcessedImageView();
		log("Image fliped horizontally");
		evt.Skip();
	}

	void Main::onFlipVButtonClick(wxCommandEvent& evt)
	{
		processedImageFile->verticalFlip();
		updateProcessedImageView();
		log("Image fliped vertically");
		evt.Skip();
	}

	void Main::log(string log)
	{
		logListBox->AppendString(log);
		logListBox->SetSelection(logListBox->GetCount() - 1);
	}

	void Main::updateOriginalImageView() {
		const Mat image = originalImageFile->get();
		imshow("Original Image", image);

		const int width = image.size().width;
		const int height = image.size().height;
		imageDimTextCtrl->WriteText(to_string(width) + "x" + to_string(height));
	}

	void Main::updateProcessedImageView() 
	{
		imshow("Processed Image", processedImageFile->get());
	}

	void Main::loadOriginalImageFromDialog()
	{
		const std::string image_path = fileDialog->GetPath().ToStdString();
		originalImageFile->load(image_path);
	}

	void Main::loadProcessedImageFromOriginal()
	{
		const Mat image = originalImageFile->get();
		processedImageFile->set(image);
	}
}