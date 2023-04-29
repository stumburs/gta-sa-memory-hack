#include <Windows.h>
#include <iostream>

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

int main()
{
	bool running = true;

	// Variable to hold in-game money value
	DWORD money = 0;

	// Variable to set new money value
	DWORD new_money = 69420;

	// Find game window
	HWND hWnd = FindWindowA(0, "GTA: San Andreas");

	// Set process ID
	DWORD pID = 0;
	GetWindowThreadProcessId(hWnd, &pID);

	// Do something
	p_handle = OpenProcess(PROCESS_ALL_ACCESS, false, pID);

	// Memory address of money in game
	LPCVOID money_address = reinterpret_cast<LPCVOID>(0x00B7CE50);

	bool is_alt_1_pressed = false;
	while (running)
	{
		// Money
		if ((GetAsyncKeyState(VK_MENU) & 0x8000) && (GetAsyncKeyState(0x31) & 0x8000))
		{
			if (!is_alt_1_pressed)
			{
				is_alt_1_pressed = true;
				SetValue(money_address, new_money);
				std::cout << "Money: " << GetValue<DWORD>(money_address) << std::endl;
			}
		}
		else
			is_alt_1_pressed = false;

		Sleep(100);
	}

	return 0;
}