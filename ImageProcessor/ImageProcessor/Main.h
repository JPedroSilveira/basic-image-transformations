#pragma once

#include <opencv2/core/utility.hpp>
#include "wx/wx.h"
#include "ImageFile.h"
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
		wxListBox* logListBox = nullptr;
		wxFileDialog* fileDialog = nullptr;
		wxDirDialog* dirDialog = nullptr;
		ImageFile* originalImage = nullptr;
		FilenameUtil* filenameUtil = nullptr;

		void onOpenButtonClick(wxCommandEvent& evt);
		void onSaveButtonClick(wxCommandEvent& evt);

		wxDECLARE_EVENT_TABLE();
	};
}