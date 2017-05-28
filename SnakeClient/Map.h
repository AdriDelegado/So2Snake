#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#include <tchar.h>
#include <vector>
#include <iostream>

using namespace std;


typedef struct {
	DWORD height;
	DWORD width;
	DWORD **tiles;
	vector<int> map;
}MAP;

void inicialMap(DWORD height, DWORD width);

boolean checkPos(DWORD pos);

boolean setItem(DWORD pos);

int convertCoordenatesIntoPos(int linhas, int colunas);