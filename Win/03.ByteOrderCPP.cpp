#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <iostream>

using namespace std;

int main()
{
	u_short x1 = 0x1234;		// WinSock2.h�� ���ǵǾ� �ִ� �ڷ���
	u_long y1 = 0x12345678;	// u_short�� unsigned short, u_long�� unsigned long�̴�.
	u_short x2;
	u_long y2;

	cout << hex << "[ȣ��Ʈ ����Ʈ -> ��Ʈ��ũ ����Ʈ]" << endl;	// C++�� �⺻�� ���� ����̱� ������ 16������ ����ϱ� ���� hex ���
	x2 = htons(x1);						// host to network short, ��Ʈ��ũ�� ����Ǿ� ���� �ʾƵ� ��ȯ�ȴ�.
	y2 = htonl(y1);						// host to network long
	cout << x1 << " -> " << x2 << endl;	// Intel CPU��� �ٲ� ����� ���̰�,
	cout << y1 << " -> " << y2 << endl;	// AMD CPU��� �ٲ��� �ʰ� ����� ���̴�.

	cout << endl;

	cout << "[��Ʈ��ũ ����Ʈ -> ȣ��Ʈ ����Ʈ]" << endl;
	cout << x2 << " -> " << ntohs(x2) << endl;	// network to host short, ��Ʈ��ũ�� ����Ǿ� �־�� ��ȯ�ȴ�. 
	cout << y2 << " -> " << ntohl(y2) << endl;	// network to host long

	cout << endl;

	cout << "[�߸��� ��� ��]" << endl;
	cout << x1 << " -> " << htonl(x1) << endl;	// u_short���� u_long������ ��ȯ�ϱ� ������ �ȵȴ�.

	return 0;
}