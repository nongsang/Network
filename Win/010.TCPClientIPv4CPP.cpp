#pragma comment(lib, "ws2_32")
//#include <WinSock2.h>			// WS2tcpip.h에 포함되어 있으므로 따로 추가 안해줘도 된다.
#include <WS2tcpip.h>
#include <iostream>

#define SERVERIP "127.0.0.1"
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

int recvn(SOCKET s, char * buf, int len, int flags)
{
	int received;
	char * ptr = buf;
	int left = len;

	while (left > 0)
	{
		received = recv(s, ptr, left, flags);
		if (SOCKET_ERROR == received)
			return SOCKET_ERROR;
		else if (0 == received)
			break;

		left -= received;

		ptr += received;
	}

	return (len - left);
}

int main()
{
	WSADATA wsa;

	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
		return 1;

	MessageBox(NULL, L"윈속 초기화", L"알림", MB_OK);

	int retval;

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == sock) err_quit(L"socket()");

	SOCKADDR_IN serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;							// serveraddr의 주소체계는 AF_INET
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr.s_addr);	// serveraddr.sin_addr.s_addr에 SERVERIP를 AF_INET에 맞게 변형하여 저장
																// 기존은 serveraddr.sin_addr.s_addr에 inet_addr(SERVERIP)를 저장했지만
																// inet_pton()를 이용하면 thread safe하므로 사용하면 좋다.
	serveraddr.sin_port = htons(SERVERPORT);					// serveraddr의 포트번호 설정


	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (SOCKET_ERROR == retval) err_quit(L"connect()");

	// 데이터 통신에 사용할 변수
	char buf[BUFSIZE + 1];
	int len;

	while (1)
	{
		cout << "[보낼 데이터] : ";
		if (NULL == fgets(buf, BUFSIZE + 1, stdin))
			break;

		len = strlen(buf);
		if ('\n' == buf[len - 1])
			buf[len - 1] = '\0';
		if (0 == strlen(buf))
			break;

		retval = send(sock, buf, strlen(buf), 0);
		if (SOCKET_ERROR == retval)
		{
			err_display(L"send()");
			break;
		}

		cout << "[TCP 클라이언트] : " << retval << "바이트를 보냈습니다." << endl;

		//retval = recvn(sock, buf, retval, 0);			// recvn()을 사용하는 대신에
		retval = recv(sock, buf, retval, MSG_WAITALL);	// recv()의 flags값을 MSG_WAITALL로 해도 recvn()과 같은 의미다.

		if (SOCKET_ERROR == retval)
		{
			err_display(L"recv()");
			break;
		}
		else if (0 == retval)
			break;

		buf[retval] = '\0';
		cout << "[TCP 클라이언트] : " << retval << "바이트를 받았습니다." << endl;
		cout << "[받은 데이터] : " << buf << endl;
	}

	closesocket(sock);

	WSACleanup();

	return 0;
}

// 사용한 함수
// socket()  : 소켓 생성
//			   생성된 소켓은 블로킹 소켓이다.
// connect() : 호스트에 연결 요청을 한다.
// send()	 : 데이터를 운영체제의 송신 버퍼에 복사한다.
//			   송신 버퍼에 있는 데이터는 연결된 호스트의 운영체제의 수신 버퍼로 전송한다.
//			   송신 버퍼에 데이터가 있다고 바로 보내는 것이 아니라 일정 시간이 지난 후 하부 프로토콜을 통해 전송이 된다.
//			   전달하는 소켓의 특성에 따라 다른 전송방법을 사용한다.
//			   socket()으로 생성한 소켓을 전달하면 블로킹 방식으로 데이터를 전송한다.
//			   송신 버퍼의 여유공간이 send()의 세번째 인자 len보다 작으면 대기한다.
//			   여유가 생기면 그때서야 송신 버퍼에 데이터를 복사한다.
//			   넌블록킹 소켓이라면 송신 버퍼의 여유가 조금이라도 생기면 바로바로 데이터를 송신 버퍼에 복사하게 된다.
//			   넌블록킹 소켓은 나중에 알아보자.
// recv()	 : 운영체제의 수신 버퍼에 도착한 데이터를 응용 프로그램 버퍼에 복사한다.
//			   반환값은 전달받은 데이터의 길이이다.
//			   반환값이 0이면 상대가 정상 종료했다는 의미이다.
// recvn()	 : TCP는 데이터 경계를 구분하지 않는다.
//			   recv()로 데이터를 받을 때 그림파일과 텍스트파일을 동시에 보내면 TCP는 그림파일에서 나온 데이터와 텍스트에서 나온 데이터를 구분하지 않는다.
//			   하나씩 하나씩 데이터 덩어리로 수신 버퍼에서 응용 프로그램 버퍼에 복사하는데, 데이터 길이를 알려준다 한들 복사할 때 크기 또한 제멋대로 복사한다.
//			   따라서 먼저 보낸 데이터를 처리한 후 다음 데이터를 처리하는 방식으로 바꾸고, 알려준 데이터 크기만큼 처리하는 방식으로 바꾼다면 제대로 구분이 될 것이다.
//			   사실 recv()의 네번째 인자 flags에 MSG_WAITALL을 전달하면 recvn()과 같다.
//			   하지만 Windows XP 이하에서는 사용할 수 없다.
//			   현재는 Windows 7이상이 일반적이므로 사용해도 된다.

// 서버와 달리 사용하지 않은 함수
// bind()	: 소켓에 IP 주소, 포트번호를 결정해준다.
//			  bind()를 사용해서 IP 주소와 포트번호를 직접 지정해줄 수는 있다.
//			  하지만 그냥 connect()를 호출하면, 운영체제가 자동으로 IP