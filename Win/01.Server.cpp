#pragma comment(lib, "ws2_32")	// 라이브러리 링크시켜준다. 이게 있으면 속성에서 링커에서 ws2_32.lib를 추가하지 않아도 된다.
#include <WinSock2.h>			// 윈도우 소켓 API를 쓰기 위한 헤더, 이전 버전인 winsock.h은 윈도우 3.1이전 시스템과 호환성을 위해서 있다.

int main()
{
	WSADATA wsa;											// 윈속 초기화 정보 구조체

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)				// 프로그램에 사용할 윈속 버전을 요청하여, 윈속 라이브러리(WS2_32.DLL)를 초기화
		return 1;											// 초기화에 성공하면 0을 반환하여 if문을 실행하지 않고 진행,
															// 초기화에 실패하면 오류코드룰 반환하여 if문을 실행하기 때문에 프로그램 종료

	//MessageBox(NULL, "윈속 초기화 성공", "알림", MB_OK);	// 속성에서 문자집합을 멀티바이트로 바꿔주면 호환성으로 한글을 제대로 입력받는다.
															// 멀티바이트는 Windows에만 호환되고 유니코드는 모든 플랫폼에 호환된다.
	MessageBox(NULL, L"윈속 초기화 성공", L"알림", MB_OK);	// 다른 방법으로는 L""을 사용해서 유니코드로 입력해도 된다.

	WSACleanup();	// 윈속 종료
	return 0;
}