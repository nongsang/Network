#pragma comment(lib, "ws2_32")
#include <WinSock2.h>

void err_quit(LPCWSTR msg)
{
	LPVOID lpMsgBuf;												// 에러 메세지를 저장할 포인터

	FormatMessage(													// Error가 발생하면 코드 형태로 출력되는데, 코드를 문자열로 바꿔준다.
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,// 시스템에 등록되어 있는 메세지를 출력하며, 메세지를 담을 메모리는 자동으로 할당
		NULL, WSAGetLastError(),									// WSA에서 발생한 가장 마지막 에러를 검출하여 에러코드를 발생하고,
		//MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),				// 발생한 에러코드는 사용자 언어 중 현재 시스템에 설정된 언어로 변경하여,
		LANG_USER_DEFAULT,											// MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)과 같은 의미다.
		(LPWSTR)&lpMsgBuf,											// lpMsgBuf에 저장한다. 
		0, NULL														// FORMAT_MESSAGE_ALLOCATE_BUFFER로 메모리를 자동 할당했기 때문에 버퍼 크기는 0이다.
	);

	MessageBox(NULL, (LPCWSTR)lpMsgBuf, msg, MB_ICONERROR);			// 에러 메세지 출력

	LocalFree(lpMsgBuf);											// FORMAT_MESSAGE_ALLOCATE_BUFFER를 사용하여 자동으로 메모리를 할당하면
																	// LocalFree로 메모리를 해제해줘야 한다.
																	// 버퍼 크기를 직접 정의해주면 사용하지 않아도 된다.

	exit(1);														// 프로그램 종료
}

int main()
{
	// 윈속 초기화, 설명은 01. Server에서
	WSADATA wsa;

	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
		return 1;

	MessageBox(NULL, L"윈속 초기화 성공", L"알림", MB_OK);

	SOCKET tcp_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);// 소켓 정의
																// IPv4(AF_INET)를 사용
																// TCP(SOCK_STREAM) 사용
																// 프로토콜은 IPPROTO_TCP
																// UDP를 구현할 때는 IPPROTO_UDP를 사용하면 되지만 IPPROTO_TCP와 같이 0이기 때문에
																// 그냥 프로토콜 부분은 0으로 해도 된다.
																// 하지만 IPPROTO_TCP는 6, IPPROTO_UDP는 17, 0은 IPPROTO_HOPOPTS인데 어떻게 같다는걸까?

	if (INVALID_SOCKET == tcp_sock) err_quit(L"socket");		// 소켓 생성에 실패하면 에러 메세지

	MessageBox(NULL, L"TCP 소켓 생성 성공", L"알림", MB_OK);

	closesocket(tcp_sock);										// 생성한 소켓은 닫아줘야 한다.

	WSACleanup();												// 소켓을 닫은 후에 윈속을 종료해야 한다.

	return 0;
}