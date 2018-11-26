#pragma comment(lib, "ws2_32")
#include <WS2tcpip.h>
#include <iostream>

// 유니코드 문자 집합

using namespace std;

void err_display(LPCWSTR msg)
{
	LPVOID lpMsgBuf;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		LANG_USER_DEFAULT,
		(LPWSTR)&lpMsgBuf
		, 0, NULL
	);

	MessageBox(NULL, (LPCWSTR)lpMsgBuf, msg, MB_ICONERROR);		// 메세지 박스로 보고싶으면 사용
	//cout << "[" << msg << "] " << (LPCWSTR)lpMsgBuf << endl;	// 콘솔에 에러코드가 출력된다.
																// 말 그대로 에러코드가 출력되니 가급적 메세지 박스를 사용하자.

	LocalFree(lpMsgBuf);
}

int main()
{
	WSADATA wsa;		// 초기화 정보

	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
		return 1;

	MessageBox(nullptr, L"윈속 초기화", L"알림", MB_OK);

	const char * TestName = "www.naver.com";
	//const char * TestName = "cvsmtppost001.nm.naver.com";		// IP -> 도메인 이름 변환할 때 제대로된 변환을 보고 싶으면 사용

	cout << "도메인 이름 변환 전 : " << TestName << endl;

	ADDRINFO hint;						// 호스트의 정보를 저장하는 구조체
	ZeroMemory(&hint, sizeof(hint));	// 모든 변수를 0으로 초기화해야 한다.
										// 꼭 해야하는 작업이다.
										// 안하면 제대로 사용을 할 수 없다.
	//hint.ai_flags = AI_CANONNAME;		// ai_flags는 옵션이며, ai_canonname 필드에 호스트의 정식명칭을 전달
	hint.ai_flags = AI_FQDN;			// ai_canonname 필드에 호스트의 별명을 전달
	hint.ai_family = AF_UNSPEC;			// 접속할 호스트가 TCP인지 UDP인지 상관하지 않겠다.
	hint.ai_socktype = SOCK_STREAM;		// 타입은 TCP
	hint.ai_protocol = IPPROTO_TCP;		// 프로토콜은 TCP지만 0으로 둬도 상관없다.

	ADDRINFO * result = nullptr;		// HOSTENT처럼 호스트를 가리킬 변수

	getaddrinfo(TestName, "80", &hint, &result);	// gethostbyname() 대신에 사용할 수 있는 ANSI 표준 함수
													// 호스트에 포트번호(80번은 HTTP)로 hint에 있는 프로토콜로 result가 가리키게 한다.
													// 0이면 성공, 그 이외의 값은 에러코드
													// gethostbyname_r()이라는 함수도 있는데 윈도우즈에서는 제대로 사용할 수 없다.

	cout << endl;

	//IN_ADDR * ipv4 = (IN_ADDR*)result->ai_addr;	// 호스트의 IP 주소는 매번 바뀌는 경우가 많기 때문에
													// IP 주소 구조체의 포인터로 따로 가리키게 하지 말고 그냥 쓰자
	//cout << inet_ntoa(*ipv4) << endl;				// IN_ADDR * 로 가리키게 해서 IP 주소를 출력하면 이상한 값이 나온다.
													// memcpy()를 사용해도 마찬가지

	SOCKADDR_IN * ipv4sock = (SOCKADDR_IN*)result->ai_addr;						// IP 주소를 IPv4 소켓 구조체의 포인터로 가리키게 할 수 있다.
																				// 데이터를 실제로 주고 받는 구조체인데 가능하겠지.
	cout << "도메인->IP 주소 변환 : " << inet_ntoa(ipv4sock->sin_addr) << endl;	// SOCKADDR_IN으로 호스트를 가리키게 한 후에
																				// 호스트의 IP 주소를 출력할 수 있다.

	cout << endl;

	cout << TestName << "의 명칭 : " << result->ai_canonname << endl;	// hint.ai_flags에 따라 정식 명칭, 혹은 별명이 반환된다.

	char hostname[NI_MAXHOST];		// 호스트의 도메인 이름을 저장할 변수
									// 배열의 길이는 호스트 이름을 최대한 저장할 수 있을 정도이다.
	char servInfo[NI_MAXSERV];		// 서비스 이름을 포트번호로 저장할 변수, HTTP?
									// 배열의 길이는 서비스 이름을 최대한 저장할 수 있을 정도이다.

	cout << endl;

	// gethostbyaddr() 대신에 사용할 수 있는 ANSI 표준 함수
	// ipv4sock이 가리키는 호스트의 주소 전부를
	// hostname에 도메인 이름으로 변환하여 저장하고,
	// servInfo에 서비스 이름(HTTP인가 FTP인가)을 저장하는데
	// 포트 번호로 저장한다.
	getnameinfo((SOCKADDR*)ipv4sock, sizeof(SOCKADDR), hostname, NI_MAXHOST, servInfo, NI_MAXSERV, NI_NUMERICSERV);

	cout << "IP->도메인 주소 변환 : " << hostname << endl;	// DNS 서버에 허용이 되어있지 않으면 제대로된 주소가 나오지 않는다.
															// 메일서버는 출력이 가능
	
	GetAddrInfoW()

	WSACleanup();

	return 0;
}

// getaddrinfo(), getnameinfo()는 ANSI표준으로 POSIX 계열 OS에서 사용이 가능하다.
// ADDRINFO가 가리키는 호스트 주소에 SOCKADDR_IN으로 가리키느냐, SOCKADDR_IN6으로 가리키느냐에 따라 IPv4, IPv6를 사용한다.
// 유니코드를 사용한다면 GetAddrInfoW(), GetNameInfoW()를 사용하면 되지만 Windows 전용 함수다.
// getaddrinfoex(), getnameinfoex()도 있지만 이건 ADDRINFOEX라는 호스트의 추가 정보를 담는 구조체를 사용해야 한다.
// 이 밖에 ADDRINFOEX2, ADDRINFOEX3, ADDRINFOEX4 구조체가 있지만 모두 ADDRINFO에 추가 정보를 저장하는 역할이다.
// ADDRINFOEX를 사용하면 GetAddrInfoEx(), GetNameInfoEx()를 사용해야 한다.
// getaddrinfo(), getnameinfo()는 thread safe하므로 gethostbyname(), gethostbyaddr()를 대체하여 사용한다.
// IPv4만 사용한다면 gethostbyname_r()나 gethostbyaddr_r()가 thread safe하므로 바꿔서 사용하면 되지만 Windows에서는 사용할 수 없다.