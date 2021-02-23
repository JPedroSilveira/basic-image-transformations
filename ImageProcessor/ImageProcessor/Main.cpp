#include "Main.h"

using namespace cv;
using namespace std;

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
		EVT_CLOSE(onFrameClosed)
		EVT_BUTTON(10001, onOpenButtonClick)
		EVT_BUTTON(10002, onSaveButtonClick)
		EVT_BUTTON(10003, onFlipHButtonClick)
		EVT_BUTTON(10004, onFlipVButtonClick)
		EVT_BUTTON(10005, onGrayVButtonClick)
		EVT_BUTTON(10006, onQuantizeButtonClick)
		EVT_BUTTON(10007, onShowImagesButtonClick)
		EVT_BUTTON(10008, onResetButtonClick)
		EVT_BUTTON(10009, onHistogramButtonClick)
	wxEND_EVENT_TABLE()

	Main::Main() : wxFrame(nullptr, 10000, "Image Processor", wxPoint(30,30), wxSize(1030,450))
	{
		this->openButton = new wxButton(this, 10001, "Open image", wxPoint(10, 10), wxSize(360, 50));
		this->saveButton = new wxButton(this, 10002, "Save image", wxPoint(410, 10), wxSize(360, 50));
		this->flipHButton = new wxButton(this, 10003, "Flip Horizontally", wxPoint(10, 70), wxSize(175, 30));
		this->flipVButton = new wxButton(this, 10004, "Flip Vertically", wxPoint(195, 70), wxSize(175, 30));
		this->grayButton = new wxButton(this, 10005, "Gray scale", wxPoint(410, 70), wxSize(175, 30));
		this->quantizeButton = new wxButton(this, 10006, "Quantize", wxPoint(595, 70), wxSize(175, 30));
		this->showImagesButton = new wxButton(this, 10007, "Show images", wxPoint(10, 110), wxSize(175, 30));
		this->resetButton = new wxButton(this, 10008, "Reset image", wxPoint(195, 110), wxSize(175, 30));
		this->histogramButton = new wxButton(this, 10009, "Histogram", wxPoint(410, 110), wxSize(175, 30));
		this->imageDimTextCtrl = new wxTextCtrl(this, wxID_ANY, "", wxPoint(10, 380), wxSize(300, 20), wxTE_READONLY);
		this->logListBox = new wxListBox(this, wxID_ANY, wxPoint(800, 10), wxSize(200, 390));
		this->quantizeValueDialog = new wxSingleChoiceDialog(this, "Select a value", "", getQuantizeOptions());
		this->fileDialog = new wxFileDialog(this, "Select an image", "", "", "JPG files(*.jpg;*.jpeg) | *.jpg;*.jpeg", wxFD_OPEN);
		this->dirDialog = new wxDirDialog(this, "Select a directory", "");
		this->originalImageFile = new Image();
		this->processedImageFile = new Image();
		this->filenameUtil = new FilenameUtil();
		this->updateImageDependentComponents();
	}

	Main::~Main()
	{
	}

	void Main::onFrameClosed(wxCloseEvent& evt)
	{
		this->Destroy();
	}

	void Main::onOpenButtonClick(wxCommandEvent& evt)
	{
		if (fileDialog->ShowModal() != wxID_CANCEL) {
			this->loadOriginalImageFromDialog();
			if (!originalImageFile->isEmpty()) {
				this->loadProcessedImageFromOriginal();
				
				this->updateOriginalImageView();
				this->updateProcessedImageView();
				this->updateImageDependentComponents();

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
		if (processedImageFile->isEmpty()) {
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
		if (processedImageFile->isEmpty()) {
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
		if (processedImageFile->isEmpty()) {
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
		if (processedImageFile->isEmpty()) {
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
		if (processedImageFile->isEmpty()) {
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
		if (processedImageFile->isEmpty()) {
			evt.Skip(); 
			return;
		}

		this->processedImageFile->set(originalImageFile->get());
		this->updateProcessedImageView();
		this->log("Reset");
		evt.Skip();
	}

	void Main::onShowImagesButtonClick(wxCommandEvent& evt)
	{
		if (processedImageFile->isEmpty()) {
			evt.Skip();
			return;
		}

		this->updateOriginalImageView();
		this->updateProcessedImageView();
		this->log("Show images");
		evt.Skip();
	}


	void Main::onHistogramButtonClick(wxCommandEvent& evt)
	{
		if (processedImageFile->isEmpty()) {
			evt.Skip();
			return;
		}

		this->createHistogram();
		this->updateProcessedImageView();
		this->log("Histogram created");
		evt.Skip();
	}

	void Main::log(string log)
	{
		this->logListBox->AppendString(log);
		this->logListBox->SetSelection(logListBox->GetCount() - 1);
	}

	void Main::updateOriginalImageView() {
		if (originalImageFile->isEmpty()) return;

		const Mat image = originalImageFile->get();
		imshow("Original Image", image);

		const int width = image.size().width;
		const int height = image.size().height;
		this->imageDimTextCtrl->Clear();
		this->imageDimTextCtrl->WriteText(to_string(width) + "x" + to_string(height));
	}

	void Main::updateProcessedImageView()
	{
		if (processedImageFile->isEmpty()) return;
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

	void Main::updateImageDependentComponents()
	{
		if (this->processedImageFile->isEmpty()) {
			this->saveButton->Disable();
			this->flipHButton->Disable();
			this->flipVButton->Disable();
			this->grayButton->Disable();
			this->quantizeButton->Disable();
			this->resetButton->Disable();
			this->showImagesButton->Disable();
			this->histogramButton->Disable();
		} 
		else
		{
			this->saveButton->Enable();
			this->flipHButton->Enable();
			this->flipVButton->Enable();
			this->grayButton->Enable();
			this->quantizeButton->Enable();
			this->resetButton->Enable();
			this->showImagesButton->Enable();
			this->histogramButton->Enable();
		}
	}

	void Main::createHistogram()
	{
		this->processedImageFile->calculateHistogram();
		const int* histogram = this->processedImageFile->getHistogram();
		int hist[256];

		for (int i = 0; i < 256; i++)
		{
			hist[i] = histogram[i];
		}

		// draw the histograms
		int hist_w = 256; 
		int hist_h = 256;
		int bin_w = cvRound((double)hist_w / 256);

		Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(255, 255, 255));

		// find the maximum intensity element from histogram
		int max = hist[0];
		for (int i = 1; i < 256; i++) {
			if (max < hist[i]) {
				max = hist[i];
			}
		}

		// normalize the histogram between 0 and histImage.rows
		for (int i = 0; i < 256; i++)
		{
			hist[i] = ((double)hist[i] / this->processedImageFile->getMaxHistogramValue()) * histImage.rows;
		}

		// draw the intensity line for histogram
		for (int i = 0; i < 256; i++)
		{
			line(histImage, Point(bin_w * (i), hist_h), Point(bin_w * (i), hist_h - hist[i]), Scalar(0, 0, 0), 1, 8, 0);
		}

		// display histogram
		imshow("Histogram", histImage);
	}
}