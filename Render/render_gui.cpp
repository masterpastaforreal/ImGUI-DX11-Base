#include <iostream>
#include <Windows.h>
#include <d3d11.h>

#include "..\Protections/skCrypter.h"// Storing strings in plain binary can be unsafe - Encrypts Strings
#include "..\settings.h"// Global settings header file

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
    DWORD window_flags = ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize;// Set the window flags (rules) the GUI must follow

    ImGui::Begin("Example GUI", NULL, window_flags);
    ImGui::Text("Hello from Dear ImGui!");

    static bool checkbox = false;
    ImGui::Checkbox("Enable", &checkbox);

    static float slider = 0.5f;
    ImGui::SliderFloat("Value", &slider, 0.0f, 1.0f);

    ImGui::End();
}