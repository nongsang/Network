#pragma comment(lib, "ws2_32")
//#include <WinSock2.h>			// WS2tcpip.h에 포함되어 있으므로 따로 추가 안해줘도 된다.
#include <WS2tcpip.h>
#include <stdio.h>

#define SERVERIP "127.0.0.1"// IPv4 루프백 주소, 그냥 자기 자신을 가리키는 주소
#define SERVERPORT 9000		// 서버 포트번호, 그냥 임의로 만든 것
#define BUFSIZE 512			// 문자열을 저장할 버퍼의 크기
							// 아니면 BUFSIZ라고 이미 똑같은 상수가 있으니 사용해도 좋다.

// 유니코드 문자 집합

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

int recvn(SOCKET s, char * buf, int len, int flags)	// recv()함수는 len보다 적은 데이터를 보낼 수 있다.
{													// 그래서 데이터가 크면 다 안보내질 수 있으므로 recvn()함수를 작성한다.
	int received;			// 보낸 데이터 크기
	char * ptr = buf;		// 보낼 데이터가 시작하는 위치
	int left = len;			// 남은 데이터 크기

	while (left > 0)		// 남은 데이터가 있으면
	{
		received = recv(s, ptr, left, flags);	// s에서 ptr 데이터를 left 길이만큼 받고, 받은 길이를 received에 저장한다.
		if (SOCKET_ERROR == received)			// 제대로 받지 못하면
			return SOCKET_ERROR;
		else if (0 == received)					// 받은 데이터의 크기가 0이면
			break;

		left -= received;						// 남은 데이터 길이에서 보낸 데이터 길이를 뺀다.
												// recv()는 데이터가 클 때 한번에 모든 데이터를 다 받을 수 없으므로
												// 일정량의 데이터를 받고 다시 받을 때 얼만큼 받을 데이터가 남았는지 알기 위해서 사용
		ptr += received;						// 데이터를 다시 받을 때 어느위치에서 부터 시작할 지 알기 위해 사용
	}

	return (len - left);	// 데이터를 다 받으면 left가 0이 되므로, 결국 받은 데이터의 길이만 반환된다.
}

int main()
{
	WSADATA wsa;

	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
		return 1;

	MessageBox(NULL, L"윈속 초기화", L"알림", MB_OK);

	int retval;		// 에러 코드를 저장할 변수

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);		// 주소체계는 IPv4, 타입은 SOCK_STREAM인 소켓 생성
	if (INVALID_SOCKET == sock) err_quit(L"socket()");

	SOCKADDR_IN serveraddr;								// 연결할 소켓의 정보를 담고 있는 구조체
	memset(&serveraddr, 0, sizeof(serveraddr));			// 0으로 초기화
	serveraddr.sin_family = AF_INET;					// IPv4
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);	// 연결할 서버의 주소
	serveraddr.sin_port = htons(SERVERPORT);			// 연결할 서버의 포트번호

	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));	// sock에 serveraddr 정보로 연결요청
	if (SOCKET_ERROR == retval) err_quit(L"connect()");					// 연결 실패하면 종료

	// 데이터 통시에 사용할 변수
	char buf[BUFSIZE + 1];
	int len;

	while (1)		// 연결이 되면 연속으로 데이터를 보낸다.
	{
		printf("[보낼 데이터] : ");
		if (NULL == fgets(buf, BUFSIZE + 1, stdin))	// 입력을 했는데 제대로 입력이 안됬으면
			break;									// 탈출

		len = strlen(buf);
		if ('\n' == buf[len - 1])	// 엔터로 입력하면 맨 뒤는 '\n'이 입력되지만 
			buf[len - 1] = '\0';	//'\n'을 '\0'으로 바꿔서 문자열의 끝을 의미하게끔 한다.
		if (0 == strlen(buf))		// 입력한 문자 없이 엔터만 입력하면
			break;					// 탈출

		retval = send(sock, buf, strlen(buf), 0);	// sock에 연결된 주소로 buf에 있는 문자열을 보낸다.
		if (SOCKET_ERROR == retval)					// 보내지지 않았다면
		{
			err_display(L"send()");
			break;
		}

		printf("[TCP 클라이언트] : %d바이트를 보냈습니다.\n", retval);	// 보내졌다면 보낸 데이터 크기를 출력

		retval = recvn(sock, buf, retval, 0);	// sock에 연결된 곳에서 보낸 데이터가 있으면 buf에 저장한다.
												// recv()가 아니라 직접 만들어준 recvn()으로 받는데 설명은 recvn()을 보도록
		if (SOCKET_ERROR == retval)				// 제대로 받지 못하면
		{
			err_display(L"recv()");
			break;
		}
		else if (0 == retval)					// 받은 값이 없으면
			break;

		buf[retval] = '\0';
		printf("[TCP 클라이언트] : %d바이트를 받았습니다.\n", retval);
		printf("[받은 데이터] : %s\n", buf);

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
//			  connect()로 호출하면 serveraddr에 저장된 IP 주소로 접속을 요청하고,
//			  접속이 되면 운영체제가 자동으로 IP 주소와 포트 번호를 할당해준다.
//			  포트번호는 Windows 버전에 따라 다르다.
// listen() : 소켓의 TCP 포트 상태를 LISTENING으로 바꾼다.
//			  서버가 아니기 때문에 다른 클라이언트의 연결을 대기하고 있어야할 이유가 없다.
//			  클라이언트에서 서버를 만들 수 있는 기능을 제공한다면 사용할 수 있다.
// accept() : 접속한 클라이언트와 통신할 수 있도록 새로운 소켓을 생성해서 리턴한다.
//			  이 역시 서버가 아니기 때문에 다른 클라이언트의 연결을 허락하고 말고가 없다.
//			  