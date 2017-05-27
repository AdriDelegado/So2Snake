#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#include <tchar.h>

typedef struct {
	DWORD snake_id;
	char direction;
	DWORD player_id;
	DWORD size;
	DWORD speed;
	DWORD coordinate_x;
	DWORD coordinate_y;
}SNAKE;