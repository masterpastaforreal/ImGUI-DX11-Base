#include <iostream>
#include <Windows.h>
#include <d3d11.h>
#include <tchar.h>

#include "Protections/skCrypter.h"// Storing strings in plain binary can be unsafe - Encrypts Strings
#include "settings.h"// Global settings header file

// ImGUI includes
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_win32.h"
#include "ImGUI/imgui_impl_dx11.h"
#include "ImGUI/imconfig.h"
#include "ImGUI/imgui_internal.h"
#include "ImGUI/imstb_rectpack.h"
#include "ImGUI/imstb_truetype.h"
#include "ImGUI/imstb_textedit.h"

#pragma comment(lib, "d3d11.lib")

// Forward declare GUI render function
void RenderGUI();

// Forward declares for D3D and Win32 helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupRenderTarget();
void CreateRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

// Create the render target for the swap chain
void CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer = NULL;
	g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (pBackBuffer)
	{
		g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
		pBackBuffer->Release();
	}
}

void CleanupRenderTarget()
{
	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

// Helper to create D3D device and swap chain
bool CreateDeviceD3D(HWND hWnd)
{
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
		return false;

	CreateRenderTarget();
	return true;
}

// Win32 message handler required by ImGui_ImplWin32
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			CleanupRenderTarget();
			g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			CreateRenderTarget();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    // Create application window
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, TEXT("gui_window"), NULL };
	RegisterClassEx(&wc);
	//HWND hwnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_NOACTIVATE, _T("gui_window"), NULL, WS_POPUP, 0, 0, 3840, 2160, NULL, NULL, wc.hInstance, NULL);// 4K
	HWND hwnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_NOACTIVATE, _T("gui_window"), NULL, WS_POPUP, 0, 0, 2560, 1440, NULL, NULL, wc.hInstance, NULL);// 1080p
	//HWND hwnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_NOACTIVATE, _T("gui_window"), NULL, WS_POPUP, 0, 0, 1920, 1080, NULL, NULL, wc.hInstance, NULL);// 1080p

	SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, ULW_COLORKEY);
	// Initialize Direct3D
	if (!CreateDeviceD3D(hwnd))
	{
		CleanupDeviceD3D();
		UnregisterClass(wc.lpszClassName, wc.hInstance);
		return 1;
	}

	ShowWindow(hwnd, SW_SHOWDEFAULT);
	UpdateWindow(hwnd);

	// Setup Dear ImGui context
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = NULL; // This prevents the .ini file from ever being created or read
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

	// Main loop
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowSize(ImVec2(windowsize::width, windowsize::height));
		// Delegate actual GUI rendering here
		RenderGUI();

		ImGui::Render();
		const float clear_color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
		g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		g_pSwapChain->Present(1, 0);
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	CleanupDeviceD3D();
	UnregisterClass(wc.lpszClassName, wc.hInstance);

	return 0;
}

void SetConsoleSize(HANDLE hConsole)
{
	// Set buffer size first
	COORD size;
	size.X = consolesize::width;
	size.Y = consolesize::height;
	SetConsoleScreenBufferSize(hConsole, size);

	// Set window size
	SMALL_RECT rect;
	rect.Left = 0;
	rect.Top = 0;
	rect.Right = consolesize::width - 1;
	rect.Bottom = consolesize::height - 1;

	SetConsoleWindowInfo(hConsole, TRUE, &rect);
}

int main()
{
	SetConsoleTitleA("ImGUI Base by ioctl_1337");
	std::cout << "\n   Loading console settings";

	Sleep(500);

	HWND hwnd = GetConsoleWindow();
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	if (hidetitlebar)// Hide title bar (Hides the title bar including the minimize and exit button)
	{
		LONG style = GetWindowLong(hwnd, GWL_STYLE);
		style &= ~(WS_BORDER | WS_CAPTION | WS_THICKFRAME);
		SetWindowLong(hwnd, GWL_STYLE, style);

		SetWindowPos(hwnd, NULL, 0, 0, 0, 0,
			SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE
			| SWP_FRAMECHANGED);
	}

	if (disableresize)// Disable the ability to resize the console window
	{
		LONG style = GetWindowLong(hwnd, GWL_STYLE);
		style &= ~(WS_MAXIMIZEBOX | WS_SIZEBOX);
		SetWindowLong(hwnd, GWL_STYLE, style);
	}

	if (changeconsolesize)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleSize(hConsole);
	}

	if (hidescrollbar)// Hide scroll bar for console window
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(hConsole, &csbi);

		SHORT windowWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		SHORT windowHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

		COORD size;
		size.X = windowWidth;// Removes horizontal scrollbar
		size.Y = windowHeight;// Removes vertical scrollbar

		SetConsoleScreenBufferSize(hConsole, size);
	}

	if (disablequickeditmode)// Disable quick edit mode to prevent the console from freezing when clicking inside it
	{
		HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
		DWORD prev_mode;

		// Get the current input mode
		GetConsoleMode(hInput, &prev_mode);

		/* ENABLE_QUICK_EDIT_MODE allows mouse selection.
		   ENABLE_EXTENDED_FLAGS is required to modify Quick Edit.
		*/
		SetConsoleMode(hInput, prev_mode & ~ENABLE_QUICK_EDIT_MODE | ENABLE_EXTENDED_FLAGS);
	}

	std::cout << "\n   Rendering gui with DX11";
	WinMain(GetModuleHandle(NULL), NULL, NULL, SW_SHOWDEFAULT);
	return 0;
}