#include <Windows.h>
#include <iostream>
#include <string>
#include <functional>

namespace Addresses
{
	LPCVOID money = reinterpret_cast<LPCVOID>(0x00B7CE50);
	LPCVOID game_speed = reinterpret_cast<LPCVOID>(0xB7CB64);
};

std::string menu_top_bar[] = { "-=-=-=-=-=-=-=-=-=-", "=-=-=-=-=-=-=-=-=-=" };

HANDLE p_handle;

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

int main()
{
	bool running = true;
	int timer = 0;

	// Find game window
	HWND hWnd = FindWindowA(0, "GTA: San Andreas");

	// Set process ID
	DWORD pID = 0;
	GetWindowThreadProcessId(hWnd, &pID);

	// Do something
	p_handle = OpenProcess(PROCESS_ALL_ACCESS, false, pID);

	bool is_1_pressed = false;
	bool is_2_pressed = false;
	bool is_3_pressed = false;

	bool is_menu_open = false;
	bool is_alt_m_pressed = false;

	std::string default_menu_text = "Page 1:                 1 - $10000             2 - Speed up game    3 - Reset game speed  ALT + M to close";
	std::string current_menu_text = default_menu_text;

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
			// Add money
			if (GetAsyncKeyState('1') & 0x8000)
			{
				if (!is_1_pressed)
				{
					is_1_pressed = true;
					DWORD current_money = GetValue<DWORD>(Addresses::money);
					DWORD new_money = current_money + 10000;
					SetValue(Addresses::money, new_money);
					timer = 1;
					current_menu_text = "Added $10000";
					DisplayIngameAlert(current_menu_text);
					std::cout << "Money: " << GetValue<DWORD>(Addresses::money) << std::endl;
				}
			}
			else
				is_1_pressed = false;

			// Speed up game
			if (GetAsyncKeyState('2') & 0x8000)
			{
				if (!is_2_pressed)
				{
					is_2_pressed = true;
					float value = GetValue<float>(Addresses::game_speed) + GetValue<float>(Addresses::game_speed) / 2;
					SetValue(Addresses::game_speed, value);
					current_menu_text = "Game speed: " + std::to_string(value);
					DisplayIngameAlert(current_menu_text);
					std::cout << "Game Speed: " << value << std::endl;
				}
			}
			else
				is_2_pressed = false;

			// Reset game speed
			if (GetAsyncKeyState('3') & 0x8000)
			{
				if (!is_3_pressed)
				{
					is_3_pressed = true;
					float value = 1.0f;
					SetValue(Addresses::game_speed, value);
					current_menu_text = "Game speed reset";
					DisplayIngameAlert(current_menu_text);
					std::cout << "Game speed reset" << std::endl;
				}
			}
			else
				is_3_pressed = false;

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