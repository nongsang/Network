#pragma comment(lib, "ws2_32")
//#include <WinSock2.h>			// WS2tcpip.h에 포함되어 있으므로 따로 추가 안해줘도 된다.
#include <WS2tcpip.h>
#include <iostream>

#define SERVERPORT 9000
#define BUFSIZE 512

// 유니코드 문자 집합

using namespace std;

void err_quit(LPCWSTR msg)		// 002.InitWinsock.cpp에 설명했으니 기억 안나면 읽어봐
{
	LPVOID lpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		LANG_USER_DEFAULT,
		(LPWSTR)&lpMsgBuf,
		0, NULL
	);

	MessageBox(NULL, (LPCWSTR)lpMsgBuf, msg, MB_ICONERROR);

	LocalFree(lpMsgBuf);

	exit(1);
}

void err_display(LPCWSTR msg)	// 007.DomainC.cpp에 설명했으니 기억 안나면 읽어봐
{
	LPVOID lpMsgBuf;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		LANG_USER_DEFAULT,
		(LPWSTR)&lpMsgBuf
		, 0, NULL
	);

	MessageBox(NULL, (LPCWSTR)lpMsgBuf, msg, MB_ICONERROR);

	LocalFree(lpMsgBuf);
}

int main()
{
	WSADATA wsa;

	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
		return 1;

	MessageBox(NULL, L"윈속 초기화", L"알림", MB_OK);

	int retval;

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == listen_sock) err_quit(L"socket()");

	SOCKADDR_IN serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));

	serveraddr.sin_family = AF_INET;							// serveraddr의 주소체계는 AF_INET
	inet_pton(AF_INET, INADDR_ANY, &serveraddr.sin_addr.s_addr);// serveraddr.sin_addr.s_addr에 INADDR_ANY을 AF_INET에 맞게 변형하여 저장
																// 기존은 serveraddr.sin_addr.s_addr에 htonl(INADDR_ANY)를 저장했지만
																// inet_pton()를 이용하면 thread safe하므로 사용하면 좋다.
	serveraddr.sin_port = htons(SERVERPORT);					// serveraddr의 포트번호 설정

	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (SOCKET_ERROR == retval) err_quit(L"bind()");

	retval = listen(listen_sock, SOMAXCONN);

	if (SOCKET_ERROR == retval) err_quit(L"listen()");

	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	
	char buf[BUFSIZE];

	while (1)
	{
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);

		if (INVALID_SOCKET == client_sock)
		{
			err_display(L"accept()");
			break;
		}

		//printf("[TCP 서버] 클라이언트 접속 : IP 주소 = %s, 포트 번호 = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		cout << "[TCP 서버] 클라이언트 접속 : IP 주소 = " << inet_ntoa(clientaddr.sin_addr) << ", 포트 번호 = " << ntohs(clientaddr.sin_port) << endl;

		while (1)
		{
			retval = recv(client_sock, buf, BUFSIZE, 0);
			if (INVALID_SOCKET == retval)
			{
				err_display(L"recv()");
				break;
			}
			else if (0 == retval)
				break;

			buf[retval] = '\0';

			//printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);
			cout << "[TCP/" << inet_ntoa(clientaddr.sin_addr) << ":" << ntohs(clientaddr.sin_port) << "] " << buf << endl;

			retval = send(client_sock, buf, retval, 0);

			if (SOCKET_ERROR == retval)
			{
				err_display(L"send()");
				break;
			}
		}

		closesocket(client_sock);
		//printf("[TCP 서버] 클라이언트 종료 : IP 주소 = %s, 포트 번호 = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		cout << "[TCP 서버] 클라이언트 종료 : IP 주소 = " << inet_ntoa(clientaddr.sin_addr) << ", 포트 번호 = " << ntohs(clientaddr.sin_port) << endl;
	}

	closesocket(listen_sock);

	WSACleanup();

	return 0;
}

// 사용한 함수
// socket() : 소켓 생성
//			  생성된 소켓은 블로킹 소켓이다.
// bind()	: 소켓에 IP 주소, 포트번호를 결정해준다.
//			  SOCKET과 SOCKADDR_IN 인자가 필요하다.
//			  서버 소켓이므로 SOCKADDR_IN에 들어가는 IP주소는 INADDR_ANY를 적는 것이 바람직하다.
// listen() : 소켓의 TCP 포트 상태를 LISTENING으로 바꾼다.
//			  이는 클라이언트 접속을 받을 수 있는 대기상태로 바꿈을 뜻한다.
//			  또한 SOMAXCONN은 클라이언트가 접속할 수 있는 갯수를 최대로 한다는 의미다.
// accept() : 접속한 클라이언트와 통신할 수 있도록 새로운 소켓을 생성해서 리턴한다.
//			  생성된 소켓은 접속을 요청한 소켓과 연결 후 데이터를 교환하게 된다.
//			  기존에 있었던 listen()으로 생성했던 대기소켓을 연결하는 것이 아니라 새로운 소켓을 만들어서 클라이언트와 연결하는 것이다.
//			  연결이 되더라도 대기소켓은 계속해서 남아서 LISTENING 상태를 계속해서 보여주는 역할이다.
//			  만약 접속을 요청한 클라이언트의 주소나 길이가 필요없으면 addr와 addrlen은 NULL로 전달하면 된다.
// send()	: 데이터를 운영체제의 송신 버퍼에 복사한다.
//			  송신 버퍼에 있는 데이터는 연결된 호스트의 운영체제의 수신 버퍼로 전송한다.
//			  송신 버퍼에 데이터가 있다고 바로 보내는 것이 아니라 일정 시간이 지난 후 하부 프로토콜을 통해 전송이 된다.
//			  전달하는 소켓의 특성에 따라 다른 전송방법을 사용한다.
//			  socket()으로 생성한 소켓을 전달하면 블로킹 방식으로 데이터를 전송한다.
//			  송신 버퍼의 여유공간이 send()의 세번째 인자 len보다 작으면 대기한다.
//			  여유가 생기면 그때서야 송신 버퍼에 데이터를 복사한다.
//			  넌블록킹 소켓이라면 송신 버퍼의 여유가 조금이라도 생기면 바로바로 데이터를 송신 버퍼에 복사하게 된다.
//			  넌블록킹 소켓은 나중에 알아보자.
// recv()	: 운영체제의 수신 버퍼에 도착한 데이터를 응용 프로그램 버퍼에 복사한다.
//			  반환값은 전달받은 데이터의 길이이다.
//			  반환값이 0이면 상대가 정상 종료했다는 의미이다.

// 클라이언트와 달리 사용하지 않은 함수
// connect() : 호스트에 연결 요청을 한다.
//			   연결을 허가해주는 쪽이므로 사용하지 않는 함수이다.
// recvn()	 : 간단하게 recv()의 네번째 인자 flags에 MSG_WAITALL을 전달하면 사용할 수 있다.