#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <WinSock2.h>
#include <Ws2tcpip.h>

#include <iostream>
#include <iomanip>
#include <locale>
//#include <locale.h>

using std::wcout;
using std::wcin;
using std::endl;

#include "hoxy_CmdStart.h"

#include "TCPSocket.h"
#include "SocketUtil.h"