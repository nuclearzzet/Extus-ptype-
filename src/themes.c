#include "includes/themes.h"

extern int syntaxLight();
extern int syntaxDark();

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
}

int light()
{
}

int syntaxLight()
{
}

int syntaxDark()
{
}