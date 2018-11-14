#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <iostream>

using namespace std;

int main()
{
	u_short x1 = 0x1234;		// WinSock2.h에 정의되어 있는 자료형
	u_long y1 = 0x12345678;	// u_short는 unsigned short, u_long는 unsigned long이다.
	u_short x2;
	u_long y2;

	cout << hex << "[호스트 바이트 -> 네트워크 바이트]" << endl;	// C++은 기본이 정수 출력이기 때문에 16진수로 출력하기 위해 hex 사용
	x2 = htons(x1);						// host to network short, 네트워크가 연결되어 있지 않아도 변환된다.
	y2 = htonl(y1);						// host to network long
	cout << x1 << " -> " << x2 << endl;	// Intel CPU라면 바뀌어서 출력할 것이고,
	cout << y1 << " -> " << y2 << endl;	// AMD CPU라면 바뀌지 않고 출력할 것이다.

	cout << endl;

	cout << "[네트워크 바이트 -> 호스트 바이트]" << endl;
	cout << x2 << " -> " << ntohs(x2) << endl;	// network to host short, 네트워크가 연결되어 있어야 변환된다. 
	cout << y2 << " -> " << ntohl(y2) << endl;	// network to host long

	cout << endl;

	cout << "[잘못된 사용 예]" << endl;
	cout << x1 << " -> " << htonl(x1) << endl;	// u_short값을 u_long값으로 변환하기 때문에 안된다.

	return 0;
}