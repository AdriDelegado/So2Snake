#pragma once

#include "MsgPipe.h"
#include "..\SnakeServer\Constants.h"


MSGPIPECLIENTE *menuMultiplayer1(MSGPIPECLIENTE * msg);
MSGPIPECLIENTE *menuSettingUpGame(int requestCode, MSGPIPECLIENTE *msg);
MSGPIPECLIENTE *createPlayer(MSGPIPECLIENTE *msg);
BOOLEAN	verifySettingRequest(int request, int valueToVerify);
MSGPIPECLIENTE *defineNewKeys(MSGPIPECLIENTE *msg);
MSGPIPECLIENTE *menuPrincipal(MSGPIPECLIENTE *msg);