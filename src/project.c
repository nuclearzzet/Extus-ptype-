#include "includes/project.h"

int newProject(LPWSTR name, LPWSTR language)
{
    Project.rootDir = newDirectory(L".\\", name);
    Project.lang = (LPWSTR)LocalAlloc(LPTR, lstrlenW(language));
}