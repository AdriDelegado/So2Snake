#pragma once
#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include	"MsgPipe.h"
#include "..\SnakeServer\Constants.h"


MSGPIPE *menuMultiplayer1(MSGPIPE * msg);
MSGPIPE* menuSettingUpGame(int requestCode, MSGPIPE *msg);
MSGPIPE *createPlayer(MSGPIPE *msg);
BOOLEAN	verifySettingRequest(int request, int valueToVerify);
MSGPIPE *defineNewKeys(MSGPIPE *msg);
MSGPIPE *mainMenu(MSGPIPE *msg);