#include "stdafx.h"
#include "hoxy_Header.h"
//#include <string.h>

#pragma pack(push, 1)
struct st_PACKET_HEADER
{
	DWORD	dwPacketCode = 0x11223344;		// 0x11223344	우리의 패킷확인 고정값

	WCHAR	szName[32] = { 0, };		// 본인이름, 유니코드 NULL 문자 끝
	WCHAR	szFileName[128] = { 0, };	// 파일이름, 유니코드 NULL 문자 끝
	int	iFileSize = 0;
};
#pragma pack(pop)

int main()
{
	CCmdStart myCmdStart;
	CSockUtill::WSAStart();
	//CTcpSocket* clientSock = CSocketUtil::CreateTCPSocket(INET);
	CTcpSocket* serverSock = CSockUtill::CreateTCPSocket(INET);

	// 서버 주소 설정
	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(10099);

	WCHAR serverDomain[] = L"procademyserver.iptime.org";
	IN_ADDR s_inAddr;
	ZeroMemory(&s_inAddr, sizeof(IN_ADDR));

	CSockUtill::DomainToIP(serverDomain, &s_inAddr);

	serverAddr.sin_addr = s_inAddr;

	// 서버 IP 출력
	WCHAR proIP[20] = { 0, };
	InetNtop(AF_INET, &s_inAddr, proIP, 20);
	wcout << L"SERVER IP : " << proIP << endl;

	//# 로직순서
	//1. 서버접속
	serverSock->Connect(&serverAddr);

	//2. 특정 파일오픈
	FILE* fp = nullptr;
	errno_t err = _wfopen_s(&fp, L"1499732002_ddds.jpg", L"rb");
	if (err != 0)
	{
		CCmdStart::CmdDebugText(L"File Open", false);
		return -1;
	}

	//3. 사이즈체크
	fseek(fp, 0, SEEK_END);
	int fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// 메모리로 읽어옴
	char* fileBuffer = (char*)malloc(fileSize);
	fread(fileBuffer, fileSize, 1, fp);

	//4. 헤더생성
	st_PACKET_HEADER fileHeader;
	fileHeader.iFileSize = fileSize;
	wcscpy_s(fileHeader.szFileName, 8, L"ljh.jpg");
	wcscpy_s(fileHeader.szName, 4, L"임재홍");

	//5. 서버로 헤더 전송
	int hRet = serverSock->Send(&fileHeader, sizeof(fileHeader));
	wcout << L"header sent : " << hRet << endl;

	//6. 서버로 파일 데이터 전송 1000단위 쪼개서 보내기
	int left = fileSize;
	char* filePtr = fileBuffer;

	while (left > 0)
	{
		int retval = 0;
		if (left >= 1000)
		{
			retval = serverSock->Send(filePtr, 1000);
		}
		else
		{
			retval = serverSock->Send(filePtr, left);
		}

		if (retval < 0)
		{
			CCmdStart::CmdDebugText(L"send FAIL", false);
			return -1;
		}

		wcout << L"file send : " << retval << endl;

		left -= retval;
		filePtr += retval;
	}

	//7. 파일전송 성공시 서버에서 클라이언트로 0xdddddddd 를 보내줌.
	UINT receiveCode = 0;
	serverSock->Receive(&receiveCode, 4);
	if (receiveCode == 0xdddddddd)
	{
		wcout << L"SUCESS!!!" << endl;
	}
	else
	{
		wcout << L"FAIL" << endl;
	}

	free(fileBuffer);

	return 0;
}