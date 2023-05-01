#include "PlayerFunctions.h"
#include "MemoryFunctions.h"
#include <Windows.h>

namespace PlayerFunctions
{
	void AddMoney()
	{
		DWORD current_money = MemoryFunctions::GetValue<DWORD>(Addresses::money);
		DWORD new_money = current_money + 10000;
		MemoryFunctions::SetValue(Addresses::money, new_money);
		//timer = 1;
		//DisplayIngameAlert("Added $10000");
		std::cout << "Money: " << MemoryFunctions::GetValue<DWORD>(Addresses::money) << std::endl;
	}

	void SpeedUpGame()
	{
		float value = MemoryFunctions::GetValue<float>(Addresses::game_speed) + MemoryFunctions::GetValue<float>(Addresses::game_speed) / 2;
		MemoryFunctions::SetValue(Addresses::game_speed, value);
		std::cout << "Game Speed: " << value << std::endl;
	}

	void ResetGameSpeed()
	{
		float value = 1.0f;
		MemoryFunctions::SetValue(Addresses::game_speed, value);
		std::cout << "Game speed reset" << std::endl;
	}
}