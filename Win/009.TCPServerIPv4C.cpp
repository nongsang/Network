#pragma comment(lib, "ws2_32")
//#include <WinSock2.h>			// WS2tcpip.h에 포함되어 있으므로 따로 추가 안해줘도 된다.
#include <WS2tcpip.h>
#include <stdio.h>

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

int main()
{
	WSADATA wsa;

	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
		return 1;

	MessageBox(NULL, L"윈속 초기화", L"알림", MB_OK);

	int retval;		// 에러 코드를 저장할 변수

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);		// 주소체계는 IPv4, 타입은 SOCK_STREAM인 소켓 생성
	if (INVALID_SOCKET == listen_sock) err_quit(L"socket()");	// 소켓 생성이 안되면 에러 메세지와 함께 종료

	SOCKADDR_IN serveraddr;						// IPv4 소켓 주소 구조체
	memset(&serveraddr, 0, sizeof(serveraddr));	// 왜 ZeroMemory()가 아니라 memset인지는 008.DomainCPP.cpp를 보면 된다.

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);	// 서버의 경우 클라이언트가 IPv4, IPv6 어떤 주소로도 접속할 수 있도록
													// INADDR_ANY로 정해주는 것이 바람직하다.
													// 서버이므로 접속할 주소를 저장하는게 아니라 아무 접속이나 받는다는 의미
	serveraddr.sin_port = htons(SERVERPORT);		// 서버에서 사용할 소켓의 포트번호 

	//retval = bine(listen_sock, (SOCKADDR*)&serveraddr, sizeof(SOCKADDR_IN));	// 만약 이처럼 자료형의 크기를 전달해주는 식으로 코딩을 하면
																				// SOCKADDR_IN이 아니라 SOCKADDR_INDA로 바뀌었을 때
																				// sizeof(SOCKADDR_INDA)로 바꾸는 번거로움이 있다.
																				// 그냥 sizeof(변수명)으로 하자.
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));	// 생성한 소켓과 주소, 포트번호를 가진 소켓 구조체를 묶는다.
	if (SOCKET_ERROR == retval) err_quit(L"bind()");						// 실패하면 에러, 성공하면 0 반환

	retval = listen(listen_sock, SOMAXCONN);			// 소켓의 TCP 포트 상태를 LISTENING으로 바꾼다.
														// SOMAXCONN은 접속 가능한 클라이언트의 최대 갯수를 나타낸다.
	if (SOCKET_ERROR == retval) err_quit(L"listen()");	// 실패하면 에러, 성공하면 0 반환

	SOCKET client_sock;			// 접속한 클라이언트의 소켓
	SOCKADDR_IN clientaddr;		// 접속한 클라이언트의 정보
	int addrlen;				// SOCKADDR_IN의 길이를 담을 변수
								// 전달할 때 주소값으로 전달할 거니까 없으면 안된다.
	char buf[BUFSIZE];

	while (1)
	{
		// 접속한 클라이언트가 있는지 무한으로 확인
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);	// LISTENING으로 되어있는 소켓(listen_sock)에
																				// 접속을 요청한 client_sock에 접속 허가를 내린다.
																				// 접속을 요청한 클라이언트의 IP 정보는 clientaddr에 저장된다.
		if (INVALID_SOCKET == client_sock)	// 접속 허가가 실패하면
		{
			err_display(L"accept()");
			break;
		}

		printf("[TCP 서버] 클라이언트 접속 : IP 주소 = %s, 포트 번호 = %d\n",	// 클라이언트가 접속에 성공하면 접속한 클라이언트의 IP 주소와 포트 번호를 출력
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// 여기부터는 서버와 접속한 클라이언트간의 무한 데이터 교환
		while (1)
		{
			retval = recv(client_sock, buf, BUFSIZE, 0);
			if (INVALID_SOCKET == retval)		// 데이터를 받는데 실패할 시
			{
				err_display(L"recv()");
				break;
			}
			else if (0 == retval)				// 연결이 끊어졌을 때
				break;

			buf[retval] = '\0';					// retval은 recv()가 성공적으로 끝나면, 받은 데이터의 수를 저장한다.
												// buf[retval]이란 뜻은 받은 데이터의 끝을 의미한다.
												// 받은 데이터가 8바이트면 retval은 8이며 buf[8] = '\0'이 되고,
												// buf[0] ~ buf[7]에는 각 문자가 저장되어 있다.
			printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);	// 받은 데이터 출력

			retval = send(client_sock, buf, retval, 0);		// 클라이언트 소켓으로 buf의 내용을 retval 길이 만큼 보낸다.
															// 사실 클라이언트에 클라이언트가 보냈던 내용을 다시 보내는 기능
			if (SOCKET_ERROR == retval)						// 데이터를 보내는데 실패할 시
			{
				err_display(L"send()");
				break;
			}
		}

		closesocket(client_sock);				// 클라이언트가 종료하면 클라이언트를 받을 소켓을 닫아줘야 한다.
		printf("[TCP 서버] 클라이언트 종료 : IP 주소 = %s, 포트 번호 = %d\n",	// 클라이언트가 종료하면 종료한 클라이언트의 IP 주소와 포트 번호를 출력
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}

	closesocket(listen_sock);		// 프로그램을 종료하기 전에 접속 허가를 줄 소켓을 닫아줘야 한다.

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