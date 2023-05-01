#include "MemoryFunctions.h"
#include <string>
#include "Addresses.h"
#include <iostream>

namespace MemoryFunctions
{
	HANDLE p_handle = nullptr;
	std::string menu_top_bar[2] = { "~b~MENU~n~", "~r~MENU~n~" };

	void SetProcessHandle(HANDLE handle)
	{
		p_handle = handle;
	}

	void DisplayIngameAlert(const std::string& text)
	{
		static int alert_count = 0; // Keep track of the number of alerts displayed
		if (alert_count > 1)
			alert_count = 0;
		std::string unique_text = menu_top_bar[alert_count] + text;
		std::cout << unique_text << "\n";
		const char* alert_text = unique_text.c_str();
		size_t size = strlen(alert_text) + 1;
		char buffer[150];
		strncpy_s(buffer, alert_text, size);
		SetValue(Addresses::text_box, buffer, size);
		alert_count++;
	}
}