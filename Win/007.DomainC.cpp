#pragma comment(lib, "ws2_32")
#include <WS2tcpip.h>
#include <stdio.h>

// �����ڵ� ���� ����

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

// IPv4 �ּ� -> ������ �̸�
BOOL GetIPAddr(const char * name, IN_ADDR * addr)
{
	HOSTENT * ptr = gethostbyname(name);

	if (NULL == ptr)							// ȣ��Ʈ�� ã�� ���ϸ�
	{
		err_display(L"gethostbyname");
		return FALSE;
	}
	if (AF_INET != ptr->h_addrtype)				// ã�� ȣ��Ʈ�� IPv4�� �ƴ϶��
		return FALSE;

	memcpy(addr, ptr->h_addr, ptr->h_length);	// ������ �� ������ ����� memcpy�� �ؾ� �Ѵ�.
	return TRUE;
}

// ������ �̸� -> IPv4
BOOL GetDomainName(IN_ADDR addr, char * name, int namelen)
{
	HOSTENT * ptr = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);	// addr�� ����� IP �ּҷ� ȣ��Ʈ�� ã�� �����Ѵ�.

	if (NULL == ptr)						// �������� ������
	{
		err_display(L"gethostbyaddr");
		return FALSE;
	}
	if (AF_INET != ptr->h_addrtype)			// IPv4�� �ƴ϶��
		return FALSE;

	strncpy(name, ptr->h_name, namelen);	// ���ڿ� ������ ����� strcpy�� �ؾ� �Ѵ�.
	return TRUE;
}

int main()
{
	WSADATA wsa;

	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
		return 1;

	MessageBox(NULL, L"���� �ʱ�ȭ", L"�˸�", MB_OK);

	//const char * TestName = "www.naver.com";				// ������ �������� ���ϴ� ����� ��� �����.
	const char * TestName = "cvsmtppost001.nm.naver.com";	// IP -> ������ �̸� ��ȯ�� �� ����ε� ��ȯ�� ���� ������ ���
															// ���̹� ������ ��

	printf("������ �̸� ��ȯ �� : %s\n", TestName);

	printf("\n");

	// ������ -> IP �ּ� ��ȯ ��� 0��°
	printf("������ -> IP �ּ� ��ȯ ��� ù��°\n");
	HOSTENT * host1 = gethostbyname(TestName);				// ȣ��Ʈ�� ������ �޾Ƽ� HOSTENT�� ������ �����Ͱ� ����Ű�� �Ѵ�.
	printf("inet_ntoa(*(IN_ADDR*)domain1->h_addr : %s\n", inet_ntoa(*(IN_ADDR*)host1->h_addr));	// ȣ��Ʈ ������ IP �ּҸ�
																								// IN_ADDR�� ������ ��ȯ�ϰ� �ٽ� ���ڿ��� ��ȯ�Ͽ� ���

	printf("\n");

	// ������ -> IP �ּ� ��ȯ ��� 1��°
	printf("������ -> IP �ּ� ��ȯ ��� �ι�°\n");
	IN_ADDR addr1;										// ���������� ������ IP �ּҸ� ������ ����

	//IP.s_addr = *domain1->h_addr_list[0];				// �� ����� ���� �պκ� IP���� �����Ͽ� ��ȯ���ش�.
														// �ֳ��ϸ� h_addr_list�� �� IP�ּ� ������ ���ҷ� �Ͽ� �迭�� �̷�� �ִ�.
														// �׷��� ������ �� ���Ҹ� s_addr�� �� ���ҿ� for������ ������ �־���� �Ѵ�.
	memcpy(&addr1, host1->h_addr, host1->h_length);		// memcpy�� �ϸ� ��� IP���� �����Ѵ�.
	printf("memcpy(&IP, domain1->h_addr, domain1->h_length) : %s\n", inet_ntoa(addr1));	// ������ ȣ��Ʈ�� IP �ּ�

	printf("\n");

	printf("%s�� ���� ��Ī : %s\n", TestName, host1->h_name);	// TestName�� ���ǵ� ȣ��Ʈ�� ���� ��Ī
																// ���ĸ�Ī���� �����ص� ���ϴ� �������� �ƴ� �� �ִ�.

	printf("\n");

	printf("%s�� �����\n", TestName);
	for (int i = 0; host1->h_aliases[i]; i++)					// �������� �� ������ ���� ������
		printf("%d��° : %s\n", i, host1->h_aliases[i]);			// ���� ���

	printf("\n");

	// ������ -> IP �ּ� ��ȯ ��� 2��°
	printf("������ -> IP �ּ� ��ȯ ��� ����°\n");

	IN_ADDR addr2;										// ���������� ������ IP �ּҸ� ������ ����
	if (GetIPAddr(TestName, &addr2))					// �ι�° ����� �Լ��� ������� ��
		printf("GetIPAddr(TestName, &addr) : %s\n", inet_ntoa(addr2));

	printf("\n");

	printf("IP -> ������ �ּ� ��ȯ ��� ù��°\n");
	HOSTENT * host2 = gethostbyaddr((char*)&addr2, sizeof(host2), AF_INET);	// addr2�� ����� IP �ּ��� ������ ����
																			// addr2�� IN_ADDR�̹Ƿ� char*�� ������ �����ؾ� �Ѵ�.

	printf("host2->h_name : %s\n", host2->h_name);	// Reverse DNS Lookup(������ ������)�� �̿��Ͽ� ������ �̸��� �˾Ƴ���.
													// ������ DNS ������ ���� ����� �Ǿ� �־�� ������ �̸��� ã�� �� �ִ�.
													// ������ �������� ������ ����� ���� �ʴ� �߼��� ���� ����ε� ���� ������ �ʴ´�.
													// ���̹� ���� ������ �������� �׽�Ʈ�غ��� NULL�̰ų� ����ε� ���� �ƴ� �� �ִ�.
													// ���� �׽�Ʈ�� �� �� �ִ� ������ �̸��� ���� �߰��� ������.

	printf("\n");

	printf("IP -> ������ �ּ� ��ȯ ��� �ι�°\n");
	char name[256];										// IP �ּҷ� ã�� ȣ��Ʈ�� �̸��� ������ �迭
	if (GetDomainName(addr2, name, sizeof(name)))		// ù��° ����� �Լ��� ������� ��
		printf("GetDomainName(addr2, name, sizeof(name) : %s\n", name);

	WSACleanup();

	return 0;
}

// gethostbyname(), gethostbyaddr()�� ANSIǥ������ POSIX �迭 OS���� ����� �����ϴ�.
// gethostbyname()�� ȣ��Ʈ�� IPv4�� IPv6 ��� ������ �ִ� ��� ������ IPv4 �ּҸ� �����Ѵ�.
// �����ڵ带 ����Ѵٸ� GetHostByNameW(), GetHostByAddrW()�� ����ϸ� ������ Windows �����̴�.
// ������ IPv4�� ����ϴ� gethostbyname()�� ���� ������ ������ IPv6�� ����� ���� �ٸ� ����� �ʿ��ϴ�.