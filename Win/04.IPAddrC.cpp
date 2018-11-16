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

	MessageBox(NULL, L"���� �ʱ�ȭ", L"�˸�", MB_OK);

	LPCWSTR ipv4test1 = L"147.46.114.70";
	printf("IPv4 �ּ� ��ȯ �� = %s\n", ipv4test1);

	IN_ADDR ipv4num;										// IPv4 �ּҸ� �����ϴ� ����ü

	//inet_pton(AF_INET, ipv4test1, &ipv4num);				// inet_addr ��� ��밡���� ANSI ǥ�� Ȯ�� �Լ�
	inet_pton(AF_INET, ipv4test1, &ipv4num.s_addr);			// ���� ����� ���� �ǹ�
	//InetPton(AF_INET, (LPCWSTR)ipv4test1, &ipv4num);		// inet_pton�� ���� ����� �ϴ� Windows ���� �Լ�
															// ���ڿ��� ��Ƽ����Ʈ�� �����ڵ带 �����ؼ� �����ؾ� �Ѵ�.
															// ���� ������ ��Ƽ����Ʈ�� ��������� �����ڵ�� �����ϴ� �� ��ü�� �����ڵ忩�� �Ѵ�.
	//printf("IPv4 �ּ� ��ȯ �� = 0x%x\n", ipv4num);			// IPv4 �ּҸ� ���̳ʸ� ������ �ٲ㼭 ���, C++������ �����ε��ؾ� ����
	printf("IPv4 �ּ� ��ȯ �� = 0x%x\n", ipv4num.s_addr);	// ���� ����� ���� �ǹ�

	char ipv4test2[16];													// ���̳ʸ� ���� ��ȯ�Ͽ� ������ ������ �迭�� �޴´�.
	//inet_ntop(AF_INET, &ipv4num, ipv4test2, INET_ADDRSTRLEN);			// inet_ntoa ��� ��밡���� ANSI ǥ�� Ȯ�� �Լ�
																		// INET_ADDRSTRLEN�� 22, IPv4�� ���̰� 15���� �׳� �������.
	inet_ntop(AF_INET, &ipv4num.s_addr, ipv4test2, INET_ADDRSTRLEN);	// ���� ����� ���� �ǹ�
	//InetNtop(AF_INET, &ipv4num, (LPCWSTR)ipv4test2, INET_ADDRSTRLEN);	// inet_ntop�� ���� ����� �ϴ� Windows ���� �Լ�
																		// ���� ������ ��Ƽ����Ʈ�� ��������� �����ڵ�� �����ϴ� �� ��ü�� �����ڵ忩�� �Ѵ�.

	printf("IPv4 �ּ� �ٽ� ��ȯ �� = %s\n", ipv4test2);

	printf("\n");

	char * ipv6test1 = "2001:0230:abcd:ffab:0023:eb00:ffff:1111";
	printf("IPv6 �ּ� ��ȯ �� = %s\n", ipv6test1);

	IN6_ADDR ipv6num;								// IPv6 �ּҸ� �����ϴ� ����ü, IPv6�� ���� �ּ� ����ü�� �ƴϴ�.
	//inet_pton(AF_INET6, ipv6test1, &ipv6num);		// inet_pton�� IPv4 �Ӹ� �ƴ϶� IPv6�� �����Ѵ�.
	inet_pton(AF_INET6, ipv6test1, &ipv6num.u.Byte);// ���� ����� ���� �ǹ�

	printf("IPv6 �ּ� ��ȯ �� = 0x");
	for (int i = 0; i < 16; i++)				// ���̳ʸ� ���� �迭 ���·� ������ �Ǿ������Ƿ� 
		printf("%02x", ipv6num.u.Byte[i]);		// for������ ���
	printf("\n");

	char ipv6test2[46];
	//inet_ntop(AF_INET6, &ipv6num, ipv6test2, INET6_ADDRSTRLEN);		// ���̳ʸ� ���� IPv6�� �ٲ㼭 ���
																		// INET6_ADDRSTRLEN�� 65, IPv6�� ���̰� 45���� �׳� �������. 
	inet_ntop(AF_INET6, &ipv6num.u.Byte, ipv6test2, INET6_ADDRSTRLEN);	// ���� ����� ���� �ǹ�
																		
	printf("IPv6 �ּ� �ٽ� ��ȯ �� = %s\n", ipv6test2);

	WSACleanup();

	return 0;
}

// inet_pton, inet_ntop�� IPv4, IPv6 ���� Ŀ�� �����ϴ�.
// ANSIǥ������ Windows�� ���������� �ʰ� POSIX �迭 OS(������ ��)���� ���� ����� �����ϴ�.
// Windows �������δ� InetPton()�� InetNtop()�� �ִ�.
// ��Ƽ����Ʈ�� �����ڵ带 �����ؼ� ��� �Ѵ�.