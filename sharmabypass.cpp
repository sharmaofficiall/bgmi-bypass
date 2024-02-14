#include "sharmabypass.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include <Settings.h>
#include <main.h>
#include <thread>



#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include <Psapi.h>
#include <thread>
#include <iterator>
#include <math.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <atlbase.h>
#include <atlstr.h>
#include <memory.h>
#include <iostream>


#include <filesystem>

#include <fstream>
#include <Windows.h>
#include <tlhelp32.h>
#include <thread>
#include <filesystem> 
#include "main.h"
#include <urlmon.h>
#include"Memx.h"
#include "Settings.h"


using namespace std;
string STATUS = "Welcome to SHARMA BYPASS.";

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter
);
typedef struct _MEMORY_REGION {
	DWORD_PTR dwBaseAddr;
	DWORD_PTR dwMemorySize;
}MEMORY_REGION;

long __stdcall WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter)
{
	if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter))
		return true;

	switch (message)
	{
	case WM_SIZE: {
		if (gui::device && wideParameter != SIZE_MINIMIZED)
		{
			gui::presentParameters.BackBufferWidth = LOWORD(longParameter);
			gui::presentParameters.BackBufferHeight = HIWORD(longParameter);
			gui::ResetDevice();
		}
	}return 0;

	case WM_SYSCOMMAND: {
		if ((wideParameter & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
	}break;

	case WM_DESTROY: {
		PostQuitMessage(0);
	}return 0;

	case WM_LBUTTONDOWN: {
		gui::position = MAKEPOINTS(longParameter); // set click points
	}return 0;

	case WM_MOUSEMOVE: {
		if (wideParameter == MK_LBUTTON)
		{
			const auto points = MAKEPOINTS(longParameter);
			auto rect = ::RECT{ };

			GetWindowRect(gui::window, &rect);

			rect.left += points.x - gui::position.x;
			rect.top += points.y - gui::position.y;

			if (gui::position.x >= 0 &&
				gui::position.x <= gui::WIDTH &&
				gui::position.y >= 0 && gui::position.y <= 19)
				SetWindowPos(
					gui::window,
					HWND_TOPMOST,
					rect.left,
					rect.top,
					0, 0,
					SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
				);
		}

	}return 0;

	}

	return DefWindowProc(window, message, wideParameter, longParameter);
}

void gui::CreateHWindow(const char* windowName) noexcept
{
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_CLASSDC;
	windowClass.lpfnWndProc = WindowProcess;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandleA(0);
	windowClass.hIcon = 0;
	windowClass.hCursor = 0;
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = "class001";
	windowClass.hIconSm = 0;

	RegisterClassEx(&windowClass);

	window = CreateWindowEx(
		0,
		"class001",
		windowName,
		WS_POPUP,
		100,
		100,
		WIDTH,
		HEIGHT,
		0,
		0,
		windowClass.hInstance,
		0
	);

	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);
}

void gui::DestroyHWindow() noexcept
{
	DestroyWindow(window);
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

bool gui::CreateDevice() noexcept
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d)
		return false;

	ZeroMemory(&presentParameters, sizeof(presentParameters));

	presentParameters.Windowed = TRUE;
	presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	presentParameters.EnableAutoDepthStencil = TRUE;
	presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
	presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if (d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		window,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&presentParameters,
		&device) < 0)
		return false;

	return true;
}

void gui::ResetDevice() noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	const auto result = device->Reset(&presentParameters);

	if (result == D3DERR_INVALIDCALL)
		IM_ASSERT(0);

	ImGui_ImplDX9_CreateDeviceObjects();

}

void gui::DestroyDevice() noexcept
{
	if (device)
	{
		device->Release();
		device = nullptr;
	}

	if (d3d)
	{
		d3d->Release();
		d3d = nullptr;
	}
}

int Bypassx(std::string command)
{
	command.insert(0, "/C ");

	SHELLEXECUTEINFOA ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = "cmd.exe";
	ShExecInfo.lpParameters = command.c_str();
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;

	if (ShellExecuteExA(&ShExecInfo) == FALSE)
		return -1;

	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

	DWORD rv;
	GetExitCodeProcess(ShExecInfo.hProcess, &rv);
	CloseHandle(ShExecInfo.hProcess);

	return rv;


}int getAowProcId22x()
{
	int pid = 0;
	PROCESS_MEMORY_COUNTERS ProcMC;
	PROCESSENTRY32 ProcEntry;
	ProcEntry.dwSize = sizeof(PROCESSENTRY32);
	HANDLE ProcHandle;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (Process32First(snapshot, &ProcEntry) == TRUE)
	{
		while (Process32Next(snapshot, &ProcEntry) == TRUE)
		{
			if (strcmp(ProcEntry.szExeFile, "AndroidEmulatorEx.exe") == 0)
			{
				ProcHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, ProcEntry.th32ProcessID);

				if (NULL == ProcHandle)
					continue;

				if (GetProcessMemoryInfo(ProcHandle, &ProcMC, sizeof(ProcMC)))
				{
					if (ProcMC.WorkingSetSize > 300000000)
					{
						pid = ProcEntry.th32ProcessID;
						return pid;
						break;
					}

				}

				CloseHandle(ProcHandle);
			}
		}
	}

	CloseHandle(snapshot);
}

int getAowProcIdx()
{
	int pid = 0;
	PROCESS_MEMORY_COUNTERS ProcMC;
	PROCESSENTRY32 ProcEntry;
	ProcEntry.dwSize = sizeof(PROCESSENTRY32);
	HANDLE ProcHandle;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (Process32First(snapshot, &ProcEntry) == TRUE)
	{
		while (Process32Next(snapshot, &ProcEntry) == TRUE)
		{
			if (strcmp(ProcEntry.szExeFile, "aow_exe.exe") == 0)
			{
				ProcHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, ProcEntry.th32ProcessID);

				if (NULL == ProcHandle)
					continue;

				if (GetProcessMemoryInfo(ProcHandle, &ProcMC, sizeof(ProcMC)))
				{
					if (ProcMC.WorkingSetSize > 300000000)
					{
						pid = ProcEntry.th32ProcessID;
						return pid;
						break;
					}

				}

				CloseHandle(ProcHandle);
			}
		}
	}

	CloseHandle(snapshot);
}


void StealthX()
{
	HWND Stealth;
	AllocConsole();
	Stealth = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(Stealth, 0);
}
void DownloadFile(string DownloadLink, string SaveLocation)
{
	string initialargument = "curl.exe --url " + DownloadLink + " --output " + SaveLocation;
	const char* argument = initialargument.c_str();
	system("@echo off");
	system(argument);
}

int getProcId2x()
{
	{
		int aow = 0;
		aow = getAowProcIdx();

			if (aow == 0 || aow == 1)
				if (aow == 0 || aow == 1)
					{
						return 0;
					}
				
				else
			
				{
					return aow;
				}		
	}
					
}

void offsetsearch2x(int offset, BYTE write[], SIZE_T size, int header)
{
	DWORD pid = getProcId2x();
	HANDLE phandle = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
	int addr = header + offset;
	unsigned long OldProtect;
	unsigned long OldProtect2;
	VirtualProtectEx(phandle, (BYTE*)addr, size, PAGE_EXECUTE_READWRITE, &OldProtect);
	WriteProcessMemory(phandle, (BYTE*)addr, write, size, NULL);
	VirtualProtectEx(phandle, (BYTE*)addr, size, OldProtect, NULL);
}
int SundaySearchx(BYTE* bStartAddr, int dwSize, BYTE* bSearchData, DWORD dwSearchSize)
{
	if (dwSize < 0)
	{
		return -1;
	}
	int iIndex[256] = { 0 };
	int i, j;
	DWORD k;

	for (i = 0; i < 256; i++)
	{
		iIndex[i] = -1;
	}

	j = 0;
	for (i = dwSearchSize - 1; i >= 0; i--)
	{
		if (iIndex[bSearchData[i]] == -1)
		{
			iIndex[bSearchData[i]] = dwSearchSize - i;
			if (++j == 256)
				break;
		}
	}
	i = 0;
	BOOL bFind = FALSE;
	//j=dwSize-dwSearchSize+1;
	j = dwSize - dwSearchSize + 1;
	while (i < j)
	{
		for (k = 0; k < dwSearchSize; k++)
		{
			if (bStartAddr[i + k] != bSearchData[k])
				break;
		}
		if (k == dwSearchSize)
		{
			//ret=bStartAddr+i;
			bFind = TRUE;
			break;
		}
		if (i + dwSearchSize >= dwSize)
		{

			return -1;
		}
		k = iIndex[bStartAddr[i + dwSearchSize]];
		if (k == -1)
			i = i + dwSearchSize + 1;
		else
			i = i + k;
	}
	if (bFind)
	{
		return i;
	}
	else
		return -1;

}

int MemFindx(BYTE* buffer, int dwBufferSize, BYTE* bstr, DWORD dwStrLen)
{
	if (dwBufferSize < 0)
	{
		return -1;
	}
	DWORD  i, j;
	for (i = 0; i < dwBufferSize; i++)
	{
		for (j = 0; j < dwStrLen; j++)
		{
			if (buffer[i + j] != bstr[j] && bstr[j] != '?')
				break;
		}
		if (j == dwStrLen)
			return i;
	}
	return -1;
}



BOOL MemSearchx(BYTE* bSearchData, int nSearchSize, DWORD_PTR dwStartAddr, DWORD_PTR dwEndAddr, BOOL bIsCurrProcess, int iSearchMode, std::vector<DWORD_PTR>& vRet)
{
	DWORD pid = getProcId2x();
	HANDLE phandle = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
	BYTE* pCurrMemoryData = NULL;
	MEMORY_BASIC_INFORMATION	mbi;
	std::vector<MEMORY_REGION> m_vMemoryRegion;
	mbi.RegionSize = 0x1000;
	DWORD dwAddress = dwStartAddr;



	while (VirtualQueryEx(phandle, (LPCVOID)dwAddress, &mbi, sizeof(mbi)) && (dwAddress < dwEndAddr) && ((dwAddress + mbi.RegionSize) > dwAddress))
	{

		if ((mbi.State == MEM_COMMIT) && ((mbi.Protect & PAGE_GUARD) == 0) && (mbi.Protect != PAGE_NOACCESS) && ((mbi.AllocationProtect & PAGE_NOCACHE) != PAGE_NOCACHE))
		{

			MEMORY_REGION mData = { 0 };
			mData.dwBaseAddr = (DWORD_PTR)mbi.BaseAddress;
			mData.dwMemorySize = mbi.RegionSize;
			m_vMemoryRegion.push_back(mData);

		}
		dwAddress = (DWORD)mbi.BaseAddress + mbi.RegionSize;

	}


	std::vector<MEMORY_REGION>::iterator it;
	for (it = m_vMemoryRegion.begin(); it != m_vMemoryRegion.end(); it++)
	{
		MEMORY_REGION mData = *it;


		DWORD_PTR dwNumberOfBytesRead = 0;

		if (bIsCurrProcess)
		{
			pCurrMemoryData = (BYTE*)mData.dwBaseAddr;
			dwNumberOfBytesRead = mData.dwMemorySize;
		}
		else
		{

			pCurrMemoryData = new BYTE[mData.dwMemorySize];
			ZeroMemory(pCurrMemoryData, mData.dwMemorySize);
			ReadProcessMemory(phandle, (LPCVOID)mData.dwBaseAddr, pCurrMemoryData, mData.dwMemorySize, &dwNumberOfBytesRead);

			if ((int)dwNumberOfBytesRead <= 0)
			{
				delete[] pCurrMemoryData;
				continue;
			}
		}
		if (iSearchMode == 0)
		{
			DWORD_PTR dwOffset = 0;
			int iOffset = MemFindx(pCurrMemoryData, dwNumberOfBytesRead, bSearchData, nSearchSize);
			while (iOffset != -1)
			{
				dwOffset += iOffset;
				vRet.push_back(dwOffset + mData.dwBaseAddr);
				dwOffset += nSearchSize;
				iOffset = MemFindx(pCurrMemoryData + dwOffset, dwNumberOfBytesRead - dwOffset - nSearchSize, bSearchData, nSearchSize);
			}
		}
		else if (iSearchMode == 1)
		{

			DWORD_PTR dwOffset = 0;
			int iOffset = SundaySearchx(pCurrMemoryData, dwNumberOfBytesRead, bSearchData, nSearchSize);

			while (iOffset != -1)
			{
				dwOffset += iOffset;
				vRet.push_back(dwOffset + mData.dwBaseAddr);
				dwOffset += nSearchSize;
				iOffset = MemFindx(pCurrMemoryData + dwOffset, dwNumberOfBytesRead - dwOffset - nSearchSize, bSearchData, nSearchSize);
			}

		}

		if (!bIsCurrProcess && (pCurrMemoryData != NULL))
		{
			delete[] pCurrMemoryData;
			pCurrMemoryData = NULL;
		}

	}
	return TRUE;
}

int SINGLEAOBSCANx(BYTE BypaRep[], SIZE_T size)
{

	 if (Settings::Gameloop)
	{
		DWORD pid = getProcId2x();
		HANDLE phandle = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
		std::vector<DWORD_PTR> Bypassxdo;
		//MemSearch(BypaRep, size, 0x40000000, 0x60000000, false, 0, Bypassxdo);
		MemSearchx(BypaRep, size, 0x26000000, 0xB0000000, false, 0, Bypassxdo);

		if (Bypassxdo.size() != 0) {
			return Bypassxdo[0];
		}

	}
}



int SINGLEAOBSCAN2x(BYTE BypaRep[], SIZE_T size)//this is for tersafe
{

	 if (Settings::Gameloop)//change
	{
		DWORD pid = getProcId2x();
		HANDLE phandle = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
		std::vector<DWORD_PTR> Bypassxdo;
		MemSearchx(BypaRep, size, 0x40000000, 0x41000000, false, 0, Bypassxdo);

		if (Bypassxdo.size() != 0) {
			return Bypassxdo[0];
		}

	}

}
void gui::CreateImGui() noexcept
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ::ImGui::GetIO();
	//io.Fonts->AddFontDefault();
	io.Fonts->AddFontFromFileTTF("C:/Windows/Ruda-Bold.ttf", 17.0f);

	ImGui::StyleColorsDark();
	ImGui::GetStyle().FrameRounding = 4.0f;
	ImGui::GetStyle().GrabRounding = 4.0f;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);




}


void gui::DestroyImGui() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

int gettersafeheaderx()
{
	int libtersafeheader = 0;
	//old //BYTE tersafehead[] = { 0x7F,0x45,0x4C,0x46,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x28,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x34,0x00,0x00,0x00,0x08,0xBD,0x3C };
	BYTE tersafehead[] = { 0x7F,0x45,0x4C,0x46,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x28,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x34,0x00,0x00,0x00,0x58,0xCD,0x3C };
	libtersafeheader = SINGLEAOBSCAN2x(tersafehead, sizeof(tersafehead));
	return libtersafeheader;
}
int getue4headerx()
{
	unsigned long  libue4header = 0;
	//BYTE ue4head[] = { 0x7F,0x45,0x4C,0x46,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x28,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x34,0x00,0x00,0x00,0x24,0x66,0x67 };
	BYTE ue4head[] = { 0x7F, 0x45, 0x4C, 0x46, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x28, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x24, 0x26, 0x8A, 0x07, 0x00, 0x02, 0x00, 0x05, 0x34, 0x00, 0x20, 0x00, 0x0A, 0x00, 0x28, 0x00, 0x1A, 0x00, 0x19, 0x00, 0x06, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00 };
	libue4header = SINGLEAOBSCANx(ue4head, sizeof(ue4head));
	return libue4header;
}

int getue4headerVnx()
{
	unsigned long  libue4header = 0;
	//BYTE ue4head[] = { 0x7F,0x45,0x4C,0x46,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x28,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x34,0x00,0x00,0x00,0x24,0x66,0x67 };
	BYTE ue4head[] = { 0x7F, 0x45, 0x4C, 0x46, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x28, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x24, 0x56, 0x8A, 0x07, 0x00, 0x02, 0x00, 0x05, 0x34, 0x00, 0x20, 0x00, 0x0A, 0x00, 0x28, 0x00, 0x1A, 0x00, 0x19, 0x00, 0x06, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x74, 0x01, 0x00, 0x00, 0x74, 0x01, 0x00, 0x00, 0x74, 0x01, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	libue4header = SINGLEAOBSCANx(ue4head, sizeof(ue4head));
	return libue4header;
}
void gui::BeginRender() noexcept
{
	MSG message;
	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);

		if (message.message == WM_QUIT)
		{
			exit = !exit;
			return;
		}
	}

	// Start the Dear ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void gui::EndRender() noexcept
{
	ImGui::EndFrame();

	device->SetRenderState(D3DRS_ZENABLE, FALSE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	if (device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();
	}

	const auto result = device->Present(0, 0, 0, 0);

	// Handle loss of D3D9 device
	if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		ResetDevice();
}

void theme() {

	ImGui::StyleColorsClassic();


	auto& style = ImGui::GetStyle();
	ImVec4* color = ImGui::GetStyle().Colors;

	style.WindowMinSize = ImVec2(300, 200);
	style.WindowRounding = 0.0f;
	style.WindowPadding = ImVec2(6, 6);
	style.FramePadding = ImVec2(4, 4);
	style.ItemSpacing = ImVec2(8,10);

	color[ImGuiCol_TitleBg] = ImVec4(0.000f, 0.000f, 0.983f, 0.830f);
	color[ImGuiCol_TitleBgActive] = ImVec4(0.000f, 0.000f, 1.000f, 0.870f);
	color[ImGuiCol_CheckMark] = ImVec4(1.000f, 0.298f, 0.000f, 0.500f);
	color[ImGuiCol_Button] = ImVec4(0.000f, 0.192f, 1.000f, 0.620f);
	color[ImGuiCol_ButtonHovered] = ImVec4(1.000f, 0.663f, 0.000f, 0.790f);
	color[ImGuiCol_ButtonActive] = ImVec4(0.000f, 1.000f, 0.619f, 1.000f);
	color[ImGuiCol_WindowBg] = ImColor(24, 24, 24, 255);

}
static float f = 0.0f;
static int counter = 0;

static float testfloat = 0.0f;
static int testint = 0;
static float X = 0.f;
static float Y = 0.f;
static int Radius = 0;
static bool chk = false;
static bool emu = false;
static bool rest = false;

static int choice = 0;
//static int Settings::choices = 1;
static int clicked = 0;
static int clicked1 = 0;
bool active_tab = 0;
const char* fmt="";
const char* bypass = "";

static const unsigned int MAX_FORMAT_STRING_LEN = 32768;
char str[MAX_FORMAT_STRING_LEN] = "";
bool IpadView = false;
bool Norecoil = false;
bool notree = false;
bool islandbypass = false;
bool Smallcrosshair = false;
bool InstaHit = false;
bool Xeffect = false;
bool luffy = false;
bool zero = false;
bool nofog = false;
bool nograss = false;
bool flashingame = false;
bool nightmode = false;
bool logoutid = false;
bool resetid = false;

//SELECT EMULATOR


int IPADSIZE = 50;
DWORD pidx=0;

namespace Settings
{
	static int Tab = 1;
}

unsigned long libue4header=0;
unsigned long libue4header2x =0;
int libtersafeheader = 0;
//MEMORY HACKS
bool MEMORYCHECK = false;
string readFile2(string location)
{
	string myText;
	ifstream MyReadFile(location);
	while (getline(MyReadFile, myText)) {
		cout << myText;
	}
	MyReadFile.close();
	return myText;
}
void gui::Render() noexcept
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	theme();
	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::SetNextWindowSize({ WIDTH, HEIGHT });
	//ImFont* font_droidsans_60 = ImGui::GetIO().Fonts->AddFontFromFileTTF("path/to/font/DroidSans.ttf", 60);

	ImGui::Begin(
		"                      SHARMA BYPASS BGMI 3.0",

		&exit,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove
	);


	ImGuiStyle& style = ImGui::GetStyle();
	auto FramePadding = style.FramePadding;
	style.FramePadding = ImVec2(4, 4); //exaggerated test values


	//if (ImGui::Button("BYPASS", ImVec2(115, 35)))
	//{
	//	Settings::Tab = 1;
	//}

	if (Settings::Tab == 1)
	{
		ImGui::TextColored(ImVec4(0, 255, 0, 122), "SELECT YOUR EMULATOR :");

		static int choice = 0;
	//	ImGui::Dummy(ImVec2(0.0f, 2.0f));
		ImGui::Combo("Select Emulator", &choice, "Gameloop EX\0Gameloop EN \0");
		ImGui::TextColored(ImVec4(0, 255, 0, 122), "SELECT YOUR GAME VERSION :");
		ImGui::RadioButton("BGMI", &Settings::choices, 1); 		ImGui::SameLine();


			//ImGui::Dummy(ImVec2(0.0f, 1.0f));
		ImGui::Spacing();

		//ImGui::Dummy(ImVec2(0.0f, 0.2f));
		//ImGui::Dummy(ImVec2(2.0f, 0.0f)); ImGui::SameLine();
		std::string gg = readFile2("C:\\sharmamodbypass.lic");
		bypass = gg.c_str();

		if (!emu && !rest == true)
			fmt = "READY TO BYPASS";

		if (Settings::bypassDone && emu) {
			fmt = "BYPASS DONE SUCCESSFUL";
		}

	//	fmt = std::s;
		if (ImGui::Button("START EMULATOR", { 487.0f, 35.f }))

		{

			if (choice == 0 && Settings::choices > 0) {

				Settings::Gameloop = true;
				std::thread(mainmenuaur1).detach();
				emu = true;
				fmt = "GAMELOOP EX IS STARTING...";
			}
			Settings::bypassDone = false;

			if (choice == 1 && Settings::choices > 0) {

				Settings::Gameloop = true;
				std::thread(mainmenuaur).detach();
				emu = true;
				fmt = "GAMELOOP EN IS STARTING...";
			}
			Settings::bypassDone = false;
		}
		if (ImGui::Button("START GAME", { 487.0f, 35.f }))
		{

			if (choice == 0 && Settings::choices > 0) {

				Settings::Gameloop = true;
				std::thread(mainmenuaur2).detach();

				fmt = "Starting Game...";
				emu = true;
				
			}

		}

		if (ImGui::Button("SAFE EXIT", { 487.0f, 35.f }))
		{
			StealthX();
			system("taskkill /f /im androidemulatorEx.exe");
			system("taskkill /f /im androidemulatorEn.exe");
			system("taskkill /f /im appmarket.exe");
			system("taskkill /f /im aow_exe.exe");
			system("taskkill /f /im QMEmulatorService.exe");
			system("taskkill /f /im RuntimeBroker.exe");
			system("taskkill /f /im adb.exe");
			system("taskkill /f /im GameLoader.exe");
			system("taskkill /f /im TSettingCenter.exe");
			system("taskkill /f /im syzs_dl_svr.exe");
			std::thread(safeExit).detach();
			fmt = "SAFE EXIT...";

		}

		else
		//IpadView = true;
		//ImGui::SameLine();
		ImGui::SliderInt("", &IPADSIZE, (40), (200));
		ImGui::Checkbox("Logout ID", &logoutid);
		ImGui::SameLine();
		ImGui::Checkbox("Reset ID", &resetid);
		//ImGui::SameLine();

		if (ImGui::Checkbox("Wide View", &IpadView))
		{
			if (Settings::Gameloop)
			{


				if (pidx == 0 && libue4header == 0) {
					pidx = getProcId2x();
					libue4header = getue4headerx();

					if (Settings::choices == 1)
						libue4header = getue4headerx();
					if (Settings::choices == 4)
						libue4header2x = getue4headerVnx();

				}
				BYTE ipad[] = { 0x00, 0x00,IPADSIZE };
				
				if (Settings::choices == 1)
					offsetsearch2x(0x3E825E4, ipad, sizeof(ipad), libue4header);

			}

		}
		else
		ImGui::SameLine();
		ImGui::SliderInt("", &IPADSIZE, 40, 200);
		//	IpadView = false;

		ImGui::TextColored(ImVec4(0, 255, 0, 122), "STATUS :- "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0, 255, 0, 122), fmt);
	//	ImGui::Text(STATUS.c_str());
		ImGui::TextColored(ImVec4(0, 255, 0, 122), "EXPIRY :- "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(0, 255, 0, 122), bypass);
		ImGui::EndTabItem();
			ImGui::End();

	}

				}