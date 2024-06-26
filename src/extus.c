#include "includes/defs.h"
#include "includes/files.h"
#include "includes/themes.h"
#include <stdio.h>

extern LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern LRESULT CALLBACK PopupProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern int loadMenu();
extern int makeMenu(enum MENU_IDS id, LPWSTR name, HMENU handle, HMENU subHandle);
extern void windowResize();
extern int createTextBox();
extern int createSideBar();
HTREEITEM insertIntoSideBar(LPWSTR name, HTREEITEM hPar);
extern int createTabView();
extern void insertIntoTabView(EXDev *dev, UINT ID);
extern void createInputPopUp();
extern int getDirsFiles();

EXDev openFiles[25];

/// @brief The main UNICODE function
/// @param hInstance The identifier windows uses to identify the executable
/// @param hPrevInstance The identifer used in 16bit-windows now always 0
/// @param pCmdLine The commandLine arguments
/// @param nCmdShow The state of the window ie minimised, maximised etc
/// @return
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"EXTUS V0.1 Prototype";

    WNDCLASS wc = {0};

    wc.lpszClassName = CLASS_NAME;
    wc.hInstance = hInstance; // HINSTNACE
    wc.lpfnWndProc = WindowProcedure;

    RegisterClass(&wc);

    GUI.hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"EXTUS V0.1 Prototype",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1080, 900,
        NULL,
        NULL,
        hInstance,
        NULL);

    if (GUI.hwnd == NULL)
    {
        return -1;
    }

    ShowWindow(GUI.hwnd, nCmdShow);

    MSG msg = {0};

    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

/// @brief The Procedure of the main Window
/// @param hwnd The handle to the main window
/// @param uMsg The message windows sends to the procedure
/// @param wParam a unsigned integer pointer
/// @param lParam a long pointer
/// @return
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
    {
        GUI.hwnd = hwnd;
        if (loadMenu() == -1)
            printf("Error loading menu");

        // HFONT font = AddFontResourceExW(
        //     L"consolat.ttf\0",
        //     FR_PRIVATE,
        //     0);
        // SendMessageW(GUI.hwnd, WM_SETFONT, (WPARAM)font, TRUE);

        if (createTextBox() == -1)
            printf("TextBox creations failed");

        if (createSideBar() == -1)
            printf("Side Bar creation failes");

        if (createTabView() == -1)
            printf("Tab View creation failed");

        break;
    }
    case WM_SIZE:
    {
        windowResize();
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_CLOSE:
    {
        if (MessageBox(GUI.hwnd, L"Do You Want to Quit?", L"Extus v0.1", MB_YESNO) == IDYES)
        {
            for (int i = 0; i < 25; i++)
            {
                if (openFiles[i].fHandle != NULL)
                {
                    closeDevices(&openFiles[i]);
                }
            }
            DestroyWindow(GUI.hwnd);
        }
        return 0;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(GUI.hwnd, &ps);

        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        light(GUI.hTextBox, GUI.hTabControl, GUI.hSideBar);

        EndPaint(GUI.hwnd, &ps);
    }
    case WM_CTLCOLOREDIT:
    {
        SetBkColor((HDC)wParam, RGB(229, 219, 219));

        return (LRESULT)CreateSolidBrush(RGB(229, 219, 219));
    }
    case WM_COMMAND:
    {
        UINT id = LOWORD(wParam);

        switch (id)
        {
        case NEW_DIR:
        {
            EXDev d = newDirectory(L"test", L".\\");

            break;
        }

        case NEW_TXT_FILE:
        {
            EXDev f = newTXTFile(L".\\test\\", L"New.txt");
            openFiles[0] = f;

            break;
        }

        case NEW_FILE:
        {
            // EXDev f = newFile(L".\\test\\", L"main.c");
            // openFiles[1] = f;
            break;
        }

        case OPEN_FILE:
        {
            EXDev f = openFile(L".\\test\\", L"New.txt", GUI.hTextBox);
            openFiles[0] = f;

            break;
        }
        case SAVE_FILE:
        {
            saveFile(&openFiles[0], GUI.hTextBox);
            break;
        }

        case CLOSE:
            SendMessageW(hwnd, WM_CLOSE, wParam, lParam);
            break;
        }
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

/// @brief A function to make the UI responsive on resizing
void windowResize()
{
    RECT rM = {0};
    GetClientRect(GUI.hwnd, &rM);

    SetWindowPos(GUI.hTextBox,
                 NULL,
                 0, 0, rM.right - 250, rM.bottom - 25,
                 SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW);

    SetWindowPos(GUI.hTabControl,
                 NULL,
                 300, 0, rM.right - 250, 25,
                 SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW);
}

/// @brief To Load the Menu called when WM_CREATE message is send to the Window Procedure
/// @return 0 if it suceeds || -1 if it fails
int loadMenu()
{
    GUI.Menu.hMenu = CreateMenu();
    GUI.Menu.hFileSubMenu = CreatePopupMenu();
    GUI.Menu.hEditSubMenu = CreatePopupMenu();
    GUI.Menu.hSettingsSubMenu = CreatePopupMenu();

    makeMenu(sFILE, L"&File", GUI.Menu.hMenu, GUI.Menu.hFileSubMenu);
    makeMenu(sEDIT, L"&Edit", GUI.Menu.hMenu, GUI.Menu.hEditSubMenu);
    makeMenu(sSETTINGS, L"&Settings", GUI.Menu.hMenu, GUI.Menu.hSettingsSubMenu);
    SetMenu(GUI.hwnd, GUI.Menu.hMenu);
    DrawMenuBar(GUI.hwnd);

    // To add the items of the File popup menu
    LPWSTR subFiles[8] = {L"&New txt File", L"&New File", L"&New Folder", L"&Open File", L"&Open Folder", L"&Save File", L"&Save All", L"&Close"};
    for (int i = 0; i < 8; i++)
    {
        if (makeMenu(NEW_TXT_FILE + i, subFiles[i], GUI.Menu.hFileSubMenu, 0) == -1)
            return -1;
    }

    // To add the items of Settinfs popup menu
    LPWSTR subSettings[5] = {L"&Themes", L"&Layout", L"&Keyboard Shortcuts", L"&Intelli-Sense", L"&Settings"};
    for (int i = 0; i < 5; i++)
    {
        if (makeMenu(THEMES + i, subSettings[i], GUI.Menu.hSettingsSubMenu, 0) == -1)
            return -1;
    }

    return 0;
}

/// @brief To programmatically add the window
/// @param id The id of the menu item
/// @param name The name of the menu item
/// @param handle The handle to the menu
/// @param subHandle The handle to the popup menu
/// @return Returns 0 if no errors otherwise -1
int makeMenu(UINT id, LPTSTR name, HMENU handle, HMENU subHandle)
{
    MENUITEMINFO m;
    LPTSTR buff;
    m.cbSize = sizeof(MENUITEMINFO);
    m.fMask = MIIM_SUBMENU | MIIM_TYPE | MIIM_ID;
    m.fType = MFT_STRING;
    m.wID = id;
    m.dwTypeData = name;
    m.hSubMenu = subHandle;

    if (InsertMenuItemW(handle, id, TRUE, &m) == 0)
    {
        return -1;
    }

    return 0;
}

int getDirsFiles()
{
    // WIN32_FIND_DATAW fData;

    // HANDLE ff = FindFirstFileExW(
    //     L".\\src",
    //     FindExInfoBasic,
    //     &fData,
    //     FindExSearchLimitToDirectories,
    //     NULL,
    //     FIND_FIRST_EX_LARGE_FETCH);

    // printf("%s", fData.cFileName);

    LPWSTR rootDir = NULL;

    GetCurrentDirectory(MAX_PATH, rootDir);

    HTREEITEM root = insertIntoSideBar(rootDir, NULL);
}

/// @brief To create a text box for editing files
/// @return 0 if suceeds || -1 if failes
int createTextBox()
{
    RECT r = {0};
    GetClientRect(GUI.hwnd, &r);

    GUI.hTextBox = NULL;

    GUI.hTextBox = CreateWindowEx(
        0,
        L"EDIT",
        NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE,
        250, 25, r.right - 250, r.bottom - 25,
        GUI.hwnd,
        NULL,
        NULL,
        NULL);

    if (GUI.hTextBox == NULL)
        return -1;

    return 0;
}

/// @brief To create a tree view for file exploration
/// @return If no errors returns 0 || -1
int createSideBar()
{
    RECT rM = {0};
    RECT rTB = {0};
    GetClientRect(GUI.hwnd, &rM);
    GetClientRect(GUI.hwnd, &rTB);

    InitCommonControls();

    GUI.hSideBar = NULL;

    GUI.hSideBar = CreateWindowExW(
        WS_EX_TOOLWINDOW | WS_EX_CLIENTEDGE,
        WC_TREEVIEWW,
        NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER | TVS_HASLINES,
        0, 0, 250, rM.bottom,
        GUI.hwnd,
        NULL,
        NULL,
        NULL);

    if (GUI.hSideBar == NULL)
        return -1;
    return 0;
}

/// @brief A function to input Items to the sidebar
/// @param dev A pointer of type EXDev
/// @param hPar The parent of a tree item (NULL)
HTREEITEM insertIntoSideBar(LPWSTR name, HTREEITEM hPar)
{
    TVITEMW tvi = {0};
    if (hPar != NULL)
        tvi.mask = TVIF_TEXT | TVIF_CHILDREN;
    else
        tvi.mask = TVIF_TEXT;
    tvi.pszText = name;
    tvi.cchTextMax = lstrlenW(name);

    TVINSERTSTRUCT tvis = {0};
    tvis.hParent = hPar;
    tvis.hInsertAfter = TVI_LAST;
    tvis.item = tvi;

    return TreeView_InsertItem(GUI.hSideBar, &tvis);
}

/// @brief A Function to create the tab view
/// @return 0 if suceeds || -1 if failes
int createTabView()
{
    INITCOMMONCONTROLSEX iccs = {0};
    RECT rM = {0};
    GetClientRect(GUI.hwnd, &rM);
    GUI.hTabControl = NULL;
    iccs.dwSize = sizeof(INITCOMMONCONTROLSEX);
    iccs.dwICC = ICC_TAB_CLASSES;

    InitCommonControlsEx(&iccs);

    GUI.hTabControl = CreateWindowExW(
        0,
        WC_TABCONTROLW,
        NULL,
        WS_CLIPSIBLINGS | WS_CHILD | WS_VISIBLE,
        250, 0, rM.right - 250, 25,
        GUI.hwnd,
        NULL,
        NULL,
        NULL);

    if (GUI.hTabControl == NULL)
        return -1;
    return 0;
}

/// @brief A function to add items to Tab-Control
/// @param dev A pointer of type EXDev
/// @param id A unsigned integer to identify the item
void insertIntoTabView(EXDev *dev, UINT id)
{
    TCITEMW tciw = {0};

    tciw.mask = TCIF_TEXT | TCIF_IMAGE;
    tciw.iImage = -1;
    tciw.cchTextMax = lstrlenW(dev->fname);
    tciw.pszText = dev->fname;

    TabCtrl_InsertItem(GUI.hTabControl, id, &tciw);
}

void createInputPopUp()
{
    Popup.dlgTemp.style = WS_CAPTION | WS_SYSMENU | DS_MODALFRAME | DS_CENTER | WS_VISIBLE | WS_EX_CONTROLPARENT;
    Popup.dlgTemp.dwExtendedStyle = 0;
    Popup.dlgTemp.x = 150;
    Popup.dlgTemp.y = 150;
    Popup.dlgTemp.cx = 250;
    Popup.dlgTemp.cy = 50;

    DialogBoxIndirectW(GetModuleHandleW(NULL), &Popup.dlgTemp, NULL, PopupProcedure);
}

/// @brief Window Procedure of popup
/// @param hwnd
/// @param uMsg
/// @param wParam
/// @param lParam
/// @return
LRESULT CALLBACK PopupProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
        Popup.hPathText = CreateWindowExW(
            0,
            L"EDIT",
            L"Path",
            WS_BORDER | WS_VISIBLE | WS_CHILD,
            50, 30, 162.5, 20,
            hwnd,
            NULL,
            NULL,
            NULL);

        Popup.hNameText = CreateWindowExW(
            0,
            L"EDIT",
            L"Name",
            WS_BORDER | WS_VISIBLE | WS_CHILD,
            220, 30, 162.5, 20,
            hwnd,
            NULL,
            NULL,
            NULL);

        Popup.hButton = CreateWindowExW(
            0,
            L"BUTTON",
            L"Submit",
            WS_BORDER | WS_CHILD | WS_VISIBLE,
            375, 30, 75, 20,
            hwnd,
            NULL,
            NULL,
            NULL);

        break;
    }
    case WM_COMMAND:
    {
        switch (HIWORD(wParam))
        {
        case BN_CLICKED:
        {

            LPWSTR pbuff;
            LPWSTR nbuff;
            UINT l_pbuff = GetWindowTextLengthW(Popup.hPathText);
            UINT l_nbuff = GetWindowTextLengthW(Popup.hNameText);
            pbuff = (LPWSTR)LocalAlloc(LPTR, (l_pbuff * 2) + 2);
            nbuff = (LPWSTR)LocalAlloc(LPTR, (l_nbuff * 2) + 2);
            GetWindowTextW(Popup.hPathText, pbuff, l_pbuff + 1);
            GetWindowTextW(Popup.hNameText, nbuff, l_nbuff + 1);
            newFile(pbuff, nbuff);

            LocalFree((void *)pbuff);
            LocalFree((void *)nbuff);
            SendMessage(hwnd, WM_DESTROY, wParam, lParam);

            break;
        }
        }
        break;
    }
    case WM_SIZE:
        break;

    case WM_DESTROY:
        EndDialog(hwnd, 0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
