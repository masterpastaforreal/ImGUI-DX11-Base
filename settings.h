#pragma once


// Global settings for the project

// Set console settings
namespace consolesize
{
    inline int width = 40; // columns
    inline int height = 20;  // rows
}
inline bool changeconsolesize = true;
inline bool hidetitlebar = false;
inline bool hidescrollbar = true;
inline bool disableresize = true;


// Set ImGUI settings
namespace windowsize
{
    inline float width = 400;
    inline float height = 400;
}