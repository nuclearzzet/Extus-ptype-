#pragma once
#ifndef PROJECT_H
#define PROJECT_H

#include <Windows.h>
#include "files.h"

struct
{
    EXDev rootDir;
    LPWSTR lang;
    LPWSTR git;
} Project;

#endif