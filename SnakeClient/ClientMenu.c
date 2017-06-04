#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include "ClientMenu.h"


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
	} while (v != 1 || v != 2);

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
	} while (verifySettingRequest(1, auxGame->max_number_items));

	do {
		_tprintf(TEXT("\t\tStarting Size of the Snake (MIN 4)(MAX 30)\n"));
		_tscanf_s(TEXT(" %d"), &auxGame->started_snake_size);
	} while (verifySettingRequest(2, auxGame->started_snake_size));

	do {
		_tprintf(TEXT("\t\Game Duration (MIN 1000)(MAX 5000)\n"));
		_tscanf_s(TEXT(" %d"), &auxGame->game_duration);
	} while (verifySettingRequest(3, auxGame->game_duration));

	do {
		_tprintf(TEXT("\t\tProbability of rare Items to Spawn (MIN 0)(MAX 100)\n"));
		_tscanf_s(TEXT(" %f"), &auxGame->probability_rare_items);
	} while (verifySettingRequest(4, auxGame->probability_rare_items));

	do {
		_tprintf(TEXT("\t\tNumber of Enemy Snakes (MIN 0)(MAX 5)\n"));
		_tscanf_s(TEXT(" %d"), &auxGame->number_ai_snakes);
	} while (verifySettingRequest(5, auxGame->number_ai_snakes));

	do {
		_tprintf(TEXT("\t\tNumber of Horizontal Tilles (MIN 10)(MAX 80)\n"));
		_tscanf_s(TEXT(" %d"), &auxGame->horizontal_sizeX);
	} while (verifySettingRequest(6, auxGame->horizontal_sizeX));

	do {
		_tprintf(TEXT("\t\tNumber of Vertical Tilles (MIN 5)(MAX 40)\n"));
		_tscanf_s(TEXT(" %d"), &auxGame->vertical_sizeY);
	} while (verifySettingRequest(7, auxGame->vertical_sizeY));






	if (requestCode == 2) {
		do {
			_tprintf(TEXT("\t\tMaximum number of players (MIN 2)(MAX 20)\n"));
			_tscanf_s(TEXT("%d"), &auxGame->actual_players);
		} while (verifySettingRequest(8, auxGame->actual_players));
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

}

MSGPIPECLIENTE *createPlayer(MSGPIPECLIENTE *msg) {
	PLAYER *auxPlayer = (PLAYER*)malloc(sizeof(PLAYER));
	int v = 0;

	_tprintf(TEXT("Creating Player\n"));

	_tprintf(TEXT("\t\tPlayer Name\n"));
	_tscanf_s(TEXT(" %s"), &auxPlayer->player_name);



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