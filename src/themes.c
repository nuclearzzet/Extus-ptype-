#include "includes/themes.h"

extern int syntaxLight();
extern int syntaxDark();

/// @brief A function to set window as dark theme
/// @param TBox Handle to TextBox
/// @param TCtrl Handle To Tab-Control
/// @param SBar Handle to SideBar
/// @return 0 if no errors || -1 if failes
int dark(HWND TBox, HWND TCtrl, HWND SBar)
{
    HBRUSH cSideBarCol = CreateSolidBrush(RGB(31, 33, 55));
    HBRUSH cTabCtrlCol = CreateSolidBrush(RGB(2, 4, 33));
    HBRUSH cTextBoxCol = CreateSolidBrush(RGB(50, 55, 83));

    PAINTSTRUCT p_TBox, p_TCtrl, p_SBar;
    HDC hdc_TBox = BeginPaint(TBox, &p_TBox);
    HDC hdc_TCtrl = BeginPaint(TCtrl, &p_TCtrl);
    HDC hdc_SBar = BeginPaint(SBar, &p_SBar);

    FillRect(hdc_TBox, &p_TBox.rcPaint, cTextBoxCol);
    FillRect(hdc_TCtrl, &p_TCtrl.rcPaint, cTabCtrlCol);
    FillRect(hdc_SBar, &p_SBar.rcPaint, cSideBarCol);

    if (EndPaint(TBox, &p_TBox) || EndPaint(TCtrl, &p_TCtrl) || EndPaint(SBar, &p_SBar) == 0)
    {
        return -1;
    }

    else
    {
        DeleteObject(cSideBarCol);
        DeleteObject(cTabCtrlCol);
        DeleteObject(cTextBoxCol);

        DeleteDC(hdc_SBar);
        DeleteDC(hdc_TCtrl);
        DeleteDC(hdc_TBox);

        return 0;
    }
}

int light(HWND TBox, HWND TCtrl, HWND SBar)
{
    HBRUSH cSideBarCol = CreateSolidBrush(RGB(236, 213, 213));
    HBRUSH cTabCtrlCol = CreateSolidBrush(RGB(208, 170, 170));
    HBRUSH cTextBoxCol = CreateSolidBrush(RGB(229, 219, 219));

    PAINTSTRUCT p_TBox, p_TCtrl, p_SBar;
    HDC hdc_TBox = BeginPaint(TBox, &p_TBox);
    HDC hdc_TCtrl = BeginPaint(TCtrl, &p_TCtrl);
    HDC hdc_SBar = BeginPaint(SBar, &p_SBar);

    FillRect(hdc_TBox, &p_TBox.rcPaint, cTextBoxCol);
    FillRect(hdc_TCtrl, &p_TCtrl.rcPaint, cTabCtrlCol);
    FillRect(hdc_SBar, &p_SBar.rcPaint, cSideBarCol);

    if (EndPaint(TBox, &p_TBox) || EndPaint(TCtrl, &p_TCtrl) || EndPaint(SBar, &p_SBar) == 0)
    {
        return -1;
    }

    else
    {
        DeleteObject(cSideBarCol);
        DeleteObject(cTabCtrlCol);
        DeleteObject(cTextBoxCol);

        DeleteDC(hdc_SBar);
        DeleteDC(hdc_TCtrl);
        DeleteDC(hdc_TBox);

        return 0;
    }
}

int syntaxLight()
{
}

int syntaxDark()
{
}