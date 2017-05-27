//CLIENTE


#include "..\..\SharedMemoryDLL\SnakeOnline\DLL.h"
#include <conio.h>
#include <process.h>

#pragma comment (lib, "user32.lib")

//TCHAR szNAME[] = TEXT("fmMsgSpace");

#define MSGSIZE 75

typedef struct _MSG {
	unsigned msgnum;
	TCHAR szMessage[MSGSIZE];

}Shared_MSG;


#define MSGBUFSIZE sizeof(Shared_MSG)

// sem sincronização

unsigned writeMensagem(Shared_MSG * shared, TCHAR * msgtext) {
	unsigned myNum;
	int numchars = _tcslen(msgtext);
	if (numchars > MSGSIZE - 1) {
		numchars = MSGSIZE - 1;
		msgtext[MSGSIZE - 1] = _T('\0'); // metodo primitivo
	}

	//fecha MUTEX
	shared->msgnum++;
	myNum = shared->msgnum;
	_tcscpy(shared->szMessage, msgtext); // ou copyMemory
										 // abre MUTEX

	return myNum;
}


void readMensagem(Shared_MSG * shared, Shared_MSG * msg) {
	//fechar MUTEX
	shared->msgnum;
	CopyMemory(msg, shared, sizeof(Shared_MSG));
	// abre MUTEX
}

//sem sincronizaçao ( mas devera ter os locais )

unsigned peekMesagem(Shared_MSG * shared) {
	unsigned msgnum;
	//fecha MUTEX

	msgnum = shared->msgnum;
	//abre MUTEX

	return msgnum;
}

//thread de leitura de mensagens 
//não usa variaveis globais

typedef struct _ControlData {
	HANDLE hMapFile;
	Shared_MSG * pBuf;
	int ServidorContinua;
	int ThreadDeveContinuar;
}ContrData;

unsigned int __stdcall listenerThread(void *p) {
	ContrData * pcd = (ContrData *)p;
	unsigned int current = peekMesagem(pcd->pBuf);
	Shared_MSG rcv;
	while (pcd->ThreadDeveContinuar) {
		Sleep(500); //solução fraca usar sincronização
					//(fazer como exercicio) sugestao : eventos
		if (peekMesagem(pcd->pBuf) > current) {
			readMensagem(pcd->pBuf, &rcv);
			current = rcv.msgnum;
			_tprintf(TEXT("[%d]: %s\n"), current, rcv.szMessage);
			if (_tcscmp(rcv.szMessage, TEXT("Servidor : fecha \n")) == 0) {
				pcd->ServidorContinua = 0;
			}
		}
	}
	return 0;
}


//--------------------------------------

// funcção principal 
//lê ( do teclado ) as mensagens do utilizador e envias ( para memoria part);

int _tmain() {
	ContrData cdata;
	unsigned tid;
	HANDLE thnd;

	TCHAR myText[MSGSIZE];
	Shared_MSG currentMSG;

	_tprintf(TEXT("Cliente de Msg a iniciar. \n"));
	_tprintf(TEXT("Vou abrir a memoria partilhada"));

	cdata.hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,
		FALSE,
		szNAME);
	if (cdata.hMapFile == NULL) {
		_tprintf(TEXT(
			"Ocorreu um erro durante a abertura da memoria partiulhada (erro: %d)\n"), GetLastError());
		return 1;
	}

	_tprintf(TEXT("vou criar a view da memoria partilhada. \n"));

	cdata.pBuf = (Shared_MSG *)MapViewOfFile(
		cdata.hMapFile,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		MSGBUFSIZE);
	if (cdata.pBuf == NULL) {
		_tprintf(TEXT(
			"Ocorreu um problema durante a criação da view ( erro: %d).\n"), GetLastError());
		CloseHandle(cdata.hMapFile);
		return 1;
	}

	_tprintf(TEXT("vou lancar a thread para ouvir o que se passa.\n"));

	cdata.ThreadDeveContinuar = 1;
	cdata.ServidorContinua = 1;
	_beginthreadex(0, 0, listenerThread, &cdata, 0, &tid);
	thnd = OpenThread(THREAD_ALL_ACCESS, FALSE, tid);

	_tprintf(TEXT("SUCESSO, vou ver a mensagem acual"));
	readMensagem(cdata.pBuf, &currentMSG);

	_tprintf(TEXT("(%d): %s"), currentMSG.msgnum, currentMSG.szMessage);

	_tprintf(TEXT("Escreve ai qualquer coisa. nao suar espaço. exit para sair \n"));


	while (1) {
		_tscanf(TEXT("%s"), myText);
		if (_tcscmp(myText, TEXT("exit")) == 0)
			break;
		if (cdata.ServidorContinua == 0) {
			_tprintf(TEXT("Servidor mandou fechar\n"));
			break;
		}
		writeMensagem(cdata.pBuf, myText);
	}
	_tprintf(TEXT("Cliente vois fechar\n"));

	cdata.ThreadDeveContinuar = 0;
	WaitForSingleObject(thnd, INFINITE);
	_tprintf(TEXT("Thread ouvinte fechou\n"));

	UnmapViewOfFile(cdata.pBuf);
	CloseHandle(cdata.hMapFile);
	_tprintf(TEXT("Ficheiro Desmapeado e recursos Libertados\n"));
	return 0;
}