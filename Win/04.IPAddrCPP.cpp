#pragma comment(lib, "ws2_32")
//#include <WinSock2.h>			// WS2tcpip.h�� ���ԵǾ� �����Ƿ� ���� �߰� �����൵ �ȴ�.
#include <WS2tcpip.h>
#include <iostream>

// �����ڵ� ���� ����

using namespace std;

int main()
{
	WSADATA wsa;

	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
		return 1;

	MessageBox(NULL, L"���� �ʱ�ȭ", L"�˸�", MB_OK);

	const char * ipv4test = "147.46.114.70";			// C++�� ���ڿ��� const�� ����ؾ� �Ѵ�.
	cout << "IPv4 �ּ� ��ȯ �� : " << ipv4test << endl;

	cout << "IPv4 �ּ� ��ȯ �� : " << hex << "0x" << inet_addr(ipv4test) << endl;	// IPv4 �ּҸ� ���̳ʸ� ������ �ٲ㼭 ���

	IN_ADDR ipv4num;									// IPv4 �ּҸ� �����ϴ� ����ü
	//ipv4num.S_un.S_addr = inet_addr(ipv4test);		// S_un.S_addr�� ���̳ʸ� ���� �����Ѵ�.
	ipv4num.s_addr = inet_addr(ipv4test);				// S_un.S_addr�� s_addr�� ����� �����ϴ�.

	cout << "IPv4 �ּ� �ٽ� ��ȯ �� : " << inet_ntoa(ipv4num) << endl;	// ���̳ʸ� ���� IPv4�� �ٲ㼭 ���

	cout << endl;

	//const char * ipv6test = "2001:0230:abcd:ffab:0023:eb00:ffff:1111";	// ��Ƽ����Ʈ
	LPCWSTR ipv6test = L"2001:0230:abcd:ffab:0023:eb00:ffff:1111";			// �����ڵ�
	cout << "IPv6 �ּ� ��ȯ �� : " << ipv6test << endl;

	SOCKADDR_IN6 ipv6num;							// IPv6�� ���� �ּ� ����ü
	int addrlen = sizeof(ipv6num);
	WSAStringToAddress((LPWSTR)ipv6test, AF_INET6, NULL, (SOCKADDR *)&ipv6num, &addrlen);	// IPv6 �ּҸ� ���̳ʸ� ������ �ٲ㼭 ����
																							// �ܼ��� LPCWSTR���� (LPWSTR)�� ����ȯ�Ѵٰ� �Է��� ����� ���� �ʴ´�.
																							// �߰��۾��� ����� �Ѵ�.
	cout << "IPv6 �ּ� ��ȯ �� : " << "0x";
	for (int i = 0; i < 16; i++)					// ���̳ʸ� ���� �迭 ���·� ������ �Ǿ������Ƿ� 
		cout << ipv6num.sin6_addr.u.Byte[i];		// for������ ���
	cout << endl;

	char ipaddr[50];
	DWORD ipaddrlen = sizeof(ipaddr);
	WSAAddressToString((SOCKADDR*)&ipv6num, sizeof(ipv6num), NULL, (LPWSTR)ipaddr, &ipaddrlen);	// ���̳ʸ� ���� IPv6 �ּҷ� �ٲ㼭 ����
	cout << "IPv6 �ּ� �ٽ� ��ȯ �� : " << ipaddr << endl;

	WSACleanup();

	return 0;
}

// WSAStringToAddress(), WSAAddressToString()�� IPv4, IPv6 ���θ� Ŀ���� �� �ִ�.
// ������ Windows���� �Լ��̹Ƿ� POSIX �迭 OS(������ ��)������ ����� �Ұ����ϴ�.
// �Դٰ� ��Ƽ����Ʈ�� ����� ���� ��� ������ �����ڵ带 ����ϸ� �߰� �۾��� �ؾ� �Ѵ�.
// ipv6test�� const���� WSAStringToAddress�� ������ ���� const�� ���� �Ѵ�.
// (LPWSTR)�� �Ͽ� const�� �����ؼ� �����ص� ����� �Է��� �� �� ����.
// WSAStringToAddress, WSAAddressToStringó�� IPv4, IPv6 ���θ� Ŀ���� �� �ְ� ��� �÷������� ����� �� �ִ� �ٸ� �Լ��� ������?