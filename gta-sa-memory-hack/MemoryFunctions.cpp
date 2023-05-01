#include "MemoryFunctions.h"
#include <string>
#include "Addresses.h"

namespace MemoryFunctions
{
	HANDLE p_handle = nullptr;
	std::string menu_top_bar[2] = { "-=-=-=-=-=-=-=-=-=-", "=-=-=-=-=-=-=-=-=-=" };

	void SetProcessHandle(HANDLE handle)
	{
		p_handle = handle;
	}

	void DisplayIngameAlert(const std::string& text)
	{
		static int alert_count = 0; // Keep track of the number of alerts displayed
		if (alert_count > 1)
			alert_count = 0;
		std::string unique_text = menu_top_bar[alert_count] + " " + text;
		const char* alert_text = unique_text.c_str();
		SetValue(Addresses::text_box, alert_text, strlen(alert_text) + 1);
		alert_count++;
	}
}