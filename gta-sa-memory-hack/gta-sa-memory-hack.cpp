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
using InputMap = std::map<char, std::pair<int, InputFunction>>;


enum MenuScreens
{
	FIRST = 0,
	PLAYER,
	WORLD
};

int menu = MenuScreens::FIRST;

void IncreaseMenuToPlayer()
{
	menu = MenuScreens::PLAYER;
}

void IncreaseMenuToWorld()
{
	menu = MenuScreens::WORLD;
}

void HandleInput(const InputMap& input_map)
{
	for (auto& input : input_map)
		if (GetAsyncKeyState(input.first) & 0x8000)
		{
			if (!input.second.first)
			{
				const_cast<int&>(input.second.first) = true;
				input.second.second();
			}
		}
		else
			const_cast<int&>(input.second.first) = false;
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
	std::string menu_first_screen = "~g~P~s~ - Player~n~~g~W~s~ - World~n~~g~ALT ~s~+ ~g~M ~s~to close";
	std::string menu_player_screen = "Player~n~~g~1~s~ - $10000~n~~g~ALT ~s~+ ~g~M ~s~to close";
	std::string menu_world_screen = "World~n~~g~1~s~ - Speed up game~n~~g~2~s~ - Reset game speed~n~~g~ALT ~s~+ ~g~M ~s~to close";
	//std::string default_menu_text = "~g~1~s~ - $10000~n~~g~2~s~ - Speed up game~n~~g~3~s~ - Reset game speed~n~~g~4~s~ - Hesoyam~n~~g~ALT ~s~+ ~g~M ~s~to close";
	std::string current_menu_text = menu_first_screen;


	/*
	* CURRENT IMPLEMENTATION OF EVERYTHING HERE IS ABSOLUTELY AWFUL
	*/
	InputMap input_map_first = {
		{'P', std::make_pair(0, IncreaseMenuToPlayer)},
		{'W', std::make_pair(0, IncreaseMenuToWorld)},
	};

	InputMap input_map_player = {
		{'1', std::make_pair(0, PlayerFunctions::AddMoney)},
	};

	InputMap input_map_world = {
		{'1', std::make_pair(0, PlayerFunctions::SpeedUpGame)},
		{'2', std::make_pair(0, PlayerFunctions::ResetGameSpeed)},
	};

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
			switch (menu)
			{
			case MenuScreens::FIRST:
				HandleInput(input_map_first);
				break;
			case MenuScreens::PLAYER:
				HandleInput(input_map_player);
				break;
			case MenuScreens::WORLD:
				HandleInput(input_map_world);
				break;
			default:
				break;
			}
		}
		else
			menu = MenuScreens::FIRST;

		std::cout << "Menu open: " << std::boolalpha << is_menu_open << "\n";

		if (is_menu_open && timer == 0)
		{
			switch (menu)
			{
			case MenuScreens::FIRST:
				current_menu_text = menu_first_screen;
				break;
			case MenuScreens::PLAYER:
				current_menu_text = menu_player_screen;
				break;
			case MenuScreens::WORLD:
				current_menu_text = menu_world_screen;
				break;
			default:
				break;
			}
			MemoryFunctions::DisplayIngameAlert(current_menu_text);
		}

		Sleep(50);
	}

	return 0;
}