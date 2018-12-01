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

	//LPCWSTR ipv4test1 = L"147.46.114.70";					// �����ڵ带 ����ϰ�ʹٸ� �̰�
	const char * ipv4test1 = "147.46.114.70";				// C++�� ���ڿ��� const�� ����ؾ� �Ѵ�.

	cout << "IPv4 �ּ� ��ȯ �� : " << ipv4test1 << endl;

	IN_ADDR ipv4num;										// IPv4 �ּҸ� �����ϴ� ����ü

	//inet_pton(AF_INET, ipv4test1, &ipv4num);				// inet_addr() ��� ��밡���� ANSI ǥ�� �Լ�
															// inet_addr()�� �ٸ� ���� inet_addr()�� �򿣵������ �ٲ� ���� �������� �˷��ֱ⸸ �Ѵ�.
															// ���� ������ ������ ��� �������.
															// inet_pton()�� �ּҸ� ������ IN_ADDR�� ������ �־�� �Ѵ�.
															// ������ �򿣵������ �ٲ� ���� �����ϴ� ����� �Ѵ�.
															// ipv4num�� ipv4test1�� AF_INET�� ���߾� �����Ѵٴ� �ǹ��̴�.
	inet_pton(AF_INET, ipv4test1, &ipv4num.s_addr);			// ���� ����� ���� �ǹ�
	//InetPton(AF_INET, (LPCWSTR)ipv4test1, &ipv4num);		// inet_pton()�� ���� ����� �ϴ� Windows ���� �Լ�
															// ���ڿ��� ��Ƽ����Ʈ�� �����ڵ带 �����ؼ� �����ؾ� �Ѵ�.
															// ���� ������ ��Ƽ����Ʈ�� ��������� �����ڵ�� �����ϴ� �� ��ü�� �����ڵ忩�� �Ѵ�.
	//cout << "IPv4 �ּ� ��ȯ �� : " << hex << "0x" << ipv4num << endl;		// IPv4 �ּҸ� ���̳ʸ� ������ �ٲ㼭 ���
																			// C++������ IN_ADDR���� ����� �� �����Ƿ� �����ε��ؾ� ����
	cout << "IPv4 �ּ� ��ȯ �� : " << hex << "0x" << ipv4num.s_addr << endl;	// ��Ȯ�ϰ� s_addr�� ����ϸ� �ȴ�.

	char ipv4test2[16];													// ���̳ʸ� ���� ��ȯ�Ͽ� ������ ������ �迭�� �޴´�.
	//inet_ntop(AF_INET, &ipv4num, ipv4test2, INET_ADDRSTRLEN);			// inet_ntoa() ��� ��밡���� ANSI ǥ�� �Լ�
																		// INET_ADDRSTRLEN�� 22, IPv4�� ���̰� 15���� �׳� �������.
																		// inet_ntoa()�� �ٸ� ���� inet_ntoa()�� �ý��� ���� ������� �ٲ� ���� �������� �˷��ֱ⸸ �Ѵ�.
																		// ���� ������ ������ ��� �������.
																		// inet_ntop()�� �ּҸ� ������ char�� �迭�� �ʿ��ϴ�.
																		// ������ �ý��� ���ķ� �ٲ� ���� �����ϴ� ����� �Ѵ�.
	inet_ntop(AF_INET, &ipv4num.s_addr, ipv4test2, INET_ADDRSTRLEN);	// ���� ����� ���� �ǹ�
	//InetNtop(AF_INET, &ipv4num, (LPCWSTR)ipv4test2, INET_ADDRSTRLEN);	// inet_ntop�� ���� ����� �ϴ� Windows ���� �Լ�
																		// ���� ������ ��Ƽ����Ʈ�� ��������� �����ڵ�� �����ϴ� �� ��ü�� �����ڵ忩�� �Ѵ�.

	cout << "IPv4 �ּ� �ٽ� ��ȯ �� : " << ipv4test2 << endl;

	cout << endl;

	const char * ipv6test1 = "2001:0230:abcd:ffab:0023:eb00:ffff:1111";
	cout << "IPv6 �ּ� ��ȯ �� : " << ipv6test1 << endl;

	IN6_ADDR ipv6num;								// IPv6 �ּҸ� �����ϴ� ����ü, IPv6�� ���� �ּ� ����ü�� �ƴϴ�.
	//SOCKADDR_IN6 ipv6sock;							// IPv6 ���� �ּ� ����ü
	//inet_pton(AF_INET6, ipv6test1, &ipv6num);		// inet_pton�� IPv4 �Ӹ� �ƴ϶� IPv6�� �����Ѵ�.
	inet_pton(AF_INET6, ipv6test1, &ipv6num.u.Byte);// ���� ����� ���� �ǹ�
	//inet_pton(AF_INET6, ipv6test1, &ipv6sock.sin6_addr.u.Byte);	// IPv6 ���� �ּ� ����ü�� ����Ѵٸ� �̰� ����ؾ� �Ѵ�.
																	// �߰��� sin6_addr�� �� �߰��Ǿ� �ִ�.

	cout << "IPv6 �ּ� ��ȯ �� : 0x";
	for (int i = 0; i < 8; i++)					// ���̳ʸ� ���� �迭 ���·� ������ �Ǿ������Ƿ�
	{
		//printf("%02x", ipv6num.u.Byte[i]);	// for������ ���
												// printf()�� �ּҰ��� �� �����µ� cout�� ����� ��µ��� �ʴ´�.
		cout << ipv6num.u.Word[i];				// �̰� ����ϸ� cout���� �� �������� �򿣵�� ������� ��µȴ�.
		//cout << ipv6num.sin6_addr.u.Word[i];	// IPv6 ���� �ּ� ����ü�� ����Ѵٸ� �̰� ����ؾ� �Ѵ�.
												// �߰��� sin6_addr�� �� �߰��Ǿ� �ִ�.
	}
	cout << endl;

	char ipv6test2[46];
	//inet_ntop(AF_INET6, &ipv6num, ipv6test2, INET6_ADDRSTRLEN);		// ���̳ʸ� ���� IPv6�� �ٲ㼭 ���
																		// INET6_ADDRSTRLEN�� 65, IPv6�� ���̰� 45���� �׳� �������. 
	inet_ntop(AF_INET6, &ipv6num.u.Byte, ipv6test2, INET6_ADDRSTRLEN);	// ���� ����� ���� �ǹ�
	//inet_ntop(AF_INET6, &ipv6num.sin6_addr.u.Byte, ipv6test2, INET6_ADDRSTRLEN);	// IPv6 ���� �ּ� ����ü�� ����Ѵٸ� �̰� ����ؾ� �Ѵ�.

	cout << "IPv6 �ּ� �ٽ� ��ȯ �� : " << ipv6test2 << endl;

	WSACleanup();

	return 0;
}

// inet_pton(), inet_ntop()�� IPv4, IPv6 ���� Ŀ�� �����ϴ�.
// ANSIǥ������ Windows�� ���������� �ʰ� POSIX �迭 OS(������ ��)���� ���� ����� �����ϴ�.
// Windows �������δ� InetPton()�� InetNtop()�� �ִ�.
// ��Ƽ����Ʈ�� �����ڵ带 �����ؼ� ��� �Ѵ�.