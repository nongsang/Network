#pragma comment(lib, "ws2_32")
#include <WinSock2.h>

void err_quit(char *msg)
{
	LPVOID lpMsgBuf;	// 모든 타입을 가리킬 수 있는 포인터
	FormatMessage(		// 에러가 코드로 나타나서 뭔지 모르니까 메세지 형태로 바꿔서 창을 띄워주는 함수
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,	// 운영체제로 부터 오류 메세지를 받아서 저장할 공간을 알아서 할당한다.
		NULL, WSAGetLastError(),	// 윈속에 관한 오류를 감지하면 호출,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	// 사용자 언어(LANG_NEUTRAL)로, 그중에서 제어판에 설정되어있는 언어(SUBLANG_DEFAULT)로 오류값을 나타내고
		(LPTSTR)&lpMsgBuf, 0, NULL	// 문자열을 포인터로 가리키게 한다.
	);
	MessageBox(NULL, (LPCSTR)lpMsgBuf, msg, MB_ICONERROR);	// 포인터로, 입력받은 문자열을 출력하게 한다.
	LocalFree(lpMsgBuf);		// 로컬 메모리를 해제한다.
	exit(1);
}

int main()
{
	// 윈속 초기화 설명은 01. Server에서
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	MessageBox(NULL, "윈속 초기화 성공", "알림", MB_OK);

	SOCKET tcp_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);		// 주소 체계는 IPv4, 소켓 타입은 TCP, 사용할 프로토콜은 IPPROTO_TCP로 명시해도 되고, 0으로 놔도 TCP나 UDP는 프로토콜을 결정할 수 있다.
	if (tcp_sock == INVALID_SOCKET) err_quit("socket()");		// 인식 불가 소켓이면 오류와 함께 종료
	MessageBox(NULL, "소켓 생성 성공", "알림", MB_OK);

	closesocket(tcp_sock);			// 소켓을 생성했으면 소켓을 닫아야 한다.

	WSACleanup();
	return 0;
}