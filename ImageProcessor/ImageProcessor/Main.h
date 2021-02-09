#pragma once

#include <opencv2/core/utility.hpp>
#include "wx/wx.h"
#include "Image.h"
#include "FilenameUtil.h"

using namespace type;
using namespace util;

namespace view {
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
		wxListBox* logListBox = nullptr;
		wxTextCtrl* imageDimTextCtrl = nullptr;
		wxFileDialog* fileDialog = nullptr;
		wxDirDialog* dirDialog = nullptr;
		Image* originalImageFile = nullptr;
		Image* processedImageFile = nullptr;
		FilenameUtil* filenameUtil = nullptr;

		void onOpenButtonClick(wxCommandEvent& evt);
		void onSaveButtonClick(wxCommandEvent& evt);
		void onFlipHButtonClick(wxCommandEvent& evt);
		void onFlipVButtonClick(wxCommandEvent& evt);

		void log(string log);
		void updateOriginalImageView();
		void updateProcessedImageView();
		void loadOriginalImageFromDialog();
		void loadProcessedImageFromOriginal();

		wxDECLARE_EVENT_TABLE();
	};
}