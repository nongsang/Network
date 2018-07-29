#pragma comment(lib, "ws2_32")	// 라이브러리 링크시켜준다. 이게 있으면 속성에서 링커에서 ws2_32.lib를 추가하지 않아도 된다.
#include <WinSock2.h>	// 윈도우 소켓 API를 쓰기 위한 헤더, winsock.h은 윈도우 3.1이전 시스템과 호환성을 위해서 있다.

int main()
{
	WSADATA wsa;	// 윈속 초기화 정보 구조체
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)	// 프로그램에 사용할 윈속 버전을 요청하여, 윈속 라이브러리(WS2_32.DLL)를 초기화
		return 1;		// 성공하면 0을 반환, 실패하면 1을 반환
	MessageBox(NULL, "윈속 초기화 성공", "알림", MB_OK);	// 속성에서 문자집합을 유니코드가 아니라 멀티바이트로 바꿔줘야 한글을 제대로 입력받는다.
								// 32비트, debug모드 기준이며, 64비트이거나, release면 LPCWSTR형 문자만 받을 수 있다. 
	WSACleanup();	// 윈속 종료
	return 0;
}
