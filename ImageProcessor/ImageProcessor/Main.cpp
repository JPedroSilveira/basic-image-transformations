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
		EVT_SLIDER(10017, onZoomOutXSliderChange)
		EVT_SLIDER(10018, onZoomOutYSliderChange)
		EVT_BUTTON(10019, onZoomOutApplyButtonClick)
		EVT_BUTTON(10020, onZoomInButtonClick)
		EVT_BUTTON(10021, onRotateRightButtonClick)
		EVT_BUTTON(10022, onRotateLeftButtonClick)
		EVT_BUTTON(10023, onConvolutionApplyButtonClick)
		EVT_BUTTON(10024, onConvolutionGaussianShortcutButton)
		EVT_BUTTON(10025, onConvolutionLaplacianShortcutButton)
		EVT_BUTTON(10026, onConvolutionHighPassShortcutButton)
		EVT_BUTTON(10027, onConvolutionPrewittHxShortcutButton)
		EVT_BUTTON(10028, onConvolutionPrewittHyShortcutButton)
		EVT_BUTTON(10029, onConvolutionSobelHxShortcutButton)
		EVT_BUTTON(10030, onConvolutionSobelHyShortcutButton)
	wxEND_EVENT_TABLE()

	Main::Main() : wxFrame(nullptr, 10000, "Image Processor", wxPoint(30,30), wxSize(1030,500))
	{
		//OPEN & SAVE
		this->openButton = new wxButton(this, 10001, "Open image", wxPoint(10, 10), wxSize(360, 50));
		this->saveButton = new wxButton(this, 10002, "Save image", wxPoint(410, 10), wxSize(360, 50));
		this->fileDialog = new wxFileDialog(this, "Select an image", "", "", "JPG files(*.jpg;*.jpeg) | *.jpg;*.jpeg", wxFD_OPEN);
		this->dirDialog = new wxDirDialog(this, "Select a directory", "");

		//FLIP
		this->flipHButton = new wxButton(this, 10003, "Flip Horizontally", wxPoint(410, 190), wxSize(175, 30));
		this->flipVButton = new wxButton(this, 10004, "Flip Vertically", wxPoint(595, 190), wxSize(175, 30));

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
		this->brightnessSlider = new wxSlider(this, 10010, 0, -255, 255, wxPoint(20, 170), wxSize(175, 30));
		this->brightnessTextCtrl = new wxTextCtrl(this, wxID_ANY, "", wxPoint(195, 172), wxSize(40, 20), wxTE_READONLY);
		this->brightnessApplyButton = new wxButton(this, 10011, "Apply", wxPoint(245, 168), wxSize(115, 30));

		//CONTRAST
		this->contrastStaticBox = new wxStaticBox(this, wxID_ANY, "Contrast", wxPoint(10, 220), wxSize(360, 60));
		this->contrastSlider = new wxSlider(this, 10012, 0, 0, 255, wxPoint(20, 240), wxSize(175, 30));
		this->contrastTextCtrl = new wxTextCtrl(this, wxID_ANY, "", wxPoint(195, 242), wxSize(40, 20), wxTE_READONLY);
		this->contrastApplyButton = new wxButton(this, 10013, "Apply", wxPoint(245, 238), wxSize(115, 30));

		//ZOOM OUT
		this->zoomOutStaticBox = new wxStaticBox(this, wxID_ANY, "Zoom out", wxPoint(10, 290), wxSize(360, 100));
		this->zoomOutXSlider = new wxSlider(this, 10017, 0, 1, 20, wxPoint(20, 310), wxSize(175, 30));
		this->zoomOutYSlider = new wxSlider(this, 10018, 0, 1, 20, wxPoint(20, 350), wxSize(175, 30));
		this->zoomOutXTextCtrl = new wxTextCtrl(this, wxID_ANY, "", wxPoint(195, 312), wxSize(40, 20), wxTE_READONLY);
		this->zoomOutYTextCtrl = new wxTextCtrl(this, wxID_ANY, "", wxPoint(195, 352), wxSize(40, 20), wxTE_READONLY);
		this->zoomOutApplyButton = new wxButton(this, 10019, "Apply", wxPoint(245, 328), wxSize(115, 30));

		//ZOOM IN
		this->zoomInButton = new wxButton(this, 10020, "Zoom In", wxPoint(410, 230), wxSize(175, 30));

		//NEGATIVE
		this->negativeButton = new wxButton(this, 10014, "Negative", wxPoint(595, 110), wxSize(175, 30));

		//HISTOGRAM EQUALIZATION
		this->histogramEqualizationButton = new wxButton(this, 10015, "Histogram Equalization", wxPoint(410, 150), wxSize(175, 30));

		//HISTOGRAM MATCHING
		this->histogramMatchingButton = new wxButton(this, 10016, "Histogram Matching", wxPoint(595, 150), wxSize(175, 30));

		//ROTATE
		this->rotateLeftButton = new wxButton(this, 10022, "Rotate Left", wxPoint(410, 270), wxSize(175, 30));
		this->rotateRightButton = new wxButton(this, 10021, "Rotate Right", wxPoint(595, 270), wxSize(175, 30));

		//CONVOLUTION
		this->convolutionApplyButton = new wxButton(this, 10023, "Apply", wxPoint(875, 350), wxSize(115, 30));
		this->convolutionStaticBox = new wxStaticBox(this, wxID_ANY, "Convolution", wxPoint(410, 310), wxSize(590, 100));
		this->convolutionTable00TextCtrl = new wxTextCtrl(this, wxID_ANY, "", wxPoint(430, 330), wxSize(45, 20));
		this->convolutionTable01TextCtrl = new wxTextCtrl(this, wxID_ANY, "", wxPoint(430, 355), wxSize(45, 20));
		this->convolutionTable02TextCtrl = new wxTextCtrl(this, wxID_ANY, "", wxPoint(430, 380), wxSize(45, 20));
		this->convolutionTable10TextCtrl = new wxTextCtrl(this, wxID_ANY, "", wxPoint(480, 330), wxSize(45, 20));
		this->convolutionTable11TextCtrl = new wxTextCtrl(this, wxID_ANY, "", wxPoint(480, 355), wxSize(45, 20));
		this->convolutionTable12TextCtrl = new wxTextCtrl(this, wxID_ANY, "", wxPoint(480, 380), wxSize(45, 20));
		this->convolutionTable20TextCtrl = new wxTextCtrl(this, wxID_ANY, "", wxPoint(530, 330), wxSize(45, 20));
		this->convolutionTable21TextCtrl = new wxTextCtrl(this, wxID_ANY, "", wxPoint(530, 355), wxSize(45, 20));
		this->convolutionTable22TextCtrl = new wxTextCtrl(this, wxID_ANY, "", wxPoint(530, 380), wxSize(45, 20));
		this->convolutionGaussianShortcutButton = new wxButton(this, 10024, "Gaussian", wxPoint(595, 330), wxSize(80, 20));
		this->convolutionLaplacianShortcutButton = new wxButton(this, 10025, "Laplacian", wxPoint(595, 355), wxSize(80, 20));
		this->convolutionHighPassShortcutButton = new wxButton(this, 10026, "High Pass", wxPoint(595, 380), wxSize(80, 20));
		this->convolutionPrewittHxShortcutButton = new wxButton(this, 10027, "Prewitt Hx", wxPoint(680, 330), wxSize(80, 20));
		this->convolutionPrewittHyShortcutButton = new wxButton(this, 10028, "Prewitt Hy", wxPoint(680, 355), wxSize(80, 20));
		this->convolutionSobelHxShortcutButton = new wxButton(this, 10029, "Sobel Hx", wxPoint(680, 380), wxSize(80, 20));
		this->convolutionSobelHyShortcutButton = new wxButton(this, 10030, "Sobel Hy", wxPoint(765, 330), wxSize(80, 20));


		//INFO
		this->imageDimTextCtrl = new wxTextCtrl(this, wxID_ANY, "", wxPoint(10, 430), wxSize(300, 20), wxTE_READONLY);
		this->logListBox = new wxListBox(this, wxID_ANY, wxPoint(800, 10), wxSize(200, 290));

		//IMAGES
		this->originalImageFile = new Image();
		this->processedImageFile = new Image();
		this->targetImageFile = new Image();

		//UTILS
		this->filenameUtil = new FilenameUtil();

		//COMPONENTS WITHOUT ENABLE OPTION
		this->brightnessTextCtrl->Disable();
		this->contrastTextCtrl->Disable();
		this->imageDimTextCtrl->Disable();
		this->zoomOutXTextCtrl->Disable();
		this->zoomOutYTextCtrl->Disable();

		//COMPONENTS THAT NEEDS AN IMAGE TO BE ENABLED
		this->componentsWithImageDependencie.push_back(this->saveButton);
		this->componentsWithImageDependencie.push_back(this->flipHButton);
		this->componentsWithImageDependencie.push_back(this->flipVButton);
		this->componentsWithImageDependencie.push_back(this->grayButton);
		this->componentsWithImageDependencie.push_back(this->quantizeButton);
		this->componentsWithImageDependencie.push_back(this->resetButton);
		this->componentsWithImageDependencie.push_back(this->showImagesButton);
		this->componentsWithImageDependencie.push_back(this->histogramButton);
		this->componentsWithImageDependencie.push_back(this->brightnessSlider);
		this->componentsWithImageDependencie.push_back(this->brightnessApplyButton);
		this->componentsWithImageDependencie.push_back(this->contrastSlider);
		this->componentsWithImageDependencie.push_back(this->contrastApplyButton);
		this->componentsWithImageDependencie.push_back(this->negativeButton);
		this->componentsWithImageDependencie.push_back(this->histogramEqualizationButton);
		this->componentsWithImageDependencie.push_back(this->histogramMatchingButton);
		this->componentsWithImageDependencie.push_back(this->zoomOutXSlider);
		this->componentsWithImageDependencie.push_back(this->zoomOutYSlider);
		this->componentsWithImageDependencie.push_back(this->zoomOutApplyButton);
		this->componentsWithImageDependencie.push_back(this->zoomInButton);
		this->componentsWithImageDependencie.push_back(this->rotateRightButton);
		this->componentsWithImageDependencie.push_back(this->rotateLeftButton);
		this->componentsWithImageDependencie.push_back(this->convolutionApplyButton);
		this->componentsWithImageDependencie.push_back(this->convolutionTable00TextCtrl);
		this->componentsWithImageDependencie.push_back(this->convolutionTable01TextCtrl);
		this->componentsWithImageDependencie.push_back(this->convolutionTable02TextCtrl);
		this->componentsWithImageDependencie.push_back(this->convolutionTable10TextCtrl);
		this->componentsWithImageDependencie.push_back(this->convolutionTable11TextCtrl);
		this->componentsWithImageDependencie.push_back(this->convolutionTable12TextCtrl);
		this->componentsWithImageDependencie.push_back(this->convolutionTable20TextCtrl);
		this->componentsWithImageDependencie.push_back(this->convolutionTable21TextCtrl);
		this->componentsWithImageDependencie.push_back(this->convolutionTable22TextCtrl);
		this->componentsWithImageDependencie.push_back(this->convolutionGaussianShortcutButton);
		this->componentsWithImageDependencie.push_back(this->convolutionLaplacianShortcutButton);
		this->componentsWithImageDependencie.push_back(this->convolutionHighPassShortcutButton);
		this->componentsWithImageDependencie.push_back(this->convolutionPrewittHxShortcutButton);
		this->componentsWithImageDependencie.push_back(this->convolutionPrewittHyShortcutButton);
		this->componentsWithImageDependencie.push_back(this->convolutionSobelHxShortcutButton);
		this->componentsWithImageDependencie.push_back(this->convolutionSobelHyShortcutButton);
		this->updateImageDependentComponents();

		// INIT COMPONENTS DATA
		this->cleanBrightnessData();
		this->cleanContrastData();
		this->cleanZoomOutData();
		this->cleanConvolutionTableData();
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

		this->logListBox->Clear();
		this->processedImageFile->set(originalImageFile->get());
		this->updateProcessedImageView();
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
		this->brightnessTextCtrl->WriteText(this->convertNumberToString(value, "+"));
		evt.Skip();
	}

	void Main::onBrightnessApplyButtonClick(wxCommandEvent& evt) 
	{
		if (processedImageFile->isEmpty()) {
			evt.Skip();
			return;
		}

		const int value = this->brightnessSlider->GetValue();
		this->processedImageFile->applyBrightness(value);
		this->cleanBrightnessData();
		this->updateProcessedImageView();
		this->log("Brightness of " + this->convertNumberToString(value, "+") + " applied");
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
		this->contrastTextCtrl->WriteText(this->convertNumberToString(value, "+"));
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
		this->log("Contrast of " + this->convertNumberToString(value, "+") + " applied");
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

	void Main::onZoomOutXSliderChange(wxCommandEvent& evt) 
	{
		if (processedImageFile->isEmpty()) {
			evt.Skip();
			return;
		}

		const int value = this->zoomOutXSlider->GetValue();
		this->zoomOutXTextCtrl->Clear();
		this->zoomOutXTextCtrl->WriteText(this->convertNumberToString(value, "x/"));
		evt.Skip();
	}

	void Main::onZoomOutYSliderChange(wxCommandEvent& evt)
	{
		if (processedImageFile->isEmpty()) {
			evt.Skip();
			return;
		}

		const int value = this->zoomOutYSlider->GetValue();
		this->zoomOutYTextCtrl->Clear();
		this->zoomOutYTextCtrl->WriteText(this->convertNumberToString(value, "y/"));
		evt.Skip();
	}

	void Main::onZoomOutApplyButtonClick(wxCommandEvent& evt)
	{
		if (processedImageFile->isEmpty()) {
			evt.Skip();
			return;
		}

		const int x = this->zoomOutXSlider->GetValue();
		const int y = this->zoomOutYSlider->GetValue();
		this->processedImageFile->applyZoomOut(x,y);
		this->cleanZoomOutData();
		this->updateProcessedImageView();
		this->log("Zoom Out of x: " + this->convertNumberToString(x) + " and y: " + this->convertNumberToString(y) + " applied");
		evt.Skip();
	}

	void Main::onZoomInButtonClick(wxCommandEvent& evt)
	{
		if (processedImageFile->isEmpty()) {
			evt.Skip();
			return;
		}

		this->processedImageFile->applyZoomIn();
		this->updateProcessedImageView();
		this->log("Zoom in (x2) applied");
		evt.Skip();
	}

	void Main::onRotateRightButtonClick(wxCommandEvent& evt)
	{
		if (processedImageFile->isEmpty()) {
			evt.Skip();
			return;
		}

		this->processedImageFile->rotateRight();
		this->updateProcessedImageView();
		this->log("90 degrees right rotation applied");
		evt.Skip();
	}

	void Main::onRotateLeftButtonClick(wxCommandEvent& evt)
	{
		if (processedImageFile->isEmpty()) {
			evt.Skip();
			return;
		}

		this->processedImageFile->rotateLeft();
		this->updateProcessedImageView();
		this->log("90 degrees left rotation applied");
		evt.Skip();
	}

	void Main::onConvolutionApplyButtonClick(wxCommandEvent& evt)
	{
		if (processedImageFile->isEmpty()) {
			evt.Skip();
			return;
		}

		try {
			float filter[3][3];
			filter[0][0] = stof(this->convolutionTable00TextCtrl->GetValue().ToStdString());
			filter[0][1] = stof(this->convolutionTable01TextCtrl->GetValue().ToStdString());
			filter[0][2] = stof(this->convolutionTable02TextCtrl->GetValue().ToStdString());
			filter[1][0] = stof(this->convolutionTable10TextCtrl->GetValue().ToStdString());
			filter[1][1] = stof(this->convolutionTable11TextCtrl->GetValue().ToStdString());
			filter[1][2] = stof(this->convolutionTable12TextCtrl->GetValue().ToStdString());
			filter[2][0] = stof(this->convolutionTable20TextCtrl->GetValue().ToStdString());
			filter[2][1] = stof(this->convolutionTable21TextCtrl->GetValue().ToStdString());
			filter[2][2] = stof(this->convolutionTable22TextCtrl->GetValue().ToStdString());

			this->processedImageFile->applyThreeByThreeMatrixConvolution(filter);
			this->updateProcessedImageView();
			this->log("Convolution applied");
		}
		catch(...)
		{
			this->cleanConvolutionTableData();
			this->log("Error: Invalid convolution values");
		}

		evt.Skip();
	}

	void Main::onConvolutionGaussianShortcutButton(wxCommandEvent& evt)
	{
		if (processedImageFile->isEmpty()) {
			evt.Skip();
			return;
		}
		this->convolutionTable00TextCtrl->SetValue("0.0625");
		this->convolutionTable01TextCtrl->SetValue("0.125");
		this->convolutionTable02TextCtrl->SetValue("0.0625");
		this->convolutionTable10TextCtrl->SetValue("0.125");
		this->convolutionTable11TextCtrl->SetValue("0.25");
		this->convolutionTable12TextCtrl->SetValue("0.125");
		this->convolutionTable20TextCtrl->SetValue("0.0625");
		this->convolutionTable21TextCtrl->SetValue("0.125");
		this->convolutionTable22TextCtrl->SetValue("0.0625");
		evt.Skip();
	}

	void Main::onConvolutionLaplacianShortcutButton(wxCommandEvent& evt)
	{
		if (processedImageFile->isEmpty()) {
			evt.Skip();
			return;
		}
		this->convolutionTable00TextCtrl->SetValue("0");
		this->convolutionTable01TextCtrl->SetValue("-1");
		this->convolutionTable02TextCtrl->SetValue("0");
		this->convolutionTable10TextCtrl->SetValue("-1");
		this->convolutionTable11TextCtrl->SetValue("4");
		this->convolutionTable12TextCtrl->SetValue("-1");
		this->convolutionTable20TextCtrl->SetValue("0");
		this->convolutionTable21TextCtrl->SetValue("-1");
		this->convolutionTable22TextCtrl->SetValue("0");
		evt.Skip();
	}

	void Main::onConvolutionHighPassShortcutButton(wxCommandEvent& evt)
	{
		if (processedImageFile->isEmpty()) {
			evt.Skip();
			return;
		}
		this->convolutionTable00TextCtrl->SetValue("-1");
		this->convolutionTable01TextCtrl->SetValue("-1");
		this->convolutionTable02TextCtrl->SetValue("-1");
		this->convolutionTable10TextCtrl->SetValue("-1");
		this->convolutionTable11TextCtrl->SetValue("8");
		this->convolutionTable12TextCtrl->SetValue("-1");
		this->convolutionTable20TextCtrl->SetValue("-1");
		this->convolutionTable21TextCtrl->SetValue("-1");
		this->convolutionTable22TextCtrl->SetValue("-1");
		evt.Skip();
	}

	void Main::onConvolutionPrewittHxShortcutButton(wxCommandEvent& evt)
	{
		if (processedImageFile->isEmpty()) {
			evt.Skip();
			return;
		}
		this->convolutionTable00TextCtrl->SetValue("-1");
		this->convolutionTable01TextCtrl->SetValue("-1");
		this->convolutionTable02TextCtrl->SetValue("-1");
		this->convolutionTable10TextCtrl->SetValue("0");
		this->convolutionTable11TextCtrl->SetValue("0");
		this->convolutionTable12TextCtrl->SetValue("0");
		this->convolutionTable20TextCtrl->SetValue("1");
		this->convolutionTable21TextCtrl->SetValue("1");
		this->convolutionTable22TextCtrl->SetValue("1");
		evt.Skip();
	}

	void Main::onConvolutionPrewittHyShortcutButton(wxCommandEvent& evt)
	{
		if (processedImageFile->isEmpty()) {
			evt.Skip();
			return;
		}
		this->convolutionTable00TextCtrl->SetValue("-1");
		this->convolutionTable01TextCtrl->SetValue("0");
		this->convolutionTable02TextCtrl->SetValue("1");
		this->convolutionTable10TextCtrl->SetValue("-1");
		this->convolutionTable11TextCtrl->SetValue("0");
		this->convolutionTable12TextCtrl->SetValue("1");
		this->convolutionTable20TextCtrl->SetValue("-1");
		this->convolutionTable21TextCtrl->SetValue("0");
		this->convolutionTable22TextCtrl->SetValue("1");
		evt.Skip();
	}

	void Main::onConvolutionSobelHxShortcutButton(wxCommandEvent& evt)
	{
		if (processedImageFile->isEmpty()) {
			evt.Skip();
			return;
		}
		this->convolutionTable00TextCtrl->SetValue("-1");
		this->convolutionTable01TextCtrl->SetValue("-2");
		this->convolutionTable02TextCtrl->SetValue("-1");
		this->convolutionTable10TextCtrl->SetValue("0");
		this->convolutionTable11TextCtrl->SetValue("0");
		this->convolutionTable12TextCtrl->SetValue("0");
		this->convolutionTable20TextCtrl->SetValue("1");
		this->convolutionTable21TextCtrl->SetValue("2");
		this->convolutionTable22TextCtrl->SetValue("1");
		evt.Skip();
	}

	void Main::onConvolutionSobelHyShortcutButton(wxCommandEvent& evt)
	{
		if (processedImageFile->isEmpty()) {
			evt.Skip();
			return;
		}
		this->convolutionTable00TextCtrl->SetValue("-1");
		this->convolutionTable01TextCtrl->SetValue("0");
		this->convolutionTable02TextCtrl->SetValue("1");
		this->convolutionTable10TextCtrl->SetValue("-2");
		this->convolutionTable11TextCtrl->SetValue("0");
		this->convolutionTable12TextCtrl->SetValue("2");
		this->convolutionTable20TextCtrl->SetValue("-1");
		this->convolutionTable21TextCtrl->SetValue("0");
		this->convolutionTable22TextCtrl->SetValue("1");
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
			for (int i = 0; i < this->componentsWithImageDependencie.size(); i++)
			{
				this->componentsWithImageDependencie[i]->Disable();
			}
		} 
		else
		{
			for (int i = 0; i < this->componentsWithImageDependencie.size(); i++)
			{
				this->componentsWithImageDependencie[i]->Enable();
			}
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

	void Main::cleanZoomOutData()
	{
		this->zoomOutXSlider->SetValue(1);
		this->zoomOutYSlider->SetValue(1);
		this->zoomOutXTextCtrl->Clear();
		this->zoomOutXTextCtrl->SetValue("x/1");
		this->zoomOutYTextCtrl->Clear();
		this->zoomOutYTextCtrl->SetValue("y/1");
	}

	void Main::cleanConvolutionTableData()
	{
		this->convolutionTable00TextCtrl->Clear();
		this->convolutionTable00TextCtrl->SetValue("0");
		this->convolutionTable01TextCtrl->Clear();
		this->convolutionTable01TextCtrl->SetValue("0");
		this->convolutionTable02TextCtrl->Clear();
		this->convolutionTable02TextCtrl->SetValue("0");
		this->convolutionTable10TextCtrl->Clear();
		this->convolutionTable10TextCtrl->SetValue("0");
		this->convolutionTable11TextCtrl->Clear();
		this->convolutionTable11TextCtrl->SetValue("0");
		this->convolutionTable12TextCtrl->Clear();
		this->convolutionTable12TextCtrl->SetValue("0");
		this->convolutionTable20TextCtrl->Clear();
		this->convolutionTable20TextCtrl->SetValue("0");
		this->convolutionTable21TextCtrl->Clear();
		this->convolutionTable21TextCtrl->SetValue("0");
		this->convolutionTable22TextCtrl->Clear();
		this->convolutionTable22TextCtrl->SetValue("0");
	}

	string Main::convertNumberToString(int value, string symbol)
	{
		return  value <= 0 ? to_string(value) : symbol + to_string(value);
	}
}