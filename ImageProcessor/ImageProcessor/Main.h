#pragma once

#include <opencv2/core/utility.hpp>
#include "wx/wx.h"
#include "Image.h"
#include "FilenameUtil.h"
#include "cvui.h"
#include <iostream>  
#include<string>  
#include <sstream> 
#include <opencv2/opencv.hpp>
#include "cvui.h" 
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace type;
using namespace util;

namespace view 
{
	class Main : public wxFrame
	{
	public:
		Main();
		~Main();
	private:
		vector<wxWindowBase*> componentsWithImageDependencie;
		wxButton* openButton = nullptr;
		wxButton* saveButton = nullptr;
		wxButton* flipHButton = nullptr;
		wxButton* flipVButton = nullptr;
		wxButton* grayButton = nullptr;
		wxButton* quantizeButton = nullptr;
		wxButton* resetButton = nullptr;
		wxButton* showImagesButton = nullptr;
		wxButton* histogramButton = nullptr;
		wxListBox* logListBox = nullptr;
		wxSlider* brightnessSlider = nullptr;
		wxStaticBox* brightnessStaticBox = nullptr;
		wxTextCtrl* brightnessTextCtrl = nullptr;
		wxButton* brightnessApplyButton = nullptr;
		wxSlider* contrastSlider = nullptr;
		wxStaticBox* contrastStaticBox = nullptr;
		wxTextCtrl* contrastTextCtrl = nullptr;
		wxButton* contrastApplyButton = nullptr;
		wxButton* negativeButton = nullptr;
		wxButton* histogramEqualizationButton = nullptr;
		wxButton* histogramMatchingButton = nullptr;
		wxSlider* zoomOutXSlider = nullptr;
		wxSlider* zoomOutYSlider = nullptr;
		wxStaticBox* zoomOutStaticBox = nullptr;
		wxTextCtrl* zoomOutXTextCtrl = nullptr;
		wxTextCtrl* zoomOutYTextCtrl = nullptr;
		wxButton* zoomOutApplyButton = nullptr;
		wxTextCtrl* imageDimTextCtrl = nullptr;
		wxButton* zoomInButton = nullptr;
		wxButton* rotateRightButton = nullptr;
		wxButton* rotateLeftButton = nullptr;
		wxButton* convolutionButton = nullptr;
		wxSingleChoiceDialog* quantizeValueDialog = nullptr;
		wxFileDialog* fileDialog = nullptr;
		wxDirDialog* dirDialog = nullptr;
		Image* originalImageFile = nullptr;
		Image* processedImageFile = nullptr;
		Image* targetImageFile = nullptr;
		FilenameUtil* filenameUtil = nullptr;
		
		void onFrameClosed(wxCloseEvent& evt);
		void onOpenButtonClick(wxCommandEvent& evt);
		void onSaveButtonClick(wxCommandEvent& evt);
		void onFlipHButtonClick(wxCommandEvent& evt);
		void onFlipVButtonClick(wxCommandEvent& evt);
		void onGrayVButtonClick(wxCommandEvent& evt);
		void onQuantizeButtonClick(wxCommandEvent& evt);
		void onResetButtonClick(wxCommandEvent& evt);
		void onShowImagesButtonClick(wxCommandEvent& evt);
		void onHistogramButtonClick(wxCommandEvent& evt);
		void onBrightnessSliderChange(wxCommandEvent& evt);
		void onBrightnessApplyButtonClick(wxCommandEvent& evt);
		void onContrastSliderChange(wxCommandEvent& evt);
		void onContrastApplyButtonClick(wxCommandEvent& evt);
		void onNegativeButtonClick(wxCommandEvent& evt);
		void onHistogramEqualizationButtonClick(wxCommandEvent& evt);
		void onHistogramMatchingButtonClick(wxCommandEvent& evt);
		void onZoomOutXSliderChange(wxCommandEvent& evt);
		void onZoomOutYSliderChange(wxCommandEvent& evt);
		void onZoomOutApplyButtonClick(wxCommandEvent& evt);
		void onZoomInButtonClick(wxCommandEvent& evt);
		void onRotateRightButtonClick(wxCommandEvent& evt);
		void onRotateLeftButtonClick(wxCommandEvent& evt);
		void onConvolutionButtonClick(wxCommandEvent& evt);

		void log(string log);
		void showImage(string viewName, Mat image);
		void updateOriginalImageView();
		void updateProcessedImageView();
		void updateTargetImageView();
		void loadImageFromDialog(Image* image);
		void updateImageDependentComponents();
		void createHistogram(string viewName);
		void cleanBrightnessData();
		void cleanContrastData();
		void cleanZoomOutData();
		string convertNumberToString(int value, string symbol = "");

		wxDECLARE_EVENT_TABLE();
	};
}