#pragma once
#include "ImGUI/imgui.h"

// Global settings for the project

// Set console settings
namespace consolesize
{
    inline int width = 45; // Columns
    inline int height = 30;  // Rows
}
inline bool changeconsolesize = true;
inline bool hidetitlebar = false;
inline bool hidescrollbar = true;
inline bool disableresize = true;
inline bool disablequickeditmode = true;


// Set ImGUI settings
namespace windowsize
{
    inline float width = 400;
    inline float height = 400;
}
namespace gui_colors
{
    inline ImVec4 text = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
    inline ImVec4 text_disabled = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);

    // Backgrounds
    inline ImVec4 window_bg = ImVec4(0.11f, 0.12f, 0.15f, 1.00f);
    inline ImVec4 child_bg = ImVec4(0.08f, 0.08f, 0.10f, 1.00f);

    // Accent Colors
    inline ImVec4 main_accent = ImVec4(0.18f, 0.67f, 0.73f, 1.00f);
    inline ImVec4 accent_hovered = ImVec4(0.25f, 0.78f, 0.85f, 1.00f);
    inline ImVec4 accent_active = ImVec4(0.18f, 0.67f, 0.73f, 0.80f);

    // Tabs
    inline ImVec4 tab = ImVec4(0.14f, 0.15f, 0.18f, 1.00f);
    inline ImVec4 tab_hovered = ImVec4(0.22f, 0.24f, 0.28f, 1.00f);
    inline ImVec4 tab_active = ImVec4(0.18f, 0.67f, 0.73f, 1.00f);
}