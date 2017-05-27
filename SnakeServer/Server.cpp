//SERVIDOR
#include "..\..\SharedMemoryDLL\SnakeOnline\DLL.h"
#include <conio.h>
#include <process.h>


//TCHAR szNAME[] = TEXT("fmMsgSpace");

#define MSGSIZE 75

typedef struct _MSG {
	unsigned msgnum;
	TCHAR szMessage[MSGSIZE];

}Shared_MSG;


#define MSGBUFSIZE sizeof	(Shared_MSG)

//------------------------------------


//funcçoes de escrita / leitura de mensagens
//sem sincronizaçao ( mas devera ter os locais )

void initMsgArea(Shared_MSG * shared) { // sem validação de tamanho
										// fheca MUTEX

	TCHAR inicio[] = TEXT("empty");
	shared->msgnum = 0;
	CopyMemory((PVOID)shared->szMessage, inicio, _tcslen(inicio) * sizeof(TCHAR)); // ou stringcopy
																				   //abre MUTEX
}

//sem sincronizaçao ( mas devera ter os locais )

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


//---------------------------------------------------

//thread de leitura de mensagens 
//não usa variaveis globais

typedef struct _ControlData {
	HANDLE hMapFile;
	Shared_MSG * pBuf;
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
		}
	}
	return 0;
}

int _tmain() {
	ContrData cdata;
	unsigned tid;
	HANDLE thnd;

	_tprintf(TEXT("Servifor de msg a iniciar. \n"));
	_tprintf(TEXT("Vou Criar Memoria Partilhada. \n "));

	// POR ESTA PARTE NA DLL;

	cdata.hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		MSGBUFSIZE,
		szNAME);

	if (cdata.hMapFile == NULL) {
		_tprintf(TEXT(
			"Houve azar na memoria partilhada ( erro %d).\n"), GetLastError());
		return 1;
	}
	_tprintf(TEXT("Vou Criar a View da memoria Partilhada.\n"));

	cdata.pBuf = (Shared_MSG*)MapViewOfFile(
		cdata.hMapFile,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		MSGBUFSIZE);
	if (cdata.pBuf == NULL) {
		_tprintf(TEXT(
			"A view da memoria partilhada nao foi criada ( erro: %d). \n"), GetLastError());
		CloseHandle(cdata.hMapFile);
		return 1;
	}
	_tprintf(TEXT("Sucess. Enviar mensagem inicial.\n"));
	initMsgArea(cdata.pBuf); //serve para colocar uma mensagem 
							 // inicia com o numero 0;
	writeMensagem(cdata.pBuf, TEXT("Servidor : Zona de Mensagem Criada"));

	_tprintf(TEXT("vou lançar uma thread para ouvir o que se passa. \n"));
	cdata.ThreadDeveContinuar = 1;
	_beginthreadex(0, 0, listenerThread, &cdata, 0, &tid);//possivel problema no TID é supostop ser DWOR na declaração

	thnd = OpenThread(THREAD_ALL_ACCESS, FALSE, tid);

	_tprintf(TEXT("carrega numa tecla para fechar o servidor"));
	_getch();
	cdata.ThreadDeveContinuar = 0; // indica que a thread deve acabar a
	WaitForSingleObject(thnd, INFINITE); // aguarda que a thread termine
	_tprintf(TEXT("thread encerrada\n"));

	writeMensagem(cdata.pBuf, TEXT("Servidor : Fechar"));
	UnmapViewOfFile(cdata.pBuf);
	CloseHandle(cdata.hMapFile);
	return 0;
}