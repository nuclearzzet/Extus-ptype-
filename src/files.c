#include "includes/files.h"
#include <stdio.h>

// Directory Operations
// creates a directory
EXDev newDirectory(LPWSTR name, LPWSTR dir)
{
    EXDev d = {0};
    d.fname = (LPWSTR)LocalAlloc(LPTR, ((lstrlenW(name) * 2) + 2));
    d.fdir = (LPWSTR)LocalAlloc(LPTR, ((lstrlenW(dir) * 2) + 2));
    d.floc = (LPWSTR)LocalAlloc(LPTR, ((lstrlenW(name) * 2) + (lstrlenW(dir) * 2) + 2));

    lstrcpyW(d.fdir, dir);
    lstrcpyW(d.fname, name);
    lstrcpyW(d.floc, d.fdir);
    lstrcatW(d.floc, d.fname);
    if (CreateDirectoryW(d.floc, NULL) == 0)
    {
        return d;
    }

    return d;
}

// Deletes a directory
void deleteDirectory(EXDev d)
{
}

// FILE OPERATION
// creates TXT file
EXDev newTXTFile(LPWSTR dir, LPWSTR name)
{
    EXDev f = {0};
    f.fdir = (LPWSTR)LocalAlloc(LPTR, ((lstrlenW(name) * 2) + 2));
    f.fname = (LPWSTR)LocalAlloc(LPTR, ((lstrlenW(dir) * 2) + 2));
    f.floc = (LPWSTR)LocalAlloc(LPTR, ((lstrlenW(name) * 2) + (lstrlenW(dir) * 2) + 2));

    lstrcpyW(f.fdir, dir);
    lstrcpyW(f.fname, name);

    lstrcpyW(f.floc, f.fdir);
    lstrcatW(f.floc, f.fname);

    f.fHandle = CreateFileW(
        f.floc,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    return f;
}

EXDev newFile(LPWSTR dir, LPWSTR name)
{
    EXDev f = {0};

    f.fdir = (LPWSTR)LocalAlloc(LPTR, ((lstrlenW(name) * 2) + 2));
    f.fname = (LPWSTR)LocalAlloc(LPTR, ((lstrlenW(dir) * 2) + 2));
    f.floc = (LPWSTR)LocalAlloc(LPTR, ((lstrlenW(name) * 2) + (lstrlenW(dir) * 2) + 2));

    lstrcpyW(f.fdir, dir);
    lstrcpyW(f.fname, name);

    lstrcpyW(f.floc, f.fdir);
    lstrcatW(f.floc, f.fname);

    f.fHandle = CreateFileW(
        f.floc,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    return f;
}

int saveFile(EXDev *d, HWND hTextBox)
{
    UINT l_buff = GetWindowTextLengthW(hTextBox);
    LPWSTR buff = (LPWSTR)LocalAlloc(LPTR, (l_buff * 2) + 2);
    if (GetWindowTextW(hTextBox, buff, l_buff + 1) == 0)
    {
        return -1;
    }

    WriteFile(d->fHandle,
              buff,
              (l_buff * 2),
              NULL,
              NULL);

    LocalFree((void *)buff);
    closeDevices(d);

    return 0;
}

EXDev openFile(LPWSTR dir, LPWSTR name, HWND hTextBox)
{
    EXDev f = {0};

    f.fname = (LPWSTR)LocalAlloc(LPTR, (lstrlenW(name) * 2) + 2);
    f.fdir = (LPWSTR)LocalAlloc(LPTR, (lstrlenW(dir) * 2) + 2);
    f.floc = (LPWSTR)LocalAlloc(LPTR, (lstrlenW(name) * 2) + (lstrlenW(dir) * 2) + 2);

    lstrcpyW(f.fname, name);
    lstrcpyW(f.fdir, dir);
    lstrcpyW(f.floc, f.fdir);
    lstrcatW(f.floc, f.fname);

    f.fHandle = CreateFileW(
        f.floc,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    DWORD s_buff = GetFileSize(f.fHandle, NULL);
    LPWSTR buff = (LPWSTR)LocalAlloc(LPTR, s_buff);

    if (ReadFile(f.fHandle, buff, s_buff, NULL, NULL) == 0)
    {
        LocalFree((void *)buff);
        closeDevices(&f);
        return f;
    }

    if (SetWindowTextW(hTextBox, buff) == 0)
    {
        LocalFree((void *)buff);
        closeDevices(&f);
        return f;
    }

    return f;
}

void closeDevices(EXDev *d)
{
    LocalFree(d->fname);
    LocalFree(d->fdir);
    LocalFree(d->floc);
}
