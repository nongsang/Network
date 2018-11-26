#pragma comment(lib, "ws2_32")
//#include <WinSock2.h>			// WS2tcpip.h�� ���ԵǾ� �����Ƿ� ���� �߰� �����൵ �ȴ�.
#include <WS2tcpip.h>
#include <stdio.h>

// �����ڵ� ���� ����

int main()
{
	WSADATA wsa;

	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
		return 1;

	MessageBox(NULL, "���� �ʱ�ȭ", "�˸�", MB_OK);

	const char * ipv4test = "147.46.114.70";
	printf("IPv4 �ּ� ��ȯ �� : %s\n", ipv4test);

	printf("IPv4 �ּ� ��ȯ �� : 0x%x", inet_addr(ipv4test));	// IPv4 �ּҸ� ��Ÿ���� ���ڿ��� ���̳ʸ� ������ �ٲ㼭 ���

	IN_ADDR ipv4num;										// IPv4 �ּҸ� �����ϴ� ����ü
	//ipv4num.S_un.S_addr = inet_addr(ipv4test);			// S_un.S_addr�� ���̳ʸ� ���� �����Ѵ�.
	ipv4num.s_addr = inet_addr(ipv4test);					// S_un.S_addr�� s_addr�� ����� �����ϴ�.

	printf("IPv4 �ּ� �ٽ� ��ȯ �� : %s\n", inet_ntoa(ipv4num));	// ���̳ʸ� ���� IPv4�� �ٲ㼭 ���

	printf("\n");

	const char * ipv6test = "2001:0230:abcd:ffab:0023:eb00:ffff:1111";	// ��Ƽ����Ʈ
	//LPCWSTR ipv6test = L"2001:0230:abcd:ffab:0023:eb00:ffff:1111";	// �����ڵ�
	printf("IPv6 �ּ� ��ȯ �� : %s\n", ipv6test);

	SOCKADDR_IN6 ipv6sock;							// IPv6�� ���� �ּ� ����ü
	int addrlen = sizeof(ipv6sock);
	WSAStringToAddress((LPWSTR)ipv6test, AF_INET6, NULL, (SOCKADDR *)&ipv6sock, &addrlen);	// IPv6 �ּҸ� ���̳ʸ� ������ �ٲ㼭 ����
																							// �ܼ��� LPCWSTR���� (LPWSTR)�� ����ȯ�Ѵٰ� �Է��� ����� ���� �ʴ´�.
																							// �߰��۾��� ����� �Ѵ�.

	printf("IPv6 �ּ� ��ȯ �� : 0x");
	for (int i = 0; i < 16; i++)						// ���̳ʸ� ���� �迭 ���·� ������ �Ǿ������Ƿ�
		printf("%02x", ipv6sock.sin6_addr.u.Byte[i]);	// for������ ���
	printf("\n");

	char ipaddr[50];
	DWORD ipaddrlen = sizeof(ipaddr);
	WSAAddressToString((SOCKADDR*)&ipv6sock, sizeof(ipv6sock), NULL, (LPWSTR)ipaddr, &ipaddrlen);	// ���̳ʸ� ���� IPv6 �ּҷ� �ٲ㼭 ����
	printf("IPv6 �ּ� �ٽ� ��ȯ �� : %s\n", ipaddr);

	WSACleanup();

	return 0;
}

// WSAStringToAddress(), WSAAddressToString()�� IPv4, IPv6 ���θ� Ŀ���� �� �ִ�.
// ������ Windows���� �Լ��̹Ƿ� POSIX �迭 OS(������ ��)������ ����� �Ұ����ϴ�.
// �Դٰ� ��Ƽ����Ʈ�� ����� ���� ��� ������ �����ڵ带 ����ϸ� �߰� �۾��� �ؾ� �Ѵ�.
// ipv6sock�� const���� WSAStringToAddress()�� ������ ���� const�� ���� �Ѵ�.
// (LPWSTR)�� �Ͽ� const�� �����ؼ� �����ص� ����� �Է��� �� �� ����.
// ������ �����ڵ�� ��ȯ�Ͽ� �����ص� ����ε� ���� ������ �ʴ´�.
// WSAStringToAddress(), WSAAddressToString()ó�� IPv4, IPv6 ���θ� Ŀ���� �� �ְ� ��� �÷������� ����� �� �ִ� �ٸ� �Լ��� ������?