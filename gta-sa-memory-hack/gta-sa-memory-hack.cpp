#include <Windows.h>
#include <iostream>
#include <string>
#include <functional>
#include <map>

namespace Addresses
{
	LPCVOID money = reinterpret_cast<LPCVOID>(0x00B7CE50);
	LPCVOID game_speed = reinterpret_cast<LPCVOID>(0xB7CB64);
};

std::string menu_top_bar[] = { "-=-=-=-=-=-=-=-=-=-", "=-=-=-=-=-=-=-=-=-=" };
int timer = 0;

HANDLE p_handle;

using InputFunction = std::function<void()>;
using InputMap = std::map<char, std::pair<bool, InputFunction>>;

template <typename T>
void SetValue(LPCVOID address, T& value)
{
	WriteProcessMemory(p_handle, (LPVOID)address, &value, sizeof(value), nullptr);
}

template <typename T>
T GetValue(LPCVOID address)
{
	T t{};
	ReadProcessMemory(p_handle, (LPVOID)address, &t, sizeof(t), nullptr);
	return t;
}

void DisplayIngameAlert(const std::string& text)
{
	static int alert_count = 0; // Keep track of the number of alerts displayed
	if (alert_count > 1)
		alert_count = 0;
	std::string unique_text = menu_top_bar[alert_count] + " " + text;
	const char* alert_text = unique_text.c_str();
	WriteProcessMemory(p_handle, (LPVOID)0xBAA7A0, alert_text, strlen(alert_text) + 1, nullptr);
	alert_count++;
}

void AddMoney()
{
	DWORD current_money = GetValue<DWORD>(Addresses::money);
	DWORD new_money = current_money + 10000;
	SetValue(Addresses::money, new_money);
	//timer = 1;
	//DisplayIngameAlert("Added $10000");
	std::cout << "Money: " << GetValue<DWORD>(Addresses::money) << std::endl;
}

void SpeedUpGame()
{
	float value = GetValue<float>(Addresses::game_speed) + GetValue<float>(Addresses::game_speed) / 2;
	SetValue(Addresses::game_speed, value);
	std::cout << "Game Speed: " << value << std::endl;
}

void ResetGameSpeed()
{
	float value = 1.0f;
	SetValue(Addresses::game_speed, value);
	std::cout << "Game speed reset" << std::endl;
}

InputMap CreateInputMap()
{
	InputMap input_map = {
		{'1', std::make_pair(false, AddMoney)},
		{'2', std::make_pair(false, SpeedUpGame)},
		{'3', std::make_pair(false, ResetGameSpeed)}
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
	p_handle = OpenProcess(PROCESS_ALL_ACCESS, false, pID);


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
			DisplayIngameAlert(current_menu_text);
			current_menu_text = default_menu_text;
		}

		Sleep(50);
	}

	return 0;
}