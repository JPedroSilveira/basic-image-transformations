#pragma once

#include <opencv2/core/utility.hpp>
#include "wx/wx.h"
#include "Image.h"
#include "FilenameUtil.h"
#include "cvui.h"
#include <iostream>  
#include<string>  
#include <sstream> 

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
		wxListBox* logListBox = nullptr;
		wxTextCtrl* imageDimTextCtrl = nullptr;
		wxSingleChoiceDialog* quantizeValueDialog = nullptr;
		wxFileDialog* fileDialog = nullptr;
		wxDirDialog* dirDialog = nullptr;
		Image* originalImageFile = nullptr;
		Image* processedImageFile = nullptr;
		FilenameUtil* filenameUtil = nullptr;

		void onOpenButtonClick(wxCommandEvent& evt);
		void onSaveButtonClick(wxCommandEvent& evt);
		void onFlipHButtonClick(wxCommandEvent& evt);
		void onFlipVButtonClick(wxCommandEvent& evt);
		void onGrayVButtonClick(wxCommandEvent& evt);
		void onQuantizeButtonClick(wxCommandEvent& evt);
		void onResetButtonClick(wxCommandEvent& evt);

		void log(string log);
		void updateOriginalImageView();
		void updateProcessedImageView();
		void loadOriginalImageFromDialog();
		void loadProcessedImageFromOriginal();

		wxDECLARE_EVENT_TABLE();
	};
}