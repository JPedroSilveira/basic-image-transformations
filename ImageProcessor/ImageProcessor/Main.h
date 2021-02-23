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
	public:
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
		wxTextCtrl* imageDimTextCtrl = nullptr;
		wxSingleChoiceDialog* quantizeValueDialog = nullptr;
		wxFileDialog* fileDialog = nullptr;
		wxDirDialog* dirDialog = nullptr;
		Image* originalImageFile = nullptr;
		Image* processedImageFile = nullptr;
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

		void log(string log);
		void updateOriginalImageView();
		void updateProcessedImageView();
		void loadOriginalImageFromDialog();
		void loadProcessedImageFromOriginal();
		void updateImageDependentComponents();
		void createHistogram();

		wxDECLARE_EVENT_TABLE();
	};
}