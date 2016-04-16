#include "GameSaver.h"


CGameSaver::CGameSaver(CVectorManager * _vm, string _saveName, int _levelId)
{
	vm = _vm;
	saveName = _saveName;
	levelId = _levelId;
}


CGameSaver::~CGameSaver()
{
}
string CGameSaver::GetRow(const string & file, int rownumber)
{
	int rowCounter = 0;
	for (int i = 0; i < file.size(); i++)
	{
		if (rowCounter == rownumber)
		{
			int counter = 0;
			string row;
			while (true)
			{
				if (i + counter < file.size())
				{
					if (file.at(i + counter) != '\n')
					{
						counter++;
					}
					else
					{
						row = file.substr(i, counter);
						return row;
					}
				}
				else return "eof";
			}
		}
		if (file.at(i) == '\n')
		{
			rowCounter++;
		}
	}
	return "eof";
}
CPlayer * CGameSaver::LoadGame(int x, int y, int textureId, int size)
{
	CPlayer* newPlayer = new CPlayer(x, y, textureId, size, *vm->GetWeaponVector());
	string savefile;
	std::fstream fileStream;
	fileStream.open("savegames/" + saveName + "/" + std::to_string(levelId) + ".txt");
	while (!fileStream.eof())
	{
		char temp;
		fileStream.read(&temp, 1);
		savefile += temp;
	}
	fileStream.close();

	int row = 0;
	//Load hp
	newPlayer->SetHP(atoi(GetRow(savefile, row).c_str()));

	//Load weapons
	for (int i = 0; i < numberOfSlots; i++)
	{
		row++;
		newPlayer->AddWeaponToSlot(atoi(GetRow(savefile, row).c_str()), i);
	}
	//Load clip ammo
	for (int i = 0; i < numberOfSlots; i++)
	{
		row++;
		newPlayer->SetAmmoInClip(i,atoi(GetRow(savefile, row).c_str()));
	}
	//Load inventory ammo
	for (int i = 0; i < numberOfAmmoTypes; i++)
	{
		row++;
		newPlayer->AddAmmo(i, atoi(GetRow(savefile, row).c_str()));
	}
	row++;
	newPlayer->SwitchWeapon(atoi(GetRow(savefile, row).c_str()));
	return newPlayer;
}
void CGameSaver::SaveGame()
{
	std::fstream fileStream;
	CPlayer * player = vm->GetPlayer();
	//Create a savefile, 
	fileStream.open("savegames/" + saveName + "/" + std::to_string(levelId + 1) + ".txt", std::fstream::out | std::ofstream::trunc);
	//Save HP
	fileStream << to_string(player->GetHP()) + "\n";
	//Save weapon inventory
	for (int i = 0; i < numberOfSlots; i++)
	{
		fileStream << to_string(player->WeaponAtSlot(i)) + "\n";

	}
	//Save ammo in clips
	for (int i = 0; i < numberOfSlots; i++)
	{
		fileStream << to_string(player->GetCurrentClip(i)) + "\n";
	}
	//Save ammo inventory
	for (int i = 0; i < numberOfAmmoTypes; i++)
	{
		fileStream << to_string(player->GetCurrentReserveAmmo(i)) + "\n";
	}
	fileStream << to_string(player->GetCurrentSlot()) + "\n";
	fileStream.close();
}