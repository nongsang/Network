#pragma comment(lib, "ws2_32")
//#include <WinSock2.h>			// WS2tcpip.h�� ���ԵǾ� �����Ƿ� ���� �߰� �����൵ �ȴ�.
#include <WS2tcpip.h>
#include <stdio.h>

#define SERVERIP "127.0.0.1"// IPv4 ������ �ּ�, �׳� �ڱ� �ڽ��� ����Ű�� �ּ�
#define SERVERPORT 9000		// ���� ��Ʈ��ȣ, �׳� ���Ƿ� ���� ��
#define BUFSIZE 512			// ���ڿ��� ������ ������ ũ��
							// �ƴϸ� BUFSIZ��� �̹� �Ȱ��� ����� ������ ����ص� ����.

// �����ڵ� ���� ����

void err_quit(LPCWSTR msg)		// 002.InitWinsock.cpp�� ���������� ��� �ȳ��� �о��
{
	LPVOID lpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		LANG_USER_DEFAULT,
		(LPWSTR)&lpMsgBuf,
		0, NULL
	);

	MessageBox(NULL, (LPCWSTR)lpMsgBuf, msg, MB_ICONERROR);

	LocalFree(lpMsgBuf);

	exit(1);
}

void err_display(LPCWSTR msg)	// 007.DomainC.cpp�� ���������� ��� �ȳ��� �о��
{
	LPVOID lpMsgBuf;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		LANG_USER_DEFAULT,
		(LPWSTR)&lpMsgBuf
		, 0, NULL
	);

	MessageBox(NULL, (LPCWSTR)lpMsgBuf, msg, MB_ICONERROR);

	LocalFree(lpMsgBuf);
}

int recvn(SOCKET s, char * buf, int len, int flags)	// recv()�Լ��� len���� ���� �����͸� ���� �� �ִ�.
{													// �׷��� �����Ͱ� ũ�� �� �Ⱥ����� �� �����Ƿ� recvn()�Լ��� �ۼ��Ѵ�.
	int received;			// ���� ������ ũ��
	char * ptr = buf;		// ���� �����Ͱ� �����ϴ� ��ġ
	int left = len;			// ���� ������ ũ��

	while (left > 0)		// ���� �����Ͱ� ������
	{
		received = recv(s, ptr, left, flags);	// s���� ptr �����͸� left ���̸�ŭ �ް�, ���� ���̸� received�� �����Ѵ�.
		if (SOCKET_ERROR == received)			// ����� ���� ���ϸ�
			return SOCKET_ERROR;
		else if (0 == received)					// ���� �������� ũ�Ⱑ 0�̸�
			break;

		left -= received;						// ���� ������ ���̿��� ���� ������ ���̸� ����.
												// recv()�� �����Ͱ� Ŭ �� �ѹ��� ��� �����͸� �� ���� �� �����Ƿ�
												// �������� �����͸� �ް� �ٽ� ���� �� ��ŭ ���� �����Ͱ� ���Ҵ��� �˱� ���ؼ� ���
		ptr += received;						// �����͸� �ٽ� ���� �� �����ġ���� ���� ������ �� �˱� ���� ���
	}

	return (len - left);	// �����͸� �� ������ left�� 0�� �ǹǷ�, �ᱹ ���� �������� ���̸� ��ȯ�ȴ�.
}

int main()
{
	WSADATA wsa;

	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
		return 1;

	MessageBox(NULL, L"���� �ʱ�ȭ", L"�˸�", MB_OK);

	int retval;		// ���� �ڵ带 ������ ����

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);		// �ּ�ü��� IPv4, Ÿ���� SOCK_STREAM�� ���� ����
	if (INVALID_SOCKET == sock) err_quit(L"socket()");

	SOCKADDR_IN serveraddr;								// ������ ������ ������ ��� �ִ� ����ü
	memset(&serveraddr, 0, sizeof(serveraddr));			// 0���� �ʱ�ȭ
	serveraddr.sin_family = AF_INET;					// IPv4
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);	// ������ ������ �ּ�
	serveraddr.sin_port = htons(SERVERPORT);			// ������ ������ ��Ʈ��ȣ

	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));	// sock�� serveraddr ������ �����û
	if (SOCKET_ERROR == retval) err_quit(L"connect()");					// ���� �����ϸ� ����

	// ������ ��ÿ� ����� ����
	char buf[BUFSIZE + 1];
	int len;

	while (1)		// ������ �Ǹ� �������� �����͸� ������.
	{
		printf("[���� ������] : ");
		if (NULL == fgets(buf, BUFSIZE + 1, stdin))	// �Է��� �ߴµ� ����� �Է��� �ȉ�����
			break;									// Ż��

		len = strlen(buf);
		if ('\n' == buf[len - 1])	// ���ͷ� �Է��ϸ� �� �ڴ� '\n'�� �Էµ����� 
			buf[len - 1] = '\0';	//'\n'�� '\0'���� �ٲ㼭 ���ڿ��� ���� �ǹ��ϰԲ� �Ѵ�.
		if (0 == strlen(buf))		// �Է��� ���� ���� ���͸� �Է��ϸ�
			break;					// Ż��

		retval = send(sock, buf, strlen(buf), 0);	// sock�� ����� �ּҷ� buf�� �ִ� ���ڿ��� ������.
		if (SOCKET_ERROR == retval)					// �������� �ʾҴٸ�
		{
			err_display(L"send()");
			break;
		}

		printf("[TCP Ŭ���̾�Ʈ] : %d����Ʈ�� ���½��ϴ�.\n", retval);	// �������ٸ� ���� ������ ũ�⸦ ���

		retval = recvn(sock, buf, retval, 0);	// sock�� ����� ������ ���� �����Ͱ� ������ buf�� �����Ѵ�.
												// recv()�� �ƴ϶� ���� ������� recvn()���� �޴µ� ������ recvn()�� ������
		if (SOCKET_ERROR == retval)				// ����� ���� ���ϸ�
		{
			err_display(L"recv()");
			break;
		}
		else if (0 == retval)					// ���� ���� ������
			break;

		buf[retval] = '\0';
		printf("[TCP Ŭ���̾�Ʈ] : %d����Ʈ�� �޾ҽ��ϴ�.\n", retval);
		printf("[���� ������] : %s\n", buf);

	}

	closesocket(sock);

	WSACleanup();

	return 0;
}

// ����� �Լ�
// socket()  : ���� ����
//			   ������ ������ ���ŷ �����̴�.
// connect() : ȣ��Ʈ�� ���� ��û�� �Ѵ�.
// send()	 : �����͸� �ü���� �۽� ���ۿ� �����Ѵ�.
//			   �۽� ���ۿ� �ִ� �����ʹ� ����� ȣ��Ʈ�� �ü���� ���� ���۷� �����Ѵ�.
//			   �۽� ���ۿ� �����Ͱ� �ִٰ� �ٷ� ������ ���� �ƴ϶� ���� �ð��� ���� �� �Ϻ� ���������� ���� ������ �ȴ�.
//			   �����ϴ� ������ Ư���� ���� �ٸ� ���۹���� ����Ѵ�.
//			   socket()���� ������ ������ �����ϸ� ���ŷ ������� �����͸� �����Ѵ�.
//			   �۽� ������ ���������� send()�� ����° ���� len���� ������ ����Ѵ�.
//			   ������ ����� �׶����� �۽� ���ۿ� �����͸� �����Ѵ�.
//			   �ͺ��ŷ �����̶�� �۽� ������ ������ �����̶� ����� �ٷιٷ� �����͸� �۽� ���ۿ� �����ϰ� �ȴ�.
//			   �ͺ��ŷ ������ ���߿� �˾ƺ���.
// recv()	 : �ü���� ���� ���ۿ� ������ �����͸� ���� ���α׷� ���ۿ� �����Ѵ�.
//			   ��ȯ���� ���޹��� �������� �����̴�.
//			   ��ȯ���� 0�̸� ��밡 ���� �����ߴٴ� �ǹ��̴�.
// recvn()	 : TCP�� ������ ��踦 �������� �ʴ´�.
//			   recv()�� �����͸� ���� �� �׸����ϰ� �ؽ�Ʈ������ ���ÿ� ������ TCP�� �׸����Ͽ��� ���� �����Ϳ� �ؽ�Ʈ���� ���� �����͸� �������� �ʴ´�.
//			   �ϳ��� �ϳ��� ������ ����� ���� ���ۿ��� ���� ���α׷� ���ۿ� �����ϴµ�, ������ ���̸� �˷��ش� �ѵ� ������ �� ũ�� ���� ���ڴ�� �����Ѵ�.
//			   ���� ���� ���� �����͸� ó���� �� ���� �����͸� ó���ϴ� ������� �ٲٰ�, �˷��� ������ ũ�⸸ŭ ó���ϴ� ������� �ٲ۴ٸ� ����� ������ �� ���̴�.
//			   ��� recv()�� �׹�° ���� flags�� MSG_WAITALL�� �����ϸ� recvn()�� ����.
//			   ������ Windows XP ���Ͽ����� ����� �� ����.
//			   ����� Windows 7�̻��� �Ϲ����̹Ƿ� ����ص� �ȴ�.

// ������ �޸� ������� ���� �Լ�
// bind()	: ���Ͽ� IP �ּ�, ��Ʈ��ȣ�� �������ش�.
//			  bind()�� ����ؼ� IP �ּҿ� ��Ʈ��ȣ�� ���� �������� ���� �ִ�.
//			  connect()�� ȣ���ϸ� serveraddr�� ����� IP �ּҷ� ������ ��û�ϰ�,
//			  ������ �Ǹ� �ü���� �ڵ����� IP �ּҿ� ��Ʈ ��ȣ�� �Ҵ����ش�.
//			  ��Ʈ��ȣ�� Windows ������ ���� �ٸ���.
// listen() : ������ TCP ��Ʈ ���¸� LISTENING���� �ٲ۴�.
//			  ������ �ƴϱ� ������ �ٸ� Ŭ���̾�Ʈ�� ������ ����ϰ� �־���� ������ ����.
//			  Ŭ���̾�Ʈ���� ������ ���� �� �ִ� ����� �����Ѵٸ� ����� �� �ִ�.
// accept() : ������ Ŭ���̾�Ʈ�� ����� �� �ֵ��� ���ο� ������ �����ؼ� �����Ѵ�.
//			  �� ���� ������ �ƴϱ� ������ �ٸ� Ŭ���̾�Ʈ�� ������ ����ϰ� ���� ����.
//			  