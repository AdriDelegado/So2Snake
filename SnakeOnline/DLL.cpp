// DLL.cpp : Defines the exported functions for the DLL application.  
// Compile by using: cl /EHsc /DMATHLIBRARY_EXPORTS /LD MathLibrary.cpp  

  
#include "DLL.h"  

HANDLE hMapFile;
GAME * game = (GAME *)malloc(sizeof(GAME));


BOOL CommandVerifier(TCHAR comando[])
{
	_tprintf(TEXT("DLL - %s\n"), comando);
	//Comando para verificar se o cliente pretende sair
	//if (_tcscmp(comando, TEXT("sair")) == 0)
	//return TRUE;
	//Comando para verificar se é o primeiro jogador
	if (_tcscmp(comando, TEXT("first")) == 0) {
		if (game->actual_players == 0) return TRUE;
	}
	return FALSE;
}

BOOL SharedMemmoryInicializer() {

	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		GAMEBUFSIZE,
		szNAME);

	if (hMapFile == NULL) {
		_tprintf(TEXT(
			"Houve azar na memoria partilhada ( erro %d).\n"), GetLastError());
		return 1;
	}
	_tprintf(TEXT("Vou Criar a View da memoria Partilhada.\n"));

	game = (GAME*)MapViewOfFile(
		hMapFile,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		GAMEBUFSIZE);
	if (game == NULL) {
		_tprintf(TEXT(
			"A view da memoria partilhada nao foi criada ( erro: %d). \n"), GetLastError());
		CloseHandle(hMapFile);
		return 1;
	}
}

BOOL SharedMemmoryCloser() {

	//Faz unmap da memoria partilhada a estrutura de jogo, chamar isto no fim do jogo ou quando algum problema surgir.
	UnmapViewOfFile(game);
	//Tira o handle;
	CloseHandle(hMapFile);

	return true;
}

BOOL PlayerAdder(TCHAR nome[]) {

	if (nome == NULL) {
		_tprintf(TEXT("DLL - Estrutura de Player Nula, a sair da função."));
		return FALSE;
	}
	
	PLAYER * newPlayer = (PLAYER *)malloc(sizeof(PLAYER));

	_tcscpy(newPlayer->player_name, nome);
	game->playerArray[game->actual_players] = *newPlayer;
	game->actual_players++;

	return TRUE;
}

int PlayerList() {
	for (int i = 0; i < game->actual_players; i++) {
		_tprintf(TEXT("DLL - Jogadores incritos %d: %s\n"), i, game->playerArray[i].player_name);
	}

	return game->actual_players;
}

void GameConfigurer(MSGPIPESERVIDOR  msgpipe) {


}
