#pragma comment(lib, "ws2_32")
//#include <WinSock2.h>			// WS2tcpip.h�� ���ԵǾ� �����Ƿ� ���� �߰� �����൵ �ȴ�.
#include <WS2tcpip.h>
#include <iostream>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 512

// �����ڵ� ���� ����

using namespace std;

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

int recvn(SOCKET s, char * buf, int len, int flags)
{
	int received;
	char * ptr = buf;
	int left = len;

	while (left > 0)
	{
		received = recv(s, ptr, left, flags);
		if (SOCKET_ERROR == received)
			return SOCKET_ERROR;
		else if (0 == received)
			break;

		left -= received;

		ptr += received;
	}

	return (len - left);
}

int main()
{
	WSADATA wsa;

	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
		return 1;

	MessageBox(NULL, L"���� �ʱ�ȭ", L"�˸�", MB_OK);

	int retval;

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == sock) err_quit(L"socket()");

	SOCKADDR_IN serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;							// serveraddr�� �ּ�ü��� AF_INET
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr.s_addr);	// serveraddr.sin_addr.s_addr�� SERVERIP�� AF_INET�� �°� �����Ͽ� ����
																// ������ serveraddr.sin_addr.s_addr�� inet_addr(SERVERIP)�� ����������
																// inet_pton()�� �̿��ϸ� thread safe�ϹǷ� ����ϸ� ����.
	serveraddr.sin_port = htons(SERVERPORT);					// serveraddr�� ��Ʈ��ȣ ����


	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (SOCKET_ERROR == retval) err_quit(L"connect()");

	// ������ ��ſ� ����� ����
	char buf[BUFSIZE + 1];
	int len;

	while (1)
	{
		cout << "[���� ������] : ";
		if (NULL == fgets(buf, BUFSIZE + 1, stdin))
			break;

		len = strlen(buf);
		if ('\n' == buf[len - 1])
			buf[len - 1] = '\0';
		if (0 == strlen(buf))
			break;

		retval = send(sock, buf, strlen(buf), 0);
		if (SOCKET_ERROR == retval)
		{
			err_display(L"send()");
			break;
		}

		cout << "[TCP Ŭ���̾�Ʈ] : " << retval << "����Ʈ�� ���½��ϴ�." << endl;

		//retval = recvn(sock, buf, retval, 0);			// recvn()�� ����ϴ� ��ſ�
		retval = recv(sock, buf, retval, MSG_WAITALL);	// recv()�� flags���� MSG_WAITALL�� �ص� recvn()�� ���� �ǹ̴�.

		if (SOCKET_ERROR == retval)
		{
			err_display(L"recv()");
			break;
		}
		else if (0 == retval)
			break;

		buf[retval] = '\0';
		cout << "[TCP Ŭ���̾�Ʈ] : " << retval << "����Ʈ�� �޾ҽ��ϴ�." << endl;
		cout << "[���� ������] : " << buf << endl;
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
//			  ������ �׳� connect()�� ȣ���ϸ�, �ü���� �ڵ����� IP