#pragma comment(lib, "ws2_32")
#include <WS2tcpip.h>
#include <iostream>

// �����ڵ� ���� ����

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

	MessageBox(NULL, (LPCWSTR)lpMsgBuf, msg, MB_ICONERROR);		// �޼��� �ڽ��� ��������� ���
	//cout << "[" << msg << "] " << (LPCWSTR)lpMsgBuf << endl;	// �ֿܼ� �����ڵ尡 ��µȴ�.
																// �� �״�� �����ڵ尡 ��µǴ� ������ �޼��� �ڽ��� �������.

	LocalFree(lpMsgBuf);
}

int main()
{
	WSADATA wsa;		// �ʱ�ȭ ����

	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
		return 1;

	MessageBox(nullptr, L"���� �ʱ�ȭ", L"�˸�", MB_OK);

	const char * TestName = "www.naver.com";
	//const char * TestName = "cvsmtppost001.nm.naver.com";		// IP -> ������ �̸� ��ȯ�� �� ����ε� ��ȯ�� ���� ������ ���

	cout << "������ �̸� ��ȯ �� : " << TestName << endl;

	ADDRINFO hint;						// ȣ��Ʈ�� ������ �����ϴ� ����ü
	ZeroMemory(&hint, sizeof(hint));	// ��� ������ 0���� �ʱ�ȭ�ؾ� �Ѵ�.
										// �� �ؾ��ϴ� �۾��̴�.
										// ���ϸ� ����� ����� �� �� ����.
	//hint.ai_flags = AI_CANONNAME;		// ai_flags�� �ɼ��̸�, ai_canonname �ʵ忡 ȣ��Ʈ�� ���ĸ�Ī�� ����
	hint.ai_flags = AI_FQDN;			// ai_canonname �ʵ忡 ȣ��Ʈ�� ������ ����
	hint.ai_family = AF_UNSPEC;			// ������ ȣ��Ʈ�� TCP���� UDP���� ������� �ʰڴ�.
	hint.ai_socktype = SOCK_STREAM;		// Ÿ���� TCP
	hint.ai_protocol = IPPROTO_TCP;		// ���������� TCP���� 0���� �ֵ� �������.

	ADDRINFO * result = nullptr;		// HOSTENTó�� ȣ��Ʈ�� ����ų ����

	getaddrinfo(TestName, "80", &hint, &result);	// gethostbyname() ��ſ� ����� �� �ִ� ANSI ǥ�� �Լ�
													// ȣ��Ʈ�� ��Ʈ��ȣ(80���� HTTP)�� hint�� �ִ� �������ݷ� result�� ����Ű�� �Ѵ�.
													// 0�̸� ����, �� �̿��� ���� �����ڵ�
													// gethostbyname_r()�̶�� �Լ��� �ִµ� ����������� ����� ����� �� ����.

	cout << endl;

	//IN_ADDR * ipv4 = (IN_ADDR*)result->ai_addr;	// ȣ��Ʈ�� IP �ּҴ� �Ź� �ٲ�� ��찡 ���� ������
													// IP �ּ� ����ü�� �����ͷ� ���� ����Ű�� ���� ���� �׳� ����
	//cout << inet_ntoa(*ipv4) << endl;				// IN_ADDR * �� ����Ű�� �ؼ� IP �ּҸ� ����ϸ� �̻��� ���� ���´�.
													// memcpy()�� ����ص� ��������

	SOCKADDR_IN * ipv4sock = (SOCKADDR_IN*)result->ai_addr;						// IP �ּҸ� IPv4 ���� ����ü�� �����ͷ� ����Ű�� �� �� �ִ�.
																				// �����͸� ������ �ְ� �޴� ����ü�ε� �����ϰ���.
	cout << "������->IP �ּ� ��ȯ : " << inet_ntoa(ipv4sock->sin_addr) << endl;	// SOCKADDR_IN���� ȣ��Ʈ�� ����Ű�� �� �Ŀ�
																				// ȣ��Ʈ�� IP �ּҸ� ����� �� �ִ�.

	cout << endl;

	cout << TestName << "�� ��Ī : " << result->ai_canonname << endl;	// hint.ai_flags�� ���� ���� ��Ī, Ȥ�� ������ ��ȯ�ȴ�.

	char hostname[NI_MAXHOST];		// ȣ��Ʈ�� ������ �̸��� ������ ����
									// �迭�� ���̴� ȣ��Ʈ �̸��� �ִ��� ������ �� ���� �����̴�.
	char servInfo[NI_MAXSERV];		// ���� �̸��� ��Ʈ��ȣ�� ������ ����, HTTP?
									// �迭�� ���̴� ���� �̸��� �ִ��� ������ �� ���� �����̴�.

	cout << endl;

	// gethostbyaddr() ��ſ� ����� �� �ִ� ANSI ǥ�� �Լ�
	// ipv4sock�� ����Ű�� ȣ��Ʈ�� �ּ� ���θ�
	// hostname�� ������ �̸����� ��ȯ�Ͽ� �����ϰ�,
	// servInfo�� ���� �̸�(HTTP�ΰ� FTP�ΰ�)�� �����ϴµ�
	// ��Ʈ ��ȣ�� �����Ѵ�.
	getnameinfo((SOCKADDR*)ipv4sock, sizeof(SOCKADDR), hostname, NI_MAXHOST, servInfo, NI_MAXSERV, NI_NUMERICSERV);

	cout << "IP->������ �ּ� ��ȯ : " << hostname << endl;	// DNS ������ ����� �Ǿ����� ������ ����ε� �ּҰ� ������ �ʴ´�.
															// ���ϼ����� ����� ����
	
	GetAddrInfoW()

	WSACleanup();

	return 0;
}

// getaddrinfo(), getnameinfo()�� ANSIǥ������ POSIX �迭 OS���� ����� �����ϴ�.
// ADDRINFO�� ����Ű�� ȣ��Ʈ �ּҿ� SOCKADDR_IN���� ����Ű����, SOCKADDR_IN6���� ����Ű���Ŀ� ���� IPv4, IPv6�� ����Ѵ�.
// �����ڵ带 ����Ѵٸ� GetAddrInfoW(), GetNameInfoW()�� ����ϸ� ������ Windows ���� �Լ���.
// getaddrinfoex(), getnameinfoex()�� ������ �̰� ADDRINFOEX��� ȣ��Ʈ�� �߰� ������ ��� ����ü�� ����ؾ� �Ѵ�.
// �� �ۿ� ADDRINFOEX2, ADDRINFOEX3, ADDRINFOEX4 ����ü�� ������ ��� ADDRINFO�� �߰� ������ �����ϴ� �����̴�.
// ADDRINFOEX�� ����ϸ� GetAddrInfoEx(), GetNameInfoEx()�� ����ؾ� �Ѵ�.
// getaddrinfo(), getnameinfo()�� thread safe�ϹǷ� gethostbyname(), gethostbyaddr()�� ��ü�Ͽ� ����Ѵ�.
// IPv4�� ����Ѵٸ� gethostbyname_r()�� gethostbyaddr_r()�� thread safe�ϹǷ� �ٲ㼭 ����ϸ� ������ Windows������ ����� �� ����.