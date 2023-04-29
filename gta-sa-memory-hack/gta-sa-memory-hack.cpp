#include <Windows.h>
#include <iostream>
#include <string>

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

	// Memory address of money in game
	LPCVOID money_address = reinterpret_cast<LPCVOID>(0x00B7CE50);
	LPCVOID blur_level_address = reinterpret_cast<LPCVOID>(0x8D5104);
	LPCVOID game_speed_address = reinterpret_cast<LPCVOID>(0xB7CB64);


	bool is_alt_1_pressed = false;
	bool is_alt_2_pressed = false;
	bool is_shift_2_pressed = false;
	bool is_ctr_2_pressed = false;
	while (running)
	{
		// Money
		if ((GetAsyncKeyState(VK_MENU) & 0x8000) && (GetAsyncKeyState(0x31) & 0x8000))
		{
			if (!is_alt_1_pressed)
			{
				is_alt_1_pressed = true;
				DWORD current_money = GetValue<DWORD>(money_address);
				DWORD new_money = current_money + 10000;
				SetValue(money_address, new_money);
				DisplayIngameAlert("Added $10000");
				std::cout << "Money: " << GetValue<DWORD>(money_address) << std::endl;
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
				float current_game_speed = GetValue<float>(game_speed_address);
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
				float value = GetValue<float>(game_speed_address) + GetValue<float>(game_speed_address) / 2;
				SetValue(game_speed_address, value);
				DisplayIngameAlert("New game speed: " + std::to_string(value));
				std::cout << "New Game Speed: " << GetValue<float>(game_speed_address) << std::endl;
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
				SetValue(game_speed_address, value);
				DisplayIngameAlert("Game speed reset");
				std::cout << "Game speed reset" << std::endl;
			}
		}
		else
			is_ctr_2_pressed = false;

		Sleep(100);
	}

	return 0;
}