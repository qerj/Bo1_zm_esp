#pragma once
#include <Windows.h>


namespace Offsets
{
	//csgo
	//uintptr_t dwEntityList = 0x4DFFF14;
	//uintptr_t vecOrigin = 0x138;
	//uintptr_t health = 0x100;
	//uintptr_t distanceBetween = 0x10;
	//uintptr_t viewMatrix = 0x4DF0D44;
	//uintptr_t teamNum = 0xF4;
	//uintptr_t localPlayer = 0xDEA964;
	//uintptr_t m_bDormant = 0xED;

	//css
	//uintptr_t dwEntityList = 0x004D5AE4;
	//uintptr_t vecOrigin = 0x138;
	//uintptr_t health = 0x8C;
	//uintptr_t distanceBetween = 0x4;
	//uintptr_t viewMatrix = 0x140D2950;
	//uintptr_t teamNum = 0xF4;
	//uintptr_t localPlayer = 0xDEA964;
	//uintptr_t m_bDormant = 0xED;

	//Black ops 1
	uintptr_t entityList = 0x017FBC84;
	uintptr_t distanceBetween = 0x8C;
	uintptr_t origin = 0x18; //there are 2, the first start is 0x18, and the other is 0x24
	uintptr_t health = 0x184;
	uintptr_t viewMatrix = 0x00BA6970;//0x7A6930 + 0x4 + 0x4 + 0x4 + 0x4 + 0x4 + 0x4 + 0x4 + 0x4 + 0x4 + 0x4 + 0x4 + 0x4 + 0x4 + 0x4;


	//tbh since cod bo1 is running on quake we should have the same type of variables maybe
}