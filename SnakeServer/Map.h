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

void inicialMap(MAP * map );

boolean checkPos(MAP * map, DWORD pos);

boolean setItem(MAP * map, DWORD pos);

int convertCoordenatesIntoPos(MAP * map ,int linhas, int colunas);

