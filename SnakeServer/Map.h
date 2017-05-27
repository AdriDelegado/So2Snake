#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#include <tchar.h>

typedef struct {
	DWORD height;
	DWORD width;
	DWORD **tiles;
}MAP;