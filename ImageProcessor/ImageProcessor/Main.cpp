#include "Main.h"

using namespace cv;
using namespace std;

#include <opencv2/opencv.hpp>
#include "cvui.h" 

#define WINDOW_NAME "CVUI Hello World!" 

wxArrayString getQuantizeOptions() {
	wxArrayString choices;
	for (int i = 1; i < 256; i++)
	{
		const string str = to_string(i);
		choices.Add(str);
	}

	return choices;
}

namespace view
{
	wxBEGIN_EVENT_TABLE(Main, wxFrame)
		EVT_BUTTON(10001, onOpenButtonClick)
		EVT_BUTTON(10002, onSaveButtonClick)
		EVT_BUTTON(10003, onFlipHButtonClick)
		EVT_BUTTON(10004, onFlipVButtonClick)
		EVT_BUTTON(10005, onGrayVButtonClick)
		EVT_BUTTON(10006, onQuantizeButtonClick)
		EVT_BUTTON(10007, onResetButtonClick)
	wxEND_EVENT_TABLE()

	Main::Main() : wxFrame(nullptr, wxID_ANY, "Image Processor", wxPoint(30,30), wxSize(800,450))
	{
		openButton = new wxButton(this, 10001, "Open image", wxPoint(10, 10), wxSize(360, 50));
		saveButton = new wxButton(this, 10002, "Save image", wxPoint(410, 10), wxSize(360, 50));
		flipHButton = new wxButton(this, 10003, "Flip Horizontally", wxPoint(10, 70), wxSize(175, 30));
		flipVButton = new wxButton(this, 10004, "Flip Vertically", wxPoint(195, 70), wxSize(175, 30));
		grayButton = new wxButton(this, 10005, "Gray scale", wxPoint(410, 70), wxSize(175, 30));
		quantizeButton = new wxButton(this, 10006, "Quantize", wxPoint(595, 70), wxSize(175, 30));
		resetButton = new wxButton(this, 10007, "Reset", wxPoint(10, 110), wxSize(175, 30)); 
		imageDimTextCtrl = new wxTextCtrl(this, wxID_ANY, "", wxPoint(10, 160), wxSize(300, 20), wxTE_READONLY);
		logListBox = new wxListBox(this, wxID_ANY, wxPoint(10, 190), wxSize(760, 210));
		quantizeValueDialog = new wxSingleChoiceDialog(this, "Select a value", "", getQuantizeOptions());
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
			this->loadOriginalImageFromDialog();
			if (!originalImageFile->empty()) {
				this->loadProcessedImageFromOriginal();
				
				this->updateOriginalImageView();
				this->updateProcessedImageView();

				this->log("Image loaded");
			}
			else 
			{
				this->log("Error loading image");
			}
		}

		evt.Skip();
	}

	void Main::onSaveButtonClick(wxCommandEvent& evt)
	{
		if (processedImageFile->empty()) {
			evt.Skip(); 
			return;
		}

		if (dirDialog->ShowModal() != wxID_CANCEL) {
			const std::string save_path = dirDialog->GetPath().ToStdString();
			const string finalPath = save_path + filenameUtil->generateAutoJPGName();
			const bool success = processedImageFile->write(finalPath);
			if (success) {
				this->log("Image saved: " + finalPath);
			}
			else
			{
				this->log("Error saving image: " + finalPath);
			}
		}

		evt.Skip();
	}

	void Main::onFlipHButtonClick(wxCommandEvent& evt) 
	{
		if (processedImageFile->empty()) {
			evt.Skip();
			return;
		}

		this->processedImageFile->horizontalFlip();
		this->updateProcessedImageView();
		this->log("Image fliped horizontally");
		
		evt.Skip();
	}

	void Main::onFlipVButtonClick(wxCommandEvent& evt)
	{
		if (processedImageFile->empty()) {
			evt.Skip();
			return;
		}

		this->processedImageFile->verticalFlip();
		this->updateProcessedImageView();
		this->log("Image fliped vertically");
		evt.Skip();
	}

	void Main::onGrayVButtonClick(wxCommandEvent& evt)
	{
		if (processedImageFile->empty()) {
			evt.Skip();
			return;
		}

		this->processedImageFile->toGrayScale();
		this->updateProcessedImageView();
		this->log("Gray filter aplied");
		evt.Skip();
	}

	void Main::onQuantizeButtonClick(wxCommandEvent& evt)
	{
		if (processedImageFile->empty()) {
			evt.Skip();
			return;
		}

		if (quantizeValueDialog->ShowModal() != wxID_CANCEL) {
			const std::string value = quantizeValueDialog->GetStringSelection().ToStdString();

			stringstream valueStream(value);

			int selectedValue = 0;

			valueStream >> selectedValue;

			this->processedImageFile->quantizeGrayScaleImage(selectedValue);
			this->updateProcessedImageView();
			this->log("Quantization applied to grayscale image");
		}

		evt.Skip();
	}

	void Main::onResetButtonClick(wxCommandEvent& evt)
	{
		if (processedImageFile->empty()) {
			evt.Skip(); 
			return;
		}

		this->processedImageFile->set(originalImageFile->get());
		this->updateProcessedImageView();
		this->log("Reset");
		evt.Skip();
	}

	void Main::log(string log)
	{
		this->logListBox->AppendString(log);
		this->logListBox->SetSelection(logListBox->GetCount() - 1);
	}

	void Main::updateOriginalImageView() {
		if (originalImageFile->empty()) return;

		const Mat image = originalImageFile->get();
		imshow("Original Image", image);

		const int width = image.size().width;
		const int height = image.size().height;
		this->imageDimTextCtrl->WriteText(to_string(width) + "x" + to_string(height));
	}

	void Main::updateProcessedImageView()
	{
		if (processedImageFile->empty()) return;
		imshow("Processed Image", processedImageFile->get());
	}

	void Main::loadOriginalImageFromDialog()
	{
		const std::string image_path = fileDialog->GetPath().ToStdString();
		this->originalImageFile->load(image_path);
	}

	void Main::loadProcessedImageFromOriginal()
	{
		const Mat image = originalImageFile->get();
		this->processedImageFile->set(image);
	}
}