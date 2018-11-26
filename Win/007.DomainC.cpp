#pragma comment(lib, "ws2_32")
#include <WS2tcpip.h>
#include <stdio.h>

// 유니코드 문자 집합

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

// IPv4 주소 -> 도메인 이름
BOOL GetIPAddr(const char * name, IN_ADDR * addr)
{
	HOSTENT * ptr = gethostbyname(name);

	if (NULL == ptr)							// 호스트를 찾지 못하면
	{
		err_display(L"gethostbyname");
		return FALSE;
	}
	if (AF_INET != ptr->h_addrtype)				// 찾은 호스트가 IPv4가 아니라면
		return FALSE;

	memcpy(addr, ptr->h_addr, ptr->h_length);	// 포인터 값 끼리의 복사는 memcpy로 해야 한다.
	return TRUE;
}

// 도메인 이름 -> IPv4
BOOL GetDomainName(IN_ADDR addr, char * name, int namelen)
{
	HOSTENT * ptr = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);	// addr에 저장된 IP 주소로 호스트를 찾아 저장한다.

	if (NULL == ptr)						// 페이지가 없으면
	{
		err_display(L"gethostbyaddr");
		return FALSE;
	}
	if (AF_INET != ptr->h_addrtype)			// IPv4가 아니라면
		return FALSE;

	strncpy(name, ptr->h_name, namelen);	// 문자열 끼리의 복사는 strcpy로 해야 한다.
	return TRUE;
}

int main()
{
	WSADATA wsa;

	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
		return 1;

	MessageBox(NULL, L"윈속 초기화", L"알림", MB_OK);

	//const char * TestName = "www.naver.com";				// 유명한 페이지는 원하는 결과를 얻기 힘들다.
	const char * TestName = "cvsmtppost001.nm.naver.com";	// IP -> 도메인 이름 변환할 때 제대로된 변환을 보고 싶으면 사용
															// 네이버 메일인 듯

	printf("도메인 이름 변환 전 : %s\n", TestName);

	printf("\n");

	// 도메인 -> IP 주소 변환 방법 0번째
	printf("도메인 -> IP 주소 변환 방법 첫번째\n");
	HOSTENT * host1 = gethostbyname(TestName);				// 호스트의 정보를 받아서 HOSTENT형 변수의 포인터가 가리키게 한다.
	printf("inet_ntoa(*(IN_ADDR*)domain1->h_addr : %s\n", inet_ntoa(*(IN_ADDR*)host1->h_addr));	// 호스트 정보의 IP 주소를
																								// IN_ADDR로 나눠서 변환하고 다시 문자열로 변환하여 출력

	printf("\n");

	// 도메인 -> IP 주소 변환 방법 1번째
	printf("도메인 -> IP 주소 변환 방법 두번째\n");
	IN_ADDR addr1;										// 페이지에서 추출한 IP 주소를 저장할 변수

	//IP.s_addr = *domain1->h_addr_list[0];				// 이 방법은 제일 앞부분 IP값만 추출하여 변환해준다.
														// 왜냐하면 h_addr_list는 각 IP주소 구간을 원소로 하여 배열을 이루고 있다.
														// 그렇기 때문에 각 원소를 s_addr의 각 원소에 for문으로 일일히 넣어줘야 한다.
	memcpy(&addr1, host1->h_addr, host1->h_length);		// memcpy를 하면 모든 IP값을 복사한다.
	printf("memcpy(&IP, domain1->h_addr, domain1->h_length) : %s\n", inet_ntoa(addr1));	// 추출한 호스트의 IP 주소

	printf("\n");

	printf("%s의 정식 명칭 : %s\n", TestName, host1->h_name);	// TestName에 정의된 호스트의 정식 명칭
																// 정식명칭으로 접속해도 원하는 페이지가 아닐 수 있다.

	printf("\n");

	printf("%s의 별명들\n", TestName);
	for (int i = 0; host1->h_aliases[i]; i++)					// 접근했을 때 별명이 없을 때까지
		printf("%d번째 : %s\n", i, host1->h_aliases[i]);			// 별명 출력

	printf("\n");

	// 도메인 -> IP 주소 변환 방법 2번째
	printf("도메인 -> IP 주소 변환 방법 세번째\n");

	IN_ADDR addr2;										// 페이지에서 추출한 IP 주소를 저장할 변수
	if (GetIPAddr(TestName, &addr2))					// 두번째 방법을 함수로 만들었을 뿐
		printf("GetIPAddr(TestName, &addr) : %s\n", inet_ntoa(addr2));

	printf("\n");

	printf("IP -> 도메인 주소 변환 방법 첫번째\n");
	HOSTENT * host2 = gethostbyaddr((char*)&addr2, sizeof(host2), AF_INET);	// addr2에 저장된 IP 주소의 정보를 추출
																			// addr2는 IN_ADDR이므로 char*로 나눠서 전달해야 한다.

	printf("host2->h_name : %s\n", host2->h_name);	// Reverse DNS Lookup(리버스 도메인)을 이용하여 도메임 이름을 알아낸다.
													// 문제는 DNS 서버에 접근 허용이 되어 있어야 도메인 이름을 찾을 수 있다.
													// 요즘은 서버에서 접근을 허용을 하지 않는 추세라서 거의 제대로된 값이 나오지 않는다.
													// 네이버 같이 유명한 페이지로 테스트해보면 NULL이거나 제대로된 값이 아닐 수 있다.
													// 따라서 테스트를 할 수 있는 도메인 이름을 위에 추가로 적었다.

	printf("\n");

	printf("IP -> 도메인 주소 변환 방법 두번째\n");
	char name[256];										// IP 주소로 찾은 호스트의 이름을 저장할 배열
	if (GetDomainName(addr2, name, sizeof(name)))		// 첫번째 방법을 함수로 만들었을 뿐
		printf("GetDomainName(addr2, name, sizeof(name) : %s\n", name);

	WSACleanup();

	return 0;
}

// gethostbyname(), gethostbyaddr()는 ANSI표준으로 POSIX 계열 OS에서 사용이 가능하다.
// gethostbyname()는 호스트가 IPv4와 IPv6 모두 가지고 있는 경우 무조건 IPv4 주소를 리턴한다.
// 유니코드를 사용한다면 GetHostByNameW(), GetHostByAddrW()를 사용하면 되지만 Windows 전용이다.
// 지금은 IPv4만 사용하니 gethostbyname()도 딱히 문제는 없지만 IPv6을 사용할 때는 다른 방법이 필요하다.