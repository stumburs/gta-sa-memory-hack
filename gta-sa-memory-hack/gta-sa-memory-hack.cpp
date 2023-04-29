#include <Windows.h>
#include <iostream>
#include <string>
#include <functional>

namespace Addresses
{
	LPCVOID money = reinterpret_cast<LPCVOID>(0x00B7CE50);
	LPCVOID blur_level = reinterpret_cast<LPCVOID>(0x8D5104);
	LPCVOID game_speed = reinterpret_cast<LPCVOID>(0xB7CB64);
};

std::string menu_top_bar[] = {"-=-=-=-=-=-=-=-=-=-", "=-=-=-=-=-=-=-=-=-="};

// Define key codes for the combinations
const int ALT_1[] = { VK_MENU, 0x31 }; // Alt + 1
const int ALT_2[] = { VK_MENU, 0x32 }; // Alt + 2
const int SHIFT_2[] = { VK_LSHIFT, 0x32 }; // Shift + 2
const int CTRL_2[] = { VK_CONTROL, 0x32 }; // Ctrl + 2

HANDLE p_handle;

// Define the function type
typedef void (*MyFunctionType)();

// Get the function address
MyFunctionType MyFunction = (MyFunctionType)0x0043A510;

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

	// Find game window
	HWND hWnd = FindWindowA(0, "GTA: San Andreas");

	// Set process ID
	DWORD pID = 0;
	GetWindowThreadProcessId(hWnd, &pID);

	// Do something
	p_handle = OpenProcess(PROCESS_ALL_ACCESS, false, pID);

	bool is_alt_1_pressed = false;
	bool is_alt_2_pressed = false;
	bool is_shift_2_pressed = false;
	bool is_ctr_2_pressed = false;

	bool is_menu_open = false;
	bool is_alt_m_pressed = false;

	while (running)
	{
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


		// Money
		if ((GetAsyncKeyState(VK_MENU) & 0x8000) && (GetAsyncKeyState(0x31) & 0x8000))
		{
			if (!is_alt_1_pressed)
			{
				is_alt_1_pressed = true;
				DWORD current_money = GetValue<DWORD>(Addresses::money);
				DWORD new_money = current_money + 10000;
				SetValue(Addresses::money, new_money);
				DisplayIngameAlert("Added $10000");
				std::cout << "Money: " << GetValue<DWORD>(Addresses::money) << std::endl;
			}
		}
		else
			is_alt_1_pressed = false;

		// Get game speed
		if ((GetAsyncKeyState(VK_MENU) & 0x8000) && (GetAsyncKeyState(0x32) & 0x8000))
		{
			if (!is_alt_2_pressed)
			{
				is_alt_2_pressed = true;
				float current_game_speed = GetValue<float>(Addresses::game_speed);
				DisplayIngameAlert("Current game speed: " + std::to_string(current_game_speed));
				std::cout << "Game Speed: " << current_game_speed << std::endl;
			}
		}
		else
			is_alt_2_pressed = false;

		// Increase game speed
		if ((GetAsyncKeyState(VK_LSHIFT) & 0x8000) && (GetAsyncKeyState(0x32) & 0x8000))
		{
			if (!is_shift_2_pressed)
			{
				is_shift_2_pressed = true;
				float value = GetValue<float>(Addresses::game_speed) + GetValue<float>(Addresses::game_speed) / 2;
				SetValue(Addresses::game_speed, value);
				DisplayIngameAlert("New game speed: " + std::to_string(value));
				std::cout << "New Game Speed: " << GetValue<float>(Addresses::game_speed) << std::endl;
			}
		}
		else
			is_shift_2_pressed = false;

		// Reset game speed
		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState(0x32) & 0x8000))
		{
			if (!is_ctr_2_pressed)
			{
				is_ctr_2_pressed = true;
				float value = 1.0f;
				SetValue(Addresses::game_speed, value);
				DisplayIngameAlert("Game speed reset");
				std::cout << "Game speed reset" << std::endl;
			}
		}
		else
			is_ctr_2_pressed = false;

		std::cout << std::boolalpha << is_menu_open << "\n";

		if (is_menu_open)
			DisplayIngameAlert("Menu! Press ALT+M to close.");

		Sleep(100);
	}

	return 0;
}