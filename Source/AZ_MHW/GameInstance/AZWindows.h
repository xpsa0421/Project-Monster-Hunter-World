#pragma once
/** 윈도우 API, 외부 API 헤더, STL 헤더는 아래에 인클루드 해주세요.*/
#include "Windows/AllowWindowsPlatformTypes.h"
#define WIN32_LEAN_AND_MEAN

#include <unordered_set>
#include <unordered_map>
#include <map>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <vector>
#include <list>
#include <tuple>

#include <thread>
#include <mutex>
#include <memory>

#include <winsock2.h>
#include <windows.h>
#include <Ws2tcpip.h>
#include <mswsock.h>

#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>

#include <tchar.h>
#include <string>
#include <time.h>
#include <ctime>
#include <iostream>
#include <stdio.h>

#pragma comment(lib, "mswsock.lib")
#pragma comment(lib, "ws2_32.lib")

#include "Windows/HideWindowsPlatformTypes.h"