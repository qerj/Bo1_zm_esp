#include "includes.h"
#include "Offsets.h"
#include "Helper.h"
#include "MemMan.h"
#include <iostream>
#include "mem.h"
#include <string>

#ifdef _WIN64
#define GWL_WNDPROC GWLP_WNDPROC
#endif

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

EndScene oEndScene = NULL;
WNDPROC oWndProc;
static HWND window = NULL;

void InitImGui(LPDIRECT3DDEVICE9 pDevice)
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(pDevice);
}

bool init = false;
bool showGui = false;
bool showEsp = false;
//uintptr_t clientBase = (uintptr_t)GetModuleHandleA("client.dll");
uintptr_t clientBase = (uintptr_t)GetModuleHandleA("BlackOps.exe");

//ID3DXLine* LineL;
ID3DXLine* pLine;
//void DrawLine(LPDIRECT3DDEVICE9 device,int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color)
//{
//	if (!LineL)
//	D3DXCreateLine(device, &LineL);
//	D3DXVECTOR2 Line[2];
//	Line[0] = D3DXVECTOR2(x1, y1);
//	Line[1] = D3DXVECTOR2(x2, y2);
//	LineL->SetWidth(thickness);
//	LineL->Draw(Line, 2, color);
//}
//
//void DrawBox(float X, float Y, float W, float H, ImColor Color)
//{
//	auto window = ImGui::GetBackgroundDrawList();
//	
//	window->AddRect(ImVec2(X + 1, Y + 1), ImVec2(((X + W) - 1), ((Y + H) - 1)), Color);
//	window->AddRect(ImVec2(X, Y), ImVec2(X + W, Y + H), Color);
//	
//}

void DrawLine(float x, float y, float x2, float y2, int r, int g, int b, int a, float w) {
	D3DXVECTOR2 points[2];
	pLine->SetWidth(w);

	points[0] = D3DXVECTOR2(x, y);
	points[1] = D3DXVECTOR2(x2, y2);

	pLine->Begin();
	pLine->Draw(points, 2, D3DCOLOR_RGBA(r, g, b, a));
	pLine->End();
}

ImDrawList* list;
long __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{
	if (!init)
	{
		InitImGui(pDevice);
		init = true;
	}
	MemMan mem;
	
	uintptr_t Process = mem.getProcess(L"BlackOps.exe");
	if (!Process) {
		std::cout << "Black ops not found!\n";
	}

	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	if (GetAsyncKeyState(VK_END) & 1) { fclose(f); FreeConsole(); kiero::shutdown(); }
	if (GetAsyncKeyState(VK_INSERT) & 1) { showGui = !showGui; }
	if (GetAsyncKeyState(VK_F1) & 1) { showEsp = !showEsp; }


	if (showGui)
	{
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// not even uc can help
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(1920, 1080));
		ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground);

		auto pDrawList = ImGui::GetWindowDrawList();


		ViewMatrix Matrix = mem.readMem<ViewMatrix>(Offsets::viewMatrix);

		for (short int i = 0; i < 12; ++i)
		{

			uintptr_t Entity = mem.readMem<uintptr_t>(clientBase + Offsets::entityList + i * Offsets::distanceBetween);

			if (Entity == NULL)	continue;

			//uintptr_t localPlayer = mem.readMem<uintptr_t>(clientBase + Offsets::localPlayer);

			//int playerHealth = mem.readMem<int>(localPlayer + Offsets::health);

			//Vec3 playerOrigin = mem.readMem<Vec3>(localPlayer + Offsets::vecOrigin);

			int Health = mem.readMem<int>(Entity + Offsets::health);
			//int enemyTeam = mem.readMem<int>(Entity + Offsets::teamNum);
			//int playerTeam = mem.readMem<int>(localPlayer + Offsets::teamNum);
			//bool isDormant = mem.readMem<bool>(Entity + Offsets::m_bDormant);

			if (Health <= 0)
				continue;

			//std::cout << "Health: " << Health << std::endl;

			//if (playerTeam == enemyTeam)
				//continue;

			//if (isDormant)
			//	continue;





			Vec3 entityLocation = mem.readMem<Vec3>(Entity + Offsets::origin);

			ImVec2 ScreenCoords;

			//so it was the world to screen function after all
			if (!WorldToScreen2(entityLocation, ScreenCoords, Matrix.Matrix))
				continue;

	
			int r = 0;
			int g = 255;
			int b = 0;

			if (Health <= 100)
			{
				r = 255;
				g = 255;
				b = 0;
			}
			if (Health <= 50)
			{
				r = 255;
				g = 0;
				b = 0;
			}
		
			
			pDrawList->AddText(ImVec2(ScreenCoords.x, ScreenCoords.y - 100), ImColor(r, g, b, 255), std::to_string(Health).c_str());
			pDrawList->AddText(ImVec2(ScreenCoords.x, ScreenCoords.y), ImColor(255, 255, 255, 255), "Zombie");
			pDrawList->AddLine(ImVec2(1920 / 2, 1080 / 2 - 2000), ImVec2(ScreenCoords.x, ScreenCoords.y), ImColor(255, 0, 0, 255), 2.0f);
		}
		ImGui::End();
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}

	return oEndScene(pDevice);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
{
	DWORD wndProcId;
	GetWindowThreadProcessId(handle, &wndProcId);

	if (GetCurrentProcessId() != wndProcId)
		return TRUE; // skip to next window

	window = handle;
	return FALSE; // window found abort search
}

HWND GetProcessWindow()
{
	window = NULL;
	EnumWindows(EnumWindowsCallback, NULL);
	return window;
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool attached = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D9) == kiero::Status::Success)
		{
			kiero::bind(42, (void**)& oEndScene, hkEndScene);
			do
				window = GetProcessWindow();
			while (window == NULL);
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWL_WNDPROC, (LONG_PTR)WndProc);
			attached = true;
		}
	} while (!attached);
	return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}
