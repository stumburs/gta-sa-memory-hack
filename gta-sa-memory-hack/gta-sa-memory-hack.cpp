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
	const char* alert_text = text.c_str();
	WriteProcessMemory(p_handle, (LPVOID)0xBAA7A0, alert_text, strlen(alert_text) + 1, nullptr);
}

// Define a function to check if a combination of keys is pressed
bool IsComboPressed(const int* combo, int size)
{
	for (int i = 0; i < size; i++)
		if (!(GetAsyncKeyState(combo[i]) & 0x8000))
			return false;
	return true;
}

// Define a function to handle key state for a given combo
void HandleComboState(const int* combo, int size, bool& is_combo_pressed, std::function<void()> action)
{
	if (IsComboPressed(combo, size))
		if (!is_combo_pressed)
		{
			is_combo_pressed = true;
			action();
		}
	else
		is_combo_pressed = false;
}

int main()
{
	bool running = true;

	// Variable to hold in-game money value
	DWORD money = 0;

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
	bool is_ctrl_2_pressed = false;
	while (running)
	{
		HandleComboState(ALT_1, 2, is_alt_1_pressed, []()
			{
				DWORD current_money = GetValue<DWORD>(Addresses::money);
				DWORD new_money = current_money + 10000;
				SetValue(Addresses::money, new_money);
				DisplayIngameAlert("Added $10000");
				std::cout << "Money: " << GetValue<DWORD>(Addresses::money) << std::endl;
			});

		HandleComboState(ALT_2, 2, is_alt_2_pressed, []()
			{
				float current_game_speed = GetValue<float>(Addresses::game_speed);
				DisplayIngameAlert("Current game speed: " + std::to_string(current_game_speed));
				std::cout << "Game Speed: " << current_game_speed << std::endl;
			});

		HandleComboState(SHIFT_2, 2, is_shift_2_pressed, []()
			{
				float value = GetValue<float>(Addresses::game_speed) + GetValue<float>(Addresses::game_speed) / 2;
				SetValue(Addresses::game_speed, value);
				DisplayIngameAlert("New game speed: " + std::to_string(value));
				std::cout << "New Game Speed: " << GetValue<float>(Addresses::game_speed) << std::endl;
			});

		HandleComboState(CTRL_2, 2, is_ctrl_2_pressed, []()
			{
				float value = 1.0f;
				SetValue(Addresses::game_speed, value);
				DisplayIngameAlert("Game speed reset");
				std::cout << "Game speed reset" << std::endl;
			});

		Sleep(100);
	}

	return 0;
}