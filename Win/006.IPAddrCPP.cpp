#pragma comment(lib, "ws2_32")
//#include <WinSock2.h>			// WS2tcpip.h에 포함되어 있으므로 따로 추가 안해줘도 된다.
#include <WS2tcpip.h>
#include <iostream>

// 유니코드 문자 집합

using namespace std;

int main()
{
	WSADATA wsa;

	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
		return 1;

	MessageBox(NULL, L"윈속 초기화", L"알림", MB_OK);

	//LPCWSTR ipv4test1 = L"147.46.114.70";					// 유니코드를 사용하고싶다면 이걸
	const char * ipv4test1 = "147.46.114.70";				// C++는 문자열을 const로 사용해야 한다.

	cout << "IPv4 주소 변환 전 : " << ipv4test1 << endl;

	IN_ADDR ipv4num;										// IPv4 주소를 저장하는 구조체

	//inet_pton(AF_INET, ipv4test1, &ipv4num);				// inet_addr() 대신 사용가능한 ANSI 표준 함수
															// inet_addr()와 다른 점은 inet_addr()은 빅엔디안으로 바꾼 값이 무엇인지 알려주기만 한다.
															// 값을 저장할 변수가 없어도 상관없다.
															// inet_pton()은 주소를 저장할 IN_ADDR형 변수가 있어야 한다.
															// 실제로 빅엔디안으로 바꾼 값을 저장하는 기능을 한다.
															// ipv4num에 ipv4test1을 AF_INET에 맞추어 저장한다는 의미이다.
	inet_pton(AF_INET, ipv4test1, &ipv4num.s_addr);			// 위의 문장과 같은 의미
	//InetPton(AF_INET, (LPCWSTR)ipv4test1, &ipv4num);		// inet_pton()과 같은 기능을 하는 Windows 전용 함수
															// 문자열을 멀티바이트와 유니코드를 구분해서 전달해야 한다.
															// 문자 집합이 멀티바이트면 상관없지만 유니코드면 전달하는 값 자체가 유니코드여야 한다.
	//cout << "IPv4 주소 변환 후 : " << hex << "0x" << ipv4num << endl;		// IPv4 주소를 바이너리 값으로 바꿔서 출력
																			// C++에서는 IN_ADDR형을 출력할 수 없으므로 오버로딩해야 가능
	cout << "IPv4 주소 변환 후 : " << hex << "0x" << ipv4num.s_addr << endl;	// 정확하게 s_addr를 출력하면 된다.

	char ipv4test2[16];													// 바이너리 값을 변환하여 저장할 변수는 배열로 받는다.
	//inet_ntop(AF_INET, &ipv4num, ipv4test2, INET_ADDRSTRLEN);			// inet_ntoa() 대신 사용가능한 ANSI 표준 함수
																		// INET_ADDRSTRLEN은 22, IPv4는 길이가 15지만 그냥 사용하자.
																		// inet_ntoa()와 다른 점은 inet_ntoa()는 시스템 정렬 방식으로 바꾼 값이 무엇인지 알려주기만 한다.
																		// 값을 저장할 변수가 없어도 상관없다.
																		// inet_ntop()는 주소를 저장할 char형 배열이 필요하다.
																		// 실제로 시스템 정렬로 바꾼 값을 저장하는 기능을 한다.
	inet_ntop(AF_INET, &ipv4num.s_addr, ipv4test2, INET_ADDRSTRLEN);	// 위의 문장과 같은 의미
	//InetNtop(AF_INET, &ipv4num, (LPCWSTR)ipv4test2, INET_ADDRSTRLEN);	// inet_ntop와 같은 기능을 하는 Windows 전용 함수
																		// 문자 집합이 멀티바이트면 상관없지만 유니코드면 전달하는 값 자체가 유니코드여야 한다.

	cout << "IPv4 주소 다시 변환 후 : " << ipv4test2 << endl;

	cout << endl;

	const char * ipv6test1 = "2001:0230:abcd:ffab:0023:eb00:ffff:1111";
	cout << "IPv6 주소 변환 전 : " << ipv6test1 << endl;

	IN6_ADDR ipv6num;								// IPv6 주소를 저장하는 구조체, IPv6의 소켓 주소 구조체가 아니다.
	//SOCKADDR_IN6 ipv6sock;							// IPv6 소켓 주소 구조체
	//inet_pton(AF_INET6, ipv6test1, &ipv6num);		// inet_pton는 IPv4 뿐만 아니라 IPv6도 지원한다.
	inet_pton(AF_INET6, ipv6test1, &ipv6num.u.Byte);// 위의 문장과 같은 의미
	//inet_pton(AF_INET6, ipv6test1, &ipv6sock.sin6_addr.u.Byte);	// IPv6 소켓 주소 구조체를 사용한다면 이걸 사용해야 한다.
																	// 중간에 sin6_addr가 더 추가되어 있다.

	cout << "IPv6 주소 변환 후 : 0x";
	for (int i = 0; i < 8; i++)					// 바이너리 값이 배열 형태로 저장이 되어있으므로
	{
		//printf("%02x", ipv6num.u.Byte[i]);	// for문으로 출력
												// printf()는 주소값이 잘 나오는데 cout은 제대로 출력되지 않는다.
		cout << ipv6num.u.Word[i];				// 이걸 사용하면 cout에도 잘 나오지만 빅엔디안 방식으로 출력된다.
		//cout << ipv6num.sin6_addr.u.Word[i];	// IPv6 소켓 주소 구조체를 사용한다면 이걸 사용해야 한다.
												// 중간에 sin6_addr가 더 추가되어 있다.
	}
	cout << endl;

	char ipv6test2[46];
	//inet_ntop(AF_INET6, &ipv6num, ipv6test2, INET6_ADDRSTRLEN);		// 바이너리 값을 IPv6로 바꿔서 출력
																		// INET6_ADDRSTRLEN은 65, IPv6은 길이가 45지만 그냥 사용하자. 
	inet_ntop(AF_INET6, &ipv6num.u.Byte, ipv6test2, INET6_ADDRSTRLEN);	// 위의 문장과 같은 의미
	//inet_ntop(AF_INET6, &ipv6num.sin6_addr.u.Byte, ipv6test2, INET6_ADDRSTRLEN);	// IPv6 소켓 주소 구조체를 사용한다면 이걸 사용해야 한다.

	cout << "IPv6 주소 다시 변환 후 : " << ipv6test2 << endl;

	WSACleanup();

	return 0;
}

// inet_pton(), inet_ntop()는 IPv4, IPv6 전부 커버 가능하다.
// ANSI표준으로 Windows에 종속적이지 않고 POSIX 계열 OS(리눅스 등)에서 전부 사용이 가능하다.
// Windows 전용으로는 InetPton()과 InetNtop()가 있다.
// 멀티바이트와 유니코드를 구분해서 써야 한다.