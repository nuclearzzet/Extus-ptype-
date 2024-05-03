#pragma once
#ifndef TYPES_H
#define TYPES_H

#include <Windows.h>
#include <CommCtrl.h>
#include <wingdi.h>

enum MENU_IDS
{
    // SUBMENUS
    sFILE = 1,
    sEDIT,
    sSETTINGS,

    // FILE CONTENTS
    NEW_TXT_FILE,
    NEW_FILE,
    NEW_DIR,
    OPEN_FILE,
    OPEN_DIR,
    SAVE_FILE,
    SAVE_ALL,
    CLOSE,

    // EDIT CONTENTS

    // SETTINGS CONTENTS
    THEMES,
    LAYOUT,
    KEYBOARD_SHORTCUTS,
    INTELLI_SENSE,
    SETTINGS,
};

enum PROGRAM_EVENTS
{
    eNEW_TXT_FILE = 1,
    eNEW_FILE,
    eOPEN_FILE,
};

struct
{
    HWND hwnd, hTextBox, hSideBar, hTabControl;
    HMENU hmenu, hFileSubMenu, hEditSubMenu, hSettingsSubMenu;
    BOOL isProject;
} GUI;

struct
{
    HWND hPathText, hNameText, hButton;
    DLGTEMPLATE dlgTemp;
} Popup;

#endif