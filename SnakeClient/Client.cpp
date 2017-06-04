#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include "ClientMenu.h"
#include "MsgPipe.h"
#define BUFSIZE 4096

//Cabeçalho das funções
bool verifyIfFirst(HANDLE hPipe, TCHAR * p);
MSGPIPECLIENTE *iniciaMenssagem(MSGPIPECLIENTE *msg);
MSGPIPECLIENTE *menuMultiplayer1(MSGPIPECLIENTE * msg);
MSGPIPECLIENTE *menuSettingUpGame(int requestCode, MSGPIPECLIENTE *msg);
MSGPIPECLIENTE *createPlayer(MSGPIPECLIENTE *msg);
BOOLEAN	verifySettingRequest(int request, int valueToVerify);
MSGPIPECLIENTE *defineNewKeys(MSGPIPECLIENTE *msg);
MSGPIPECLIENTE *menuPrincipal(MSGPIPECLIENTE *msg);

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
			GENERIC_READ |		//acesso read and write, como está o server
			GENERIC_WRITE,
			0,					//sem partilha
			NULL,				// default security
			OPEN_EXISTING,		//abrir um pipe se já existir
			0,					//atributos default
			NULL);				//sem template

								//se correu tudo bem, o handle já existe e sai o ciclo
		if (hPipe != INVALID_HANDLE_VALUE)
			break;

		//caso seja um erro diferente de todas as instancias estarem ocupados é pq algo correu mal na criaçãpo e sai do prog
		if (GetLastError() != ERROR_PIPE_BUSY) {
			_tprintf(TEXT("Não foi possivel abrir o pipe. Erro = %d\n"), GetLastError());
			return -1;
		}

		//Se passou tudo o resto, é pq todas as 255 instancias do pipe estão ocupadas
		// a solução é esperar que alguma se torne livre e tentar novamente
		//aguarda 20sec

		if (!WaitNamedPipe(lpszPipename, 20000)) {
			_tprintf(TEXT("Esperei 20seg, mas não existe ainda nenhuma livre. Saindo...\n"));
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

	//Verifica se o Servidor já tem algum Cliente Registado
	//if (verifyIfFirst(hPipe, TEXT("first")) == true) {
	//	//Configura a partida se for o primeiro Cliente

	//}

	//Configura os botões com o quais vai jogar
	//
	//
	//

	while (TRUE) {

		_tprintf(TEXT("MENSAGEM TO SERVER -> "));
		_tscanf_s(TEXT("%s"), lpvMessage, BUFSIZE);
		//chegou aqui, está tudo pronto para enviar mensagem para o servidor

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

		} while (!fSucess); //Continua até ainda ter dados para ler

		if (!fSucess) {
			_tprintf(TEXT("ReadFile Falhou. Erro = %d\n"), GetLastError());
			return -1;
		}
	}

	//Fecha o pipe de comunicação
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
		//verifica se é o primeiro
		if (_tcscmp(chBuf, TEXT("sim")) == 0) {
			return true;
		}

	} while (!fSucess); //Continua até ainda ter dados para ler

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

	//TODO: ver como é o mapa com o FOCA 
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

MSGPIPECLIENTE *menuPrincipal(MSGPIPECLIENTE *msg) {
#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif
	int v = 0;

	_tprintf(TEXT("BEM VINDO\n"));

	_tprintf(TEXT("\t\tSingle Player\t-1-\n"));
	_tprintf(TEXT("\t\tMultiPlayer\t-2-\n"));
	do {
		_tscanf_s(TEXT("%d"), &v);
	} while (v < 1 || v > 2);
	_tprintf(TEXT("DO WHILE  %d"), v);
	if (v == 1) {
		msg = menuSettingUpGame(1, msg);
	}
	else if (v == 2) {
		msg = menuMultiplayer1(msg);
	}

	return msg;
}

MSGPIPECLIENTE* menuSettingUpGame(int requestCode, MSGPIPECLIENTE *msg) {

	GAME * auxGame = (GAME *)malloc(sizeof(GAME));

	_tprintf(TEXT("Setting Up The Game\n"));

	createPlayer(msg);

	_tcscpy(auxGame->playerCreator_name, msg->player->player_name);

	do {
		_tprintf(TEXT("\t\tMaximum Number of Items on the Fild (MIN 0)(MAX 10)\n"));
		_tscanf_s(TEXT(" %d"), &auxGame->max_number_items);
	} while (!verifySettingRequest(1, auxGame->max_number_items));

	do {
		_tprintf(TEXT("\t\tStarting Size of the Snake (MIN 4)(MAX 30)\n"));
		_tscanf_s(TEXT(" %d"), &auxGame->started_snake_size);
	} while (!verifySettingRequest(2, auxGame->started_snake_size));

	do {
		_tprintf(TEXT("\t\Game Duration (MIN 1000)(MAX 5000)\n"));
		_tscanf_s(TEXT(" %d"), &auxGame->game_duration);
	} while (!verifySettingRequest(3, auxGame->game_duration));

	do {
		_tprintf(TEXT("\t\tProbability of rare Items to Spawn (MIN 0)(MAX 100)\n"));
		_tscanf_s(TEXT(" %f"), &auxGame->probability_rare_items);
	} while (!verifySettingRequest(4, auxGame->probability_rare_items));

	do {
		_tprintf(TEXT("\t\tNumber of Enemy Snakes (MIN 0)(MAX 5)\n"));
		_tscanf_s(TEXT(" %d"), &auxGame->number_ai_snakes);
	} while (!verifySettingRequest(5, auxGame->number_ai_snakes));

	do {
		_tprintf(TEXT("\t\tNumber of Horizontal Tilles (MIN 10)(MAX 80)\n"));
		_tscanf_s(TEXT(" %d"), &auxGame->horizontal_sizeX);
	} while (!verifySettingRequest(6, auxGame->horizontal_sizeX));

	do {
		_tprintf(TEXT("\t\tNumber of Vertical Tilles (MIN 5)(MAX 40)\n"));
		_tscanf_s(TEXT(" %d"), &auxGame->vertical_sizeY);
	} while (!verifySettingRequest(7, auxGame->vertical_sizeY));






	if (requestCode == 2) {
		do {
			_tprintf(TEXT("\t\tMaximum number of players (MIN 2)(MAX 20)\n"));
			_tscanf_s(TEXT("%d"), &auxGame->actual_players);
		} while (!verifySettingRequest(8, auxGame->actual_players));
	}


	msg->game = auxGame;

	return msg;
}

MSGPIPECLIENTE *menuMultiplayer1(MSGPIPECLIENTE * msg) {
	int v = 0;
	int d = 0;
	_tprintf(TEXT("Multiplayer\n"));
	if (msg->game->gameExist == TRUE) {
		do {
			_tprintf(TEXT("\t\tJoin a Game\t-1-"));
			d = 2;
		} while (v != 1);

	}
	else {
		do {
			_tprintf(TEXT("\t\tHost a Game\t-1-"));
			d = 1;
		} while (v != 1);

	}
	if (d == 1) {
		_tprintf(TEXT("Host Game\n"));
		menuSettingUpGame(2, msg);
	}
	else {
		_tprintf(TEXT("Join Game\n"));
		createPlayer(msg);
	}
	return msg;
}

MSGPIPECLIENTE *createPlayer(MSGPIPECLIENTE *msg) {
	PLAYER *auxPlayer = (PLAYER*)malloc(sizeof(PLAYER));
	int v = 0;

	_tprintf(TEXT("Creating Player\n"));

	_tprintf(TEXT("\t\tPlayer Name\n"));
	_tscanf(TEXT("%s"), &auxPlayer->player_name);



	_tprintf(TEXT("\t\tUse Default Keys to control the snake ? (1-YES) (2-NO)\n"));
	_tscanf_s(TEXT(" %d"), &v);
	if (v == 1) {
		auxPlayer->down_key = KEY_DOWN;
		auxPlayer->left_key = KEY_LEFT;
		auxPlayer->rigth_key = KEY_RIGHT;
		auxPlayer->up_key = KEY_UP;
	}
	else {
		msg = defineNewKeys(msg);
	}

	msg->player = auxPlayer;
	return msg;

}

BOOLEAN	verifySettingRequest(int request, int valueToVerify) {
	int v = 0;
	switch (request) {
	case 1:
		if (valueToVerify > MIN_NUMBER_ITEM && valueToVerify < MAX_NUMBER_ITEM) {
			v = 1;
		}

		break;
	case 2:
		if (valueToVerify > MIN_SIZE_SNAKE && valueToVerify < MAX_SIZE_SNAKE) {
			v = 1;
		}
		break;
	case 3:
		if (valueToVerify > MIN_DURATION_GAME && valueToVerify < MAX_DURATION_GAME) {
			v = 1;
		}
		break;
	case 4:
		if (valueToVerify > MIN_PROP_RARE_ITEMS && valueToVerify < MAX_PROP_RARE_ITEMS) {
			v = 1;
		}
		break;
	case 5:
		if (valueToVerify > MIN_AI_SNAKES && valueToVerify < MAX_AI_SNAKES) {
			v = 1;
		}
		break;
	case 6:
		if (valueToVerify > MIN_HORIZONTAL_SIZE && valueToVerify < MAX_HORIZONTAL_SIZE) {
			v = 1;
		}
		break;
	case 7:
		if (valueToVerify > MIN_VERTICAL_SIZE && valueToVerify < MAX_VERTICAL_SIZE) {
			v = 1;
		}
		break;
	case 8:
		if (valueToVerify > MIN_NUMBER_PLAYER && valueToVerify < MAX_NUMBER_PLAYER) {
			v = 1;
		}
		break;
	default:
		break;
	}

	if (v == 1) {
		return TRUE;
	}
	_tprintf(TEXT("\t\tError: value out of bound, try again.\n"));
	return FALSE;

}

MSGPIPECLIENTE *defineNewKeys(MSGPIPECLIENTE *msg) {
	BOOLEAN v = FALSE;
	int c = 0;
	char up;
	char down;
	char left;
	char right;
	do {
		_tprintf(TEXT("insert key to go UP\n"));
		_tscanf(TEXT("%c"), &up);
		msg->player->up_key = (int)up;
		_tprintf(TEXT("CONFIRM (1-Yes)(2-NO)\n"));
		_tscanf(TEXT("%d"), &c);
		if (c == 1) {
			v = TRUE;
		}
	} while (v != TRUE);
	v = FALSE;

	do {
		_tprintf(TEXT("insert key to go RIGHT\n"));
		_tscanf(TEXT("%c"), &right);
		msg->player->up_key = (int)right;
		_tprintf(TEXT("CONFIRM (1-Yes)(2-NO)\n"));
		_tscanf(TEXT("%d"), &c);
		if (c == 1) {
			v = TRUE;
		}
	} while (v != TRUE);
	v = FALSE;

	do {
		_tprintf(TEXT("insert key to go DOWN\n"));
		_tscanf(TEXT("%c"), &down);
		msg->player->up_key = (int)down;
		_tprintf(TEXT("CONFIRM (1-Yes)(2-NO)\n"));
		_tscanf(TEXT("%d"), &c);
		if (c == 1) {
			v = TRUE;
		}
	} while (v != TRUE);
	v = FALSE;

	do {
		_tprintf(TEXT("insert key to go LEFT\n"));
		_tscanf(TEXT("%c"), &left);
		msg->player->up_key = (int)left;
		_tprintf(TEXT("CONFIRM (1-Yes)(2-NO)\n"));
		_tscanf(TEXT("%d"), &c);
		if (c == 1) {
			v = TRUE;
		}
	} while (v != TRUE);






	return msg;

}