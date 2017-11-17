#pragma once
#pragma comment(lib, "ws2_32")

class CTcpSocket
{
public:
	~CTcpSocket();
	int	Connect(const SOCKADDR_IN* inAddress);
	int	Bind(const SOCKADDR_IN* inToAddress);
	int	Listen(int inBackLog = SOMAXCONN);
	CTcpSocket* Accept(SOCKADDR_IN* inFromAddress);
	int	Send(const void* inData, size_t inLen);
	int	Receive(void* inBuffer, size_t inLen);
private:
	friend class CSockUtill;
	CTcpSocket(SOCKET inSocket) : mSocket(inSocket) {}
	SOCKET mSocket;
};
