#include <iostream>
#include <Windows.h>
#include <d3d11.h>

#include "..\Protections/Headers/skCrypter.h"// Storing strings in plain binary can be unsafe - Encrypts Strings
#include "..\settings.h"// Global settings header file

#include "..\Protections/Headers/anti_tamper.h"// Anti-tampering checks

// ImGUI includes
#include "..\ImGUI/imgui.h"
#include "..\ImGUI/imgui_impl_win32.h"
#include "..\ImGUI/imgui_impl_dx11.h"
#include "..\ImGUI/imconfig.h"
#include "..\ImGUI/imgui_internal.h"
#include "..\ImGUI/imstb_rectpack.h"
#include "..\ImGUI/imstb_truetype.h"
#include "..\ImGUI/imstb_textedit.h"


#pragma comment(lib, "d3d11.lib")


void RenderGUI()
{
	// Static variables persist between frames
	static double last_check_time = 0.0;
	double current_time = ImGui::GetTime();

	// Run the check every 10.0 seconds
	if (current_time - last_check_time >= 10.0)
	{
		if (!security::run_security_checks())
		{
			Beep(1000, 800);
			std::cout << "\n   [!] Security violation ERROR";
			if (security::permanently_lock_system()){
				*(int*)0 = 0;
			}
		}
		last_check_time = current_time;
	}

	static bool gui_open = true;

    ImGui::Begin("Example GUI", &gui_open, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
	ImGui::BeginTabBar("Example Tab Bar", ImGuiTabBarFlags_None);

	if (ImGui::BeginTabItem("Tab 1"))
	{
		ImGui::Text("This is tab 1");
		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Tab 2"))
	{
		ImGui::Text("This is tab 2");
		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Tab 3"))
	{
		ImGui::Text("This is tab 3");
		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Tab 4"))
	{
		ImGui::Text("This is tab 4");
		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Tab 5"))
	{
		ImGui::Text("This is tab 5");
		ImGui::EndTabItem();
	}

	ImGui::EndTabBar();
	ImGui::EndChild();

	if (!gui_open)
	{
		PostQuitMessage(0);
	}
}