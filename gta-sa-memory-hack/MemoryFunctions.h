#pragma once
#include <Windows.h>
#include <string>

namespace MemoryFunctions
{
	extern HANDLE p_handle;

	/*
	* Text that gets added to top of in-game alert display.
	* When DisplayIngameAlert is called, it alternates between index 0 and 1.
	* This is done because if the same text is sent to the in-game text box,
	* only the 1st occurrance actually gets displayed.
	*/
	extern std::string menu_top_bar[2];

	// Set p_handle
	void SetProcessHandle(HANDLE handle);

	// Set value in specific memory address
	template <typename T>
	void SetValue(const LPCVOID address, T& value)
	{
		WriteProcessMemory(p_handle, (LPVOID)address, &value, sizeof(value), nullptr);
	}

	// Set value by also specifying buffer size in specific memory address
	template <typename T>
	void SetValue(const LPCVOID address, T& value, std::size_t size)
	{
		WriteProcessMemory(p_handle, (LPVOID)address, &value, size, nullptr);
	}

	// Get value from specific memory address
	template <typename T>
	T GetValue(const LPCVOID address)
	{
		T t{};
		ReadProcessMemory(p_handle, (LPVOID)address, &t, sizeof(t), nullptr);
		return t;
	}

	// Display an alert in top left corner in-game
	// Note: Max length = 150 chars
	void DisplayIngameAlert(const std::string& text);
};