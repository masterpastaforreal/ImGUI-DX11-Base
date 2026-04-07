#pragma once


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
