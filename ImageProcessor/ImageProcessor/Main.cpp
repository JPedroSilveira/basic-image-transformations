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
		EVT_SLIDER(10010, onBrightnessSliderChange)
		EVT_BUTTON(10011, onBrightnessApplyButtonClick)
		EVT_SLIDER(10012, onContrastSliderChange)
		EVT_BUTTON(10013, onContrastApplyButtonClick)
		EVT_BUTTON(10014, onNegativeButtonClick)
		EVT_BUTTON(10015, onHistogramEqualizationButtonClick)
		EVT_BUTTON(10016, onHistogramMatchingButtonClick)
	wxEND_EVENT_TABLE()

	Main::Main() : wxFrame(nullptr, 10000, "Image Processor", wxPoint(30,30), wxSize(1030,450))
	{
		//OPEN & SAVE
		this->openButton = new wxButton(this, 10001, "Open image", wxPoint(10, 10), wxSize(360, 50));
		this->saveButton = new wxButton(this, 10002, "Save image", wxPoint(410, 10), wxSize(360, 50));
		this->fileDialog = new wxFileDialog(this, "Select an image", "", "", "JPG files(*.jpg;*.jpeg) | *.jpg;*.jpeg", wxFD_OPEN);
		this->dirDialog = new wxDirDialog(this, "Select a directory", "");

		//FLIP
		this->flipHButton = new wxButton(this, 10003, "Flip Horizontally", wxPoint(10, 70), wxSize(175, 30));
		this->flipVButton = new wxButton(this, 10004, "Flip Vertically", wxPoint(195, 70), wxSize(175, 30));

		//GRAY SCALE
		this->grayButton = new wxButton(this, 10005, "Gray scale", wxPoint(410, 70), wxSize(175, 30));

		//QUANTIZE
		this->quantizeButton = new wxButton(this, 10006, "Quantize", wxPoint(595, 70), wxSize(175, 30));
		this->quantizeValueDialog = new wxSingleChoiceDialog(this, "Select a value", "", getQuantizeOptions());

		//IMAGE CONTROL
		this->showImagesButton = new wxButton(this, 10007, "Show images", wxPoint(10, 110), wxSize(175, 30));
		this->resetButton = new wxButton(this, 10008, "Reset image", wxPoint(195, 110), wxSize(175, 30));

		//HISTOGRAM
		this->histogramButton = new wxButton(this, 10009, "Histogram", wxPoint(410, 110), wxSize(175, 30));

		//BRIGHTNESS
		this->brightnessStaticBox = new wxStaticBox(this, wxID_ANY, "Brightness", wxPoint(10, 150), wxSize(360, 60));
		this->brightnessSlider = new wxSlider(this, 10010, 0, -255, +255, wxPoint(20, 170), wxSize(175, 30));
		this->brightnessTextCtrl = new wxTextCtrl(this, wxID_ANY, "", wxPoint(195, 172), wxSize(40, 20), wxTE_READONLY);
		this->brightnessApplyButton = new wxButton(this, 10011, "Apply", wxPoint(245, 168), wxSize(115, 30));

		//CONTRAST
		this->contrastStaticBox = new wxStaticBox(this, wxID_ANY, "Contrast", wxPoint(10, 220), wxSize(360, 60));
		this->contrastSlider = new wxSlider(this, 10012, 0, 0, +255, wxPoint(20, 240), wxSize(175, 30));
		this->contrastTextCtrl = new wxTextCtrl(this, wxID_ANY, "", wxPoint(195, 242), wxSize(40, 20), wxTE_READONLY);
		this->contrastApplyButton = new wxButton(this, 10013, "Apply", wxPoint(245, 238), wxSize(115, 30));

		//NEGATIVE
		this->negativeButton = new wxButton(this, 10014, "Negative", wxPoint(595, 110), wxSize(175, 30));

		//HISTOGRAM EQUALIZATION
		this->histogramEqualizationButton = new wxButton(this, 10015, "Histogram Equalization", wxPoint(410, 150), wxSize(175, 30));

		//HISTOGRAM MATCHING
		this->histogramMatchingButton = new wxButton(this, 10016, "Open target image", wxPoint(595, 150), wxSize(175, 30));

		//INFO
		this->imageDimTextCtrl = new wxTextCtrl(this, wxID_ANY, "", wxPoint(10, 380), wxSize(300, 20), wxTE_READONLY);
		this->logListBox = new wxListBox(this, wxID_ANY, wxPoint(800, 10), wxSize(200, 390));

		//IMAGES
		this->originalImageFile = new Image();
		this->processedImageFile = new Image();
		this->targetImageFile = new Image();

		//UTILS
		this->filenameUtil = new FilenameUtil();

		//INIT
		this->updateImageDependentComponents();
		this->cleanBrightnessData();
		this->cleanContrastData();
		this->brightnessTextCtrl->Disable();
		this->contrastTextCtrl->Disable();
		this->imageDimTextCtrl->Disable();
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
			this->loadImageFromDialog(this->originalImageFile);
			if (!originalImageFile->isEmpty()) {
				this->loadImageFromDialog(this->processedImageFile);
				
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

		this->processedImageFile->applyGrayScaleFilter();
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

			this->processedImageFile->applyQuantizeFilter(selectedValue);
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

		this->createHistogram("Histogram");
		this->updateProcessedImageView();
		this->log("Histogram created");
		evt.Skip();
	}

	void Main::onBrightnessSliderChange(wxCommandEvent& evt)
	{
		if (processedImageFile->isEmpty()) {
			evt.Skip();
			return;
		}

		const int value = this->brightnessSlider->GetValue();
		this->brightnessTextCtrl->Clear();
		this->brightnessTextCtrl->WriteText(this->convertNumberToString(value));
		evt.Skip();
	}

	void Main::onBrightnessApplyButtonClick(wxCommandEvent& evt) {
		if (processedImageFile->isEmpty()) {
			evt.Skip();
			return;
		}

		const int value = this->brightnessSlider->GetValue();
		this->processedImageFile->applyBrightness(value);
		this->cleanBrightnessData();
		this->updateProcessedImageView();
		this->log("Brightness of " + this->convertNumberToString(value) + " applied");
		evt.Skip();
	}

	void Main::onContrastSliderChange(wxCommandEvent& evt)
	{
		if (processedImageFile->isEmpty()) {
			evt.Skip();
			return;
		}

		const int value = this->contrastSlider->GetValue();
		this->contrastTextCtrl->Clear();
		this->contrastTextCtrl->WriteText(this->convertNumberToString(value));
		evt.Skip();
	}

	void Main::onContrastApplyButtonClick(wxCommandEvent& evt) {
		if (processedImageFile->isEmpty()) {
			evt.Skip();
			return;
		}

		const int value = this->contrastSlider->GetValue();
		this->processedImageFile->applyBrightness(value);
		this->cleanContrastData();
		this->updateProcessedImageView();
		this->log("Contrast of " + this->convertNumberToString(value) + " applied");
		evt.Skip();
	}

	void Main::onNegativeButtonClick(wxCommandEvent& evt) {
		if (processedImageFile->isEmpty()) {
			evt.Skip();
			return;
		}

		this->processedImageFile->applyNegativeFilter();
		this->updateProcessedImageView();
		this->log("Negative filter applied");
		evt.Skip();
	}

	void Main::onHistogramEqualizationButtonClick(wxCommandEvent& evt) {
		if (processedImageFile->isEmpty()) {
			evt.Skip();
			return;
		}

		this->showImage("Previous Image", this->processedImageFile->get());
		this->createHistogram("Previous Histogram");
		this->processedImageFile->applyHistogramEqualizationFilter();
		this->createHistogram("Histogram");
		this->updateProcessedImageView();
		this->log("Histogram equalization filter applied");
		evt.Skip();
	}

	void Main::onHistogramMatchingButtonClick(wxCommandEvent& evt) {
		if (processedImageFile->isEmpty()) {
			evt.Skip();
			return;
		}

		if (fileDialog->ShowModal() != wxID_CANCEL) {
			this->loadImageFromDialog(this->targetImageFile);
			if (!this->targetImageFile->isEmpty()) {
				this->processedImageFile->applyGrayScaleFilter();
				this->targetImageFile->applyGrayScaleFilter();
				this->processedImageFile->applyHistogramMatchingFilter(this->targetImageFile);

				this->updateProcessedImageView();
				this->updateTargetImageView();
				this->log("Histogram matching filter applied");
			}
			else
			{
				this->log("Error loading image");
			}
		}

		evt.Skip();
	}

	void Main::log(string log)
	{
		this->logListBox->AppendString(log);
		this->logListBox->SetSelection(logListBox->GetCount() - 1);
	}

	void Main::showImage(string viewName, Mat image)
	{
		if (image.empty()) return;
		imshow(viewName, image);
	}

	void Main::updateOriginalImageView() {
		if (this->originalImageFile->isEmpty()) return;

		const Mat image = this->originalImageFile->get();
		this->showImage("Original Image", image);

		const int width = image.size().width;
		const int height = image.size().height;
		this->imageDimTextCtrl->Clear();
		this->imageDimTextCtrl->WriteText(to_string(width) + "x" + to_string(height));
	}

	void Main::updateProcessedImageView()
	{
		if (this->processedImageFile->isEmpty()) return;

		this->showImage("Processed Image", this->processedImageFile->get());
	}

	void Main::updateTargetImageView()
	{
		if (this->targetImageFile->isEmpty()) return;

		this->showImage("Target Image", this->targetImageFile->get());
	}

	void Main::loadImageFromDialog(Image* image)
	{
		const std::string image_path = fileDialog->GetPath().ToStdString();
		image->load(image_path);
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
			this->brightnessSlider->Disable();
			this->brightnessApplyButton->Disable();
			this->contrastSlider->Disable();
			this->contrastApplyButton->Disable();
			this->negativeButton->Disable();
			this->histogramEqualizationButton->Disable();
			this->histogramMatchingButton->Disable();
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
			this->brightnessSlider->Enable();
			this->brightnessApplyButton->Enable();
			this->contrastSlider->Enable();
			this->contrastApplyButton->Enable();
			this->negativeButton->Enable();
			this->histogramEqualizationButton->Enable();
			this->histogramMatchingButton->Enable();
		}
	}

	void Main::createHistogram(string viewName)
	{
		int histogram[256];
		const int maxHistogramValue = this->processedImageFile->calculateHistogram(histogram);
		
		// draw the histograms
		int hist_w = 256; 
		int hist_h = 256;
		int bin_w = cvRound((double)hist_w / 256);

		Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(255, 255, 255));


		// normalize the histogram between 0 and histImage.rows
		this->processedImageFile->normalizeHistogram(histImage.rows, histogram, maxHistogramValue);

		// draw the intensity line for histogram
		for (int i = 0; i < 256; i++)
		{
			line(histImage, Point(bin_w * (i), hist_h), Point(bin_w * (i), hist_h - histogram[i]), Scalar(0, 0, 0), 1, 8, 0);
		}

		// display histogram
		imshow(viewName, histImage);
	}

	void Main::cleanBrightnessData() 
	{
		this->brightnessSlider->SetValue(0);
		this->brightnessTextCtrl->Clear();
		this->brightnessTextCtrl->SetValue("0");
	}

	void Main::cleanContrastData()
	{
		this->contrastSlider->SetValue(0);
		this->contrastTextCtrl->Clear();
		this->contrastTextCtrl->SetValue("0");
	}

	string Main::convertNumberToString(int value)
	{
		return  value <= 0 ? to_string(value) : "+" + to_string(value);
	}
}