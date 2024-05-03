#pragma once
#ifndef FILES_H
#define FILES_H

#include <Windows.h>
#include <CommCtrl.h>

typedef struct
{
    LPWSTR fname;
    LPWSTR floc;
    LPWSTR fdir;
    HWND fHandle;
    HTREEITEM fTreeHandle;
} EXDev;

EXDev newDirectory(LPWSTR name, LPWSTR dir);
void closeDevices(EXDev *d);
EXDev newTXTFile(LPWSTR dir, LPWSTR name);
EXDev newFile(LPWSTR dir, LPWSTR name);
int saveFile(EXDev *d, HWND hTextbox);
EXDev openFile(LPWSTR dir, LPWSTR name, HWND hTextbox);

#endif