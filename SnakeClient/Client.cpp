#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include "menus.h"
#include "MsgPipe.h"
#define BUFSIZE 4096

//Cabe�alho das fun��es
bool verifyIfFirst(HANDLE hPipe, TCHAR * p);
//MSGPIPECLIENTE *menuPrincipal(MSGPIPECLIENTE *msg);
MSGPIPECLIENTE *iniciaMenssagem(MSGPIPECLIENTE *msg);

int _tmain(int argc, TCHAR *argv[]) {

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif

	HANDLE hPipe;
	TCHAR chBuf[BUFSIZE];
	TCHAR lpvMessage[BUFSIZE];
	BOOL fSucess = FALSE;
	DWORD cbRead, cbToWrite, cbWritten, dwMode;
	LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\pipeSnake");
	MSGPIPECLIENTE *msg = (MSGPIPECLIENTE*)malloc(sizeof(MSGPIPECLIENTE));
	
	msg = iniciaMenssagem(msg);
	
	if (!WaitNamedPipe(lpszPipename, NMPWAIT_WAIT_FOREVER)) {
		_tprintf(TEXT("[Information] Connecting.. '%s'\n"), lpszPipename);
		exit(-1);
	}

	//Tenta ocupar uma instancia no pipe
	while (TRUE) {

		hPipe = CreateFile(
			lpszPipename,		// pipe name
			GENERIC_READ |		//acesso read and write, como est� o server
			GENERIC_WRITE,
			0,					//sem partilha
			NULL,				// default security
			OPEN_EXISTING,		//abrir um pipe se j� existir
			0,					//atributos default
			NULL);				//sem template

								//se correu tudo bem, o handle j� existe e sai o ciclo
		if (hPipe != INVALID_HANDLE_VALUE)
			break;

		//caso seja um erro diferente de todas as instancias estarem ocupados � pq algo correu mal na cria��po e sai do prog
		if (GetLastError() != ERROR_PIPE_BUSY) {
			_tprintf(TEXT("N�o foi possivel abrir o pipe. Erro = %d\n"), GetLastError());
			return -1;
		}

		//Se passou tudo o resto, � pq todas as 255 instancias do pipe est�o ocupadas
		// a solu��o � esperar que alguma se torne livre e tentar novamente
		//aguarda 20sec

		if (!WaitNamedPipe(lpszPipename, 20000)) {
			_tprintf(TEXT("Esperei 20seg, mas n�o existe ainda nenhuma livre. Saindo...\n"));
			return -1;
		}
	} //termina o ciclo que trata a instancia

	dwMode = PIPE_READMODE_MESSAGE;
	fSucess = SetNamedPipeHandleState(
		hPipe,
		&dwMode,
		NULL,
		NULL);

	if (!fSucess) {
		_tprintf(TEXT("SetNamedPipeHandleState Falhou. Erro = %d\n"), GetLastError());
		return -1;
	}



	

	msg = menuPrincipal(msg);


	fSucess = WriteFile(
		hPipe, //handle do pipe
		msg, //mensagem
		sizeof(MSGPIPECLIENTE), //comprimento da mensagem
		&cbWritten, //ptr para guardar numero de bytes escritos
		NULL);

	if (!fSucess) {
		_tprintf(TEXT("WriteFile Falhou. Erro = %d\n"), GetLastError());
		return -1;
	}

	_tprintf(TEXT("Jogo configurado, a aguardar resposta por parte do servidor!\n\n"));

	//Verifica se o Servidor j� tem algum Cliente Registado
	//if (verifyIfFirst(hPipe, TEXT("first")) == true) {
	//	//Configura a partida se for o primeiro Cliente

	//}

	//Configura os bot�es com o quais vai jogar
	//
	//
	//

	while (TRUE) {

		_tprintf(TEXT("MENSAGEM TO SERVER -> "));
		_tscanf_s(TEXT("%s"), lpvMessage, BUFSIZE);
		//chegou aqui, est� tudo pronto para enviar mensagem para o servidor

		cbToWrite = (_tcslen(lpvMessage) + 1) * sizeof(TCHAR);
		_tprintf(TEXT("A enviar %d bytes: \"%s\"\n"), cbToWrite, lpvMessage);

		//Envia a mensagem para o servidor
		fSucess = WriteFile(
			hPipe, //handle do pipe
			lpvMessage, //mensagem
			cbToWrite, //comprimento da mensagem
			&cbWritten, //ptr para guardar numero de bytes escritos
			NULL);

		if (!fSucess) {
			_tprintf(TEXT("WriteFile Falhou. Erro = %d\n"), GetLastError());
			return -1;
		}

		_tprintf(TEXT("Mensagem Enviada, a aguardar resposta por parte do servidor!\n\n"));

		do {
			fSucess = ReadFile(
				hPipe, //nome do pipe
				chBuf,//buffer para recever a mensagem
				BUFSIZE * sizeof(TCHAR), //tamanho de bytes a ler
				&cbRead, // ptr onde guardar o numero de bytes lidos
				NULL); //not overlapped

			if (!fSucess && GetLastError() != ERROR_MORE_DATA)
				break;

			_tprintf(TEXT("Recebido do servidor: %s\n"), chBuf);
			if (_tcscmp(chBuf, TEXT("sair")) == 0) {
				return -1;
			}

		} while (!fSucess); //Continua at� ainda ter dados para ler

		if (!fSucess) {
			_tprintf(TEXT("ReadFile Falhou. Erro = %d\n"), GetLastError());
			return -1;
		}
	}

	//Fecha o pipe de comunica��o
	CloseHandle(hPipe);

	return 0;
}


bool verifyIfFirst(HANDLE hPipe, TCHAR * p) {
	TCHAR chBuf[BUFSIZE];
	TCHAR lpvMessage[BUFSIZE];
	BOOL fSucess = FALSE;
	DWORD cbRead, cbToWrite, cbWritten;

	_tcscpy(lpvMessage, p);
	_tprintf(TEXT("Copiei a mensagem: %s\n"), p);
	cbToWrite = (_tcslen(lpvMessage) + 1) * sizeof(TCHAR);
	_tprintf(TEXT("A enviar %d bytes: \"%s\"\n"), cbToWrite, lpvMessage);

	//Envia a mensagem para o servidor
	fSucess = WriteFile(
		hPipe, //handle do pipe
		lpvMessage, //mensagem
		cbToWrite, //comprimento da mensagem
		&cbWritten, //ptr para guardar numero de bytes escritos
		NULL);

	if (!fSucess) {
		_tprintf(TEXT("WriteFile Falhou. Erro = %d\n"), GetLastError());
		return -1;
	}

	_tprintf(TEXT("Mensagem Enviada, a aguardar resposta por parte do servidor!\n\n"));

	do {
		fSucess = ReadFile(
			hPipe, //nome do pipe
			chBuf,//buffer para recever a mensagem
			BUFSIZE * sizeof(TCHAR), //tamanho de bytes a ler
			&cbRead, // ptr onde guardar o numero de bytes lidos
			NULL); //not overlapped

		if (!fSucess && GetLastError() != ERROR_MORE_DATA)
			break;

		_tprintf(TEXT("Recebido do servidor: %s\n"), chBuf);
		//verifica se � o primeiro
		if (_tcscmp(chBuf, TEXT("sim")) == 0) {
			return true;
		}

	} while (!fSucess); //Continua at� ainda ter dados para ler

	if (!fSucess) {
		_tprintf(TEXT("ReadFile Falhou. Erro = %d\n"), GetLastError());
		return -1;
	}
}


MSGPIPECLIENTE *iniciaMenssagem(MSGPIPECLIENTE *msg) {


	GAME *auxGame = (GAME*)malloc(sizeof(GAME));
	PLAYER *auxPalyer = (PLAYER*)malloc(sizeof(PLAYER));
	MAP *auxMap = (MAP*)malloc(sizeof(MAP));

	auxGame->gameExist = FALSE;

	//TODO: ver como � o mapa com o FOCA 
	msg->hPipe = INVALID_HANDLE_VALUE;
	msg->lpszPipename = TEXT("\\\\.\\pipe\\pipeSnake");
	msg->dwThreadID = 0;
	msg->hThread = 0;
	msg->game = auxGame;
	msg->map = auxMap;
	msg->player = auxPalyer;

	return msg;

}
//MSGPIPECLIENTE *menuPrincipal(MSGPIPECLIENTE *msg) {
//
//	int v = 0;
//
//	_tprintf(TEXT("BEM VINDO\n"));
//do {
//	_tprintf(TEXT("\t\tSingle Player\t-1-\n"));
//	_tprintf(TEXT("\t\tMultiPlayer\t-2-\n"));
//	
//	_tscanf_s(TEXT("%d"), &v);
//	} while (v != 1 && v != 2);
//
//	if (v == 1) {
//	//	msg = menuSettingUpGame(1, msg);
//		_tprintf(TEXT("OI\n"));
//	}
//	else if (v == 2) {
//		_tprintf(TEXT("XAU\n"));
//		//msg = menuMultiplayer1(msg);
//	}
//
//	return msg;
//}