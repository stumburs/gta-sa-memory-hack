#include <Windows.h>
#include <iostream>
#include <string>
#include <functional>
#include <map>
#include "Addresses.h"
#include "PlayerFunctions.h"
#include "MemoryFunctions.h"

int timer = 0;

using InputFunction = std::function<void()>;
using InputMap = std::map<char, std::pair<bool, InputFunction>>;

InputMap CreateInputMap()
{
	InputMap input_map = {
		{'1', std::make_pair(false, PlayerFunctions::AddMoney)},
		{'2', std::make_pair(false, PlayerFunctions::SpeedUpGame)},
		{'3', std::make_pair(false, PlayerFunctions::ResetGameSpeed)}
	};
	return input_map;
}

void HandleInput(const InputMap& input_map)
{
	for (auto& input : input_map)
	{
		if (GetAsyncKeyState(input.first) & 0x8000)
		{
			if (!input.second.first)
			{
				const_cast<bool&>(input.second.first) = true;
				input.second.second();
			}
		}
		else
			const_cast<bool&>(input.second.first) = false;
	}
}

int main()
{
	bool running = true;

	// Find game window
	HWND hWnd = FindWindowA(0, "GTA: San Andreas");

	// Set process ID
	DWORD pID = 0;
	GetWindowThreadProcessId(hWnd, &pID);

	// Do something
	HANDLE p_handle;
	p_handle = OpenProcess(PROCESS_ALL_ACCESS, false, pID);
	MemoryFunctions::SetProcessHandle(p_handle);

	bool is_menu_open = false;
	bool is_alt_m_pressed = false;
	std::string default_menu_text = "Page 1:                 1 - $10000             2 - Speed up game    3 - Reset game speed  ALT + M to close";
	std::string current_menu_text = default_menu_text;

	InputMap input_map = CreateInputMap();

	while (running)
	{
		timer++;
		if (timer > 30)
			timer = 0;

		// Open/Close menu
		if ((GetAsyncKeyState(VK_MENU) & 0x8000) && (GetAsyncKeyState('M') & 0x8000))
		{
			if (!is_alt_m_pressed)
			{
				is_menu_open = !is_menu_open;
				is_alt_m_pressed = true;
			}
		}
		else
			is_alt_m_pressed = false;

		if (is_menu_open)
		{
			// Get inputs
			HandleInput(input_map);
		}

		std::cout << "Menu open: " << std::boolalpha << is_menu_open << "\n";

		if (is_menu_open && timer == 0)
		{
			MemoryFunctions::DisplayIngameAlert(current_menu_text);
			current_menu_text = default_menu_text;
		}

		Sleep(50);
	}

	return 0;
}