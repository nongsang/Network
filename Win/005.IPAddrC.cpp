#pragma comment(lib, "ws2_32")
//#include <WinSock2.h>			// WS2tcpip.h에 포함되어 있으므로 따로 추가 안해줘도 된다.
#include <WS2tcpip.h>
#include <stdio.h>

// 유니코드 문자 집합

int main()
{
	WSADATA wsa;

	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
		return 1;

	MessageBox(NULL, "윈속 초기화", "알림", MB_OK);

	const char * ipv4test = "147.46.114.70";
	printf("IPv4 주소 변환 전 : %s\n", ipv4test);

	printf("IPv4 주소 변환 후 : 0x%x", inet_addr(ipv4test));	// IPv4 주소를 나타내는 문자열을 바이너리 값으로 바꿔서 출력

	IN_ADDR ipv4num;										// IPv4 주소를 저장하는 구조체
	//ipv4num.S_un.S_addr = inet_addr(ipv4test);			// S_un.S_addr에 바이너리 값을 저장한다.
	ipv4num.s_addr = inet_addr(ipv4test);					// S_un.S_addr는 s_addr로 축약이 가능하다.

	printf("IPv4 주소 다시 변환 후 : %s\n", inet_ntoa(ipv4num));	// 바이너리 값을 IPv4로 바꿔서 출력

	printf("\n");

	const char * ipv6test = "2001:0230:abcd:ffab:0023:eb00:ffff:1111";	// 멀티바이트
	//LPCWSTR ipv6test = L"2001:0230:abcd:ffab:0023:eb00:ffff:1111";	// 유니코드
	printf("IPv6 주소 변환 전 : %s\n", ipv6test);

	SOCKADDR_IN6 ipv6sock;							// IPv6의 소켓 주소 구조체
	int addrlen = sizeof(ipv6sock);
	WSAStringToAddress((LPWSTR)ipv6test, AF_INET6, NULL, (SOCKADDR *)&ipv6sock, &addrlen);	// IPv6 주소를 바이너리 값으로 바꿔서 저장
																							// 단순히 LPCWSTR에서 (LPWSTR)로 형변환한다고 입력이 제대로 되지 않는다.
																							// 추가작업을 해줘야 한다.

	printf("IPv6 주소 변환 후 : 0x");
	for (int i = 0; i < 16; i++)						// 바이너리 값이 배열 형태로 저장이 되어있으므로
		printf("%02x", ipv6sock.sin6_addr.u.Byte[i]);	// for문으로 출력
	printf("\n");

	char ipaddr[50];
	DWORD ipaddrlen = sizeof(ipaddr);
	WSAAddressToString((SOCKADDR*)&ipv6sock, sizeof(ipv6sock), NULL, (LPWSTR)ipaddr, &ipaddrlen);	// 바이너리 값을 IPv6 주소로 바꿔서 저장
	printf("IPv6 주소 다시 변환 후 : %s\n", ipaddr);

	WSACleanup();

	return 0;
}

// WSAStringToAddress(), WSAAddressToString()은 IPv4, IPv6 전부를 커버할 수 있다.
// 하지만 Windows전용 함수이므로 POSIX 계열 OS(리눅스 등)에서는 사용이 불가능하다.
// 게다가 멀티바이트를 사용할 때는 상관 없으나 유니코드를 사용하면 추가 작업을 해야 한다.
// ipv6sock는 const지만 WSAStringToAddress()에 전달할 때는 const를 빼야 한다.
// (LPWSTR)를 하여 const를 삭제해서 전달해도 제대로 입력을 할 수 없다.
// 실제로 유니코드로 변환하여 전달해도 제대로된 값이 나오지 않는다.
// WSAStringToAddress(), WSAAddressToString()처럼 IPv4, IPv6 전부를 커버할 수 있고 모든 플랫폼에서 사용할 수 있는 다른 함수는 없을까?