#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#include <tchar.h>

typedef struct {
	DWORD item_id;

	char item_name[20];
	//coordinates where the item is placed 
	DWORD coordinate_x;
	DWORD coordinate_y;
}ITEM;

