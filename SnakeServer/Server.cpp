#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include "MsgPipe.h"

#define BUFSIZE 4096
#define kBufferSize 512

//DLL 
HINSTANCE hDll;
BOOL(*sharedMemory)(void); //Fun��o que incializa a memoria partilhada
BOOL(*command)(TCHAR[]); //Testa os comandos enviados pelo cliente
BOOL(*addPlayer)(TCHAR[]); //Adiciona um jogador � partida
int(*player)(); //Verifica os o nome e o numero de jogadores inscritos na partida
void(*configureGame)(MSGPIPESERVIDOR * msg); //Envia a estrutura de dados que contem todas as informa��es referentes � partida
MSGPIPESERVIDOR *iniciaMenssagem(MSGPIPESERVIDOR *msg);
DWORD WINAPI ThreadAtendeCliente(LPVOID lpvParam);
void SendClientMessage(HANDLE hPipe, TCHAR * message);
void ReceiveClientMessage(HANDLE hPipe);


int _tmain(int argc, TCHAR *argv[]) {
	BOOL fConnected = FALSE;

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif
	//Faz a liga��o h� dll
	hDll = LoadLibrary(TEXT("SnakeDLL.dll"));

	if (hDll == NULL) {
		_tprintf(TEXT("Erro a encontrar a DLL!\n "));
		return TRUE;
	}

	//Cria ponteiro para aceder � fun��o
	sharedMemory = (BOOL(*)(void)) GetProcAddress(hDll, "SharedMemmoryInicializer");

	if (sharedMemory == NULL) {
		_tprintf(TEXT("Erro a criar o ponteiro para a fun��o!\n, erro: %d"), GetLastError());
		return TRUE;
	}

	//Cria a mem�ria partilhada 
	sharedMemory();


	MSGPIPESERVIDOR *msg = (MSGPIPESERVIDOR*)malloc(sizeof(MSGPIPESERVIDOR));

	msg = iniciaMenssagem(msg);

	while (TRUE) {
		_tprintf(TEXT("SERVIDOR - Bem vindo ao servidor do Snake em Win32\n"));

		msg->hPipe = CreateNamedPipe(
			msg->lpszPipename, //nome do pipe
			PIPE_ACCESS_DUPLEX, // Pipe de acesso read and write
			PIPE_TYPE_MESSAGE | //pipe do tipo message
			PIPE_READMODE_MESSAGE | //pipe do tipo read message
			PIPE_WAIT, // pipe do tipo bloqueante
			20, // 255 instancias
			BUFSIZE, // tamanho do buffer de output
			BUFSIZE, // tamanho do buffer input
			0, // timeout
			NULL); // security params

		if (msg->hPipe == INVALID_HANDLE_VALUE) {
			_tprintf(TEXT("SERVIDOR - Erro na cria��o do NamedPipe, erro = %d\n"), GetLastError());
			return -1; //Qualquer valor diferente de 0 ou 0 representa sucesso, apenas o -1 faz o prog sair.
		}

		//createNamedPipe criado com sucesso
		_tprintf(TEXT("SERVIDOR - � espera de Clientes...\n\n"));

		//� null pq no connectednamedpipe, porque n�o v�o ser precisas opera��es de I/O Overlapsed
		fConnected = ConnectNamedPipe(msg->hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

		if (fConnected) // Se j� existe algum client connected trata-o
		{
			_tprintf(TEXT("SERVIDOR - Cliente Ligado, Criando Thread...\n"));

			//Cria a Thread para o cliente
			msg->hThread = CreateThread(
				NULL, //security params
				0, // size da pilha
				ThreadAtendeCliente, // fun��o da thread
				(LPVOID)msg->hPipe, // handle para a thread
				0, // nao suspensa
				&msg->dwThreadID); // ptr onde guarda o id da thread a criar

			if (msg->hThread == NULL) {
				_tprintf(TEXT("SERVIDOR - Erro na Cria��o da Thread. Erro = %d.\n"), GetLastError());
				return -1;
			}
			else // correu tudo bem
				CloseHandle(msg->hThread);
		}
		//Cliente nao se consegui ligar - fechar o pipe
		else
			CloseHandle(msg->hPipe);
	}
	return 0; //Fecha o server
}

DWORD WINAPI ThreadAtendeCliente(LPVOID lpvParam) {
	HANDLE hHeap = GetProcessHeap(); //aloca espa�o para o pedido no heap
	TCHAR * pchRequest = (TCHAR*)HeapAlloc(hHeap, 0, BUFSIZE * sizeof(TCHAR)); //alloca espa�o para a resposta no heap
	TCHAR * pchReply = (TCHAR*)HeapAlloc(hHeap, 0, BUFSIZE * sizeof(TCHAR)); //alloca espa�o para a resposta no heap
	//GameDefinition * game = (GameDefinition *)malloc(sizeof(GameDefinition));
	DWORD cbBytesRead = 0, cbReplyBytes = 0, cbWritten = 0;
	BOOL fSucess = FALSE;
	HANDLE hPipe = NULL;
	LPTSTR lpvMessage = TEXT("server_client");

	if (lpvParam == NULL) {
		_tprintf(TEXT("THREAD - Handle enviado pelo param � nulo\n"));
		if (pchReply != NULL)
			HeapFree(hHeap, 0, pchReply);
		if (pchRequest != NULL)
			HeapFree(hHeap, 0, pchRequest);
		return (DWORD)-1;
	}

	if (pchRequest == NULL) {
		_tprintf(TEXT("THREAD - Erro[N�o h� mem�ria para o pedido]\n"));
		if (pchReply != NULL)
			HeapFree(hHeap, 0, pchReply);
		return (DWORD)-1;
	}

	if (pchReply == NULL) {
		_tprintf(TEXT("THREAD - Erro[N�o h� mem�ria para o pedido]\n"));
		if (pchRequest != NULL)
			HeapFree(hHeap, 0, pchRequest);
		return (DWORD)-1;
	}

	_tprintf(TEXT("THREAD - Thread para atender cliente criada,\n\tA voltar ao ciclo do Servidor--\n"));


	hPipe = (HANDLE)lpvParam;

	//Verifica por parte do cliente se � o primeiro
	ReceiveClientMessage(hPipe);

	_tprintf(TEXT("THREAD - Sai do receive client\n"));

	//Vai � memoria partilhada ver se � o primeiro player, se for recebe a estrutura com o jogo configurado!


	//Configura o Jogo na memoria partilhada com os dados recebidos


	//While relativo ao cliente
	while (TRUE) {
		fSucess = ReadFile(
			hPipe, //handle do pipe recebido
			pchRequest, //buffer dos dados a ler
			BUFSIZE * sizeof(TCHAR), //tamanho do buffer
			&cbBytesRead, // numero de bytes a ler
			NULL); //overlapped I/O -> not

		if (!fSucess || cbBytesRead == 0) {
			if (GetLastError() == ERROR_BROKEN_PIPE) {
				_tprintf(TEXT("THREAD - Cliente foi desligado\n"));
			}
			else
				_tprintf(TEXT("THREAD - Erro no ReadFile\n"));

			break;
		}

		//l� o que recebeu do cliente
		_tprintf(TEXT("THREAD - Mensagem do Cliente: %s - Tamanho da Mensagem: %d\n"), pchRequest, cbBytesRead);

		//Chama o adicionar Jogador presente na DLL
		//addPlayer(pchRequest);
		//verificaJogadores();

	

		//processa-se a mensagem recebida
		//faz-se alguma coisa com ela
		//Trato o que � suposto fazer com a mensamge recebida

		//Faz uma fun��o de echo no servidor
		cbReplyBytes = (_tcslen(pchReply) + 1) * sizeof(TCHAR);;

		//Escreve a reposta de volta
		fSucess = WriteFile(
			hPipe, //handle do pipe
			pchReply, //buffer da resposta
			cbReplyBytes, // tamanho da reposta
			&cbWritten, //numero de bytes escritos
			NULL);

		if (!fSucess || cbReplyBytes == 0) {
			_tprintf(TEXT("THREAD - Erro no WriteFile\n"));
			break; //termina intera��o cliente
		}
	}



	//desliga a liga��o com o cliente
	FlushFileBuffers(hPipe);
	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);

	//liberta a mem�ria
	HeapFree(hHeap, 0, pchRequest);
	HeapFree(hHeap, 0, pchReply);

	//Fecha a thread
	_tprintf(TEXT("THREAD - Thread a Finalizar\n"));
	return 1;
}

void ReceiveClientMessage(HANDLE hPipe) {
	HANDLE hHeap = GetProcessHeap(); //aloca espa�o para o pedido no heap
	BOOL fSucess = FALSE;
	TCHAR * pchRequest = (TCHAR*)HeapAlloc(hHeap, 0, BUFSIZE * sizeof(TCHAR)); //alloca espa�o para a resposta no heap
	TCHAR pchReply[kBufferSize];
	DWORD cbBytesRead;

	fSucess = ReadFile(
		hPipe, //handle do pipe recebido
		pchRequest, //buffer dos dados a ler
		BUFSIZE * sizeof(TCHAR), //tamanho do buffer
		&cbBytesRead, // numero de bytes a ler
		NULL); //overlapped I/O -> not

	if (!fSucess || cbBytesRead == 0) {
		if (GetLastError() == ERROR_BROKEN_PIPE) {
			_tprintf(TEXT("THREAD - Cliente foi desligado\n"));
		}
		else
			_tprintf(TEXT("THREAD - Erro no ReadFile\n"));
	}



	_tprintf(TEXT("Receive - MENSAGEM: %s\n"), pchReply);
	SendClientMessage(hPipe, pchReply);
}

void SendClientMessage(HANDLE hPipe, TCHAR * message) {
	DWORD cbBytesRead, cbWritten, cbReplyBytes;
	BOOL fSucess = FALSE;

	_tprintf(TEXT("Send - MENSAGEM: %s\n"), message);
	cbReplyBytes = (_tcslen(message) + 1) * sizeof(TCHAR);;

	//Escreve a reposta de volta
	fSucess = WriteFile(
		hPipe, //handle do pipe
		message, //buffer da resposta
		cbReplyBytes, // tamanho da reposta
		&cbWritten, //numero de bytes escritos
		NULL);

	if (!fSucess || cbReplyBytes == 0) {
		_tprintf(TEXT("THREAD - Erro no WriteFile\n"));
	}
}

MSGPIPESERVIDOR *iniciaMenssagem(MSGPIPESERVIDOR *msg) {


	GAME *auxGame = (GAME*)malloc(sizeof(GAME));
	PLAYER *auxPalyer = (PLAYER*)malloc(sizeof(PLAYER));
	MAP *auxMap = (MAP*)malloc(sizeof(MAP));

	auxGame->gameExist = FALSE;

	msg->hPipe = INVALID_HANDLE_VALUE;
	msg->lpszPipename = TEXT("\\\\.\\pipe\\pipeSnake");
	msg->dwThreadID = 0;
	msg->hThread = 0;
	msg->game = auxGame;
	msg->map = auxMap;
	msg->player = auxPalyer;

	return msg;

}

BOOL ConfigureDLLFunctions() {

	//Faz a liga��o h� dll
	hDll = LoadLibrary(TEXT("DLL.dll"));

	if (hDll == NULL) {
		_tprintf(TEXT("Erro a encontrar a DLL!\n "));
		return TRUE;
	}
	
	//Cria ponteiro para aceder � fun��o
	command = (BOOL(*)(TCHAR[])) GetProcAddress(hDll, "CommandVerifier");
	addPlayer = (BOOL(*)(TCHAR[])) GetProcAddress(hDll, "PlayerAdder");
	player = (int(*)()) GetProcAddress(hDll, "PlayerList");
	configureGame = (void(*)(MSGPIPESERVIDOR  * msgpipe)) GetProcAddress(hDll, "GameConfigurer");

	if (command == NULL) {
		_tprintf(TEXT("Erro a criar o ponteiro para a fun��o!\n, erro: %d"), GetLastError());
		return TRUE;
	}

	if (addPlayer == NULL) {
		_tprintf(TEXT("Erro a criar o ponteiro para a fun��o!\n, erro: %d"), GetLastError());
		return TRUE;
	}

	if (player == NULL) {
		_tprintf(TEXT("Erro a criar o ponteiro para a fun��o!\n, erro: %d"), GetLastError());
		return TRUE;
	}

	if (configureGame == NULL) {
		_tprintf(TEXT("Erro a criar o ponteiro para a fun��o --ConfigureGame--! \n, erro: %d"), GetLastError());
		return TRUE;
	}
}