#include <Windows.h>
#include <iostream>

int main()
{
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
    HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, false, pID);

    // Memory address of money in game
    LPCVOID address = reinterpret_cast<LPCVOID>(0x00B7CE50);

    // Read money value into variable
    ReadProcessMemory(pHandle, (LPVOID)address, &money, sizeof(money), nullptr);
    std::cout << "Money before funny: " << money << "\n";

    // Set money value from variable
    WriteProcessMemory(pHandle, (LPVOID)address, &new_money, sizeof(new_money), nullptr);

    // Read money value again to see if it worked
    ReadProcessMemory(pHandle, (LPVOID)address, &money, sizeof(money), nullptr);
    std::cout << "Money after funny: " << money << "\n";

    system("PAUSE");
    return 0;
}