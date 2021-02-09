#include "FilenameUtil.h"

namespace util 
{
	string FilenameUtil::generateAutoJPGName() 
	{
		const auto now = std::chrono::system_clock::now();
		const std::time_t date = std::chrono::system_clock::to_time_t(now);
		string stringDate = std::ctime(&date);
		replace(stringDate.begin(), stringDate.end(), ':', '-');
		replace(stringDate.begin(), stringDate.end(), ' ', '-');
		stringDate.erase(remove(stringDate.begin(), stringDate.end(), '\n'), stringDate.end());
		transform(stringDate.begin(), stringDate.end(), stringDate.begin(), [](unsigned char c) { return std::tolower(c); });
		return "\\processed-image-" + stringDate + ".jpg";
	}
}