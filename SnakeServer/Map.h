#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#include <tchar.h>
#include <vector>
#include <iostream>
#include "MsgPipe.h"
using namespace std;


typedef struct {
	DWORD height;
	DWORD width;
	DWORD **tiles;
	vector<int> map;
}MAP;

void inicialMap(MAP * mapa, MSGPIPE * mapC);

boolean checkPos(DWORD pos);

boolean setItem(DWORD pos);

int convertCoordenatesIntoPos(int linhas, int colunas);