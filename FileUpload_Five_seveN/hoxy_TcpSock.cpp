#include "stdafx.h"
#include "hoxy_Header.h"

int CTcpSocket::Connect(const SOCKADDR_IN* inAddress)
{
	int err = connect(mSocket, (SOCKADDR*)inAddress, sizeof(SOCKADDR));
	if (err < 0)
	{
		CCmdStart::CmdDebugText(L"CTcpSocket::Connect", false);
		return GetLastError();
	}
	CCmdStart::CmdDebugText(L"CTcpSocket::Connect", true);
	return NO_ERROR;
}

int CTcpSocket::Listen(int inBackLog)
{
	int err = listen(mSocket, inBackLog);
	if (err < 0)
	{
		CCmdStart::CmdDebugText(L"CTcpSocket::Listen", false);
		return GetLastError();
	}
	CCmdStart::CmdDebugText(L"CTcpSocket::Listen", true);
	return NO_ERROR;
}

CTcpSocket* CTcpSocket::Accept(SOCKADDR_IN* inFromAddress)
{
	int length = sizeof(SOCKADDR);
	SOCKET newSocket = accept(mSocket, (SOCKADDR*)inFromAddress, &length);

	if (newSocket != INVALID_SOCKET)
	{
		CCmdStart::CmdDebugText(L"CTcpSocket::Accept", true);
		return new CTcpSocket(newSocket);
	}
	else
	{
		CCmdStart::CmdDebugText(L"CTcpSocket::Accept", false);
		return nullptr;
	}
}

int	CTcpSocket::Send(const void* inData, size_t inLen)
{
	int bytesSentCount = send(mSocket, static_cast<const char*>(inData), inLen, 0);
	if (bytesSentCount < 0)
	{
		CCmdStart::CmdDebugText(L"CTcpSocket::Send", false);
		return -1;
	}
	CCmdStart::CmdDebugText(L"CTcpSocket::Send", true);

	return bytesSentCount;
}

int	CTcpSocket::Receive(void* inData, size_t inLen)
{
	int bytesReceivedCount = recv(mSocket, static_cast<char*>(inData), inLen, 0);
	if (bytesReceivedCount < 0)
	{
		CCmdStart::CmdDebugText(L"CTcpSocket::Receive", false);
		return -1;
	}
	CCmdStart::CmdDebugText(L"CTcpSocket::Receive", true);

	return bytesReceivedCount;
}

int CTcpSocket::Bind(const SOCKADDR_IN* inBindAddress)
{
	int error = bind(mSocket, (SOCKADDR*)inBindAddress, sizeof(SOCKADDR));
	if (error != 0)
	{
		CCmdStart::CmdDebugText(L"CTcpSocket::Bind", false);
		return -1;
	}
	CCmdStart::CmdDebugText(L"CTcpSocket::Bind", true);

	return NO_ERROR;
}

CTcpSocket::~CTcpSocket()
{
	closesocket(mSocket);
}