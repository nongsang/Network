#pragma comment(lib, "ws2_32")
//#include <WinSock2.h>			// WS2tcpip.h�� ���ԵǾ� �����Ƿ� ���� �߰� �����൵ �ȴ�.
#include <WS2tcpip.h>
#include <iostream>

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

int main()
{
	WSADATA wsa;

	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
		return 1;

	MessageBox(NULL, L"���� �ʱ�ȭ", L"�˸�", MB_OK);

	int retval;

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == listen_sock) err_quit(L"socket()");

	SOCKADDR_IN serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));

	serveraddr.sin_family = AF_INET;							// serveraddr�� �ּ�ü��� AF_INET
	inet_pton(AF_INET, INADDR_ANY, &serveraddr.sin_addr.s_addr);// serveraddr.sin_addr.s_addr�� INADDR_ANY�� AF_INET�� �°� �����Ͽ� ����
																// ������ serveraddr.sin_addr.s_addr�� htonl(INADDR_ANY)�� ����������
																// inet_pton()�� �̿��ϸ� thread safe�ϹǷ� ����ϸ� ����.
	serveraddr.sin_port = htons(SERVERPORT);					// serveraddr�� ��Ʈ��ȣ ����

	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (SOCKET_ERROR == retval) err_quit(L"bind()");

	retval = listen(listen_sock, SOMAXCONN);

	if (SOCKET_ERROR == retval) err_quit(L"listen()");

	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	
	char buf[BUFSIZE];

	while (1)
	{
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);

		if (INVALID_SOCKET == client_sock)
		{
			err_display(L"accept()");
			break;
		}

		//printf("[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ� = %s, ��Ʈ ��ȣ = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		cout << "[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ� = " << inet_ntoa(clientaddr.sin_addr) << ", ��Ʈ ��ȣ = " << ntohs(clientaddr.sin_port) << endl;

		while (1)
		{
			retval = recv(client_sock, buf, BUFSIZE, 0);
			if (INVALID_SOCKET == retval)
			{
				err_display(L"recv()");
				break;
			}
			else if (0 == retval)
				break;

			buf[retval] = '\0';

			//printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);
			cout << "[TCP/" << inet_ntoa(clientaddr.sin_addr) << ":" << ntohs(clientaddr.sin_port) << "] " << buf << endl;

			retval = send(client_sock, buf, retval, 0);

			if (SOCKET_ERROR == retval)
			{
				err_display(L"send()");
				break;
			}
		}

		closesocket(client_sock);
		//printf("[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ� = %s, ��Ʈ ��ȣ = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		cout << "[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ� = " << inet_ntoa(clientaddr.sin_addr) << ", ��Ʈ ��ȣ = " << ntohs(clientaddr.sin_port) << endl;
	}

	closesocket(listen_sock);

	WSACleanup();

	return 0;
}

// ����� �Լ�
// socket() : ���� ����
//			  ������ ������ ���ŷ �����̴�.
// bind()	: ���Ͽ� IP �ּ�, ��Ʈ��ȣ�� �������ش�.
//			  SOCKET�� SOCKADDR_IN ���ڰ� �ʿ��ϴ�.
//			  ���� �����̹Ƿ� SOCKADDR_IN�� ���� IP�ּҴ� INADDR_ANY�� ���� ���� �ٶ����ϴ�.
// listen() : ������ TCP ��Ʈ ���¸� LISTENING���� �ٲ۴�.
//			  �̴� Ŭ���̾�Ʈ ������ ���� �� �ִ� �����·� �ٲ��� ���Ѵ�.
//			  ���� SOMAXCONN�� Ŭ���̾�Ʈ�� ������ �� �ִ� ������ �ִ�� �Ѵٴ� �ǹ̴�.
// accept() : ������ Ŭ���̾�Ʈ�� ����� �� �ֵ��� ���ο� ������ �����ؼ� �����Ѵ�.
//			  ������ ������ ������ ��û�� ���ϰ� ���� �� �����͸� ��ȯ�ϰ� �ȴ�.
//			  ������ �־��� listen()���� �����ߴ� �������� �����ϴ� ���� �ƴ϶� ���ο� ������ ���� Ŭ���̾�Ʈ�� �����ϴ� ���̴�.
//			  ������ �Ǵ��� �������� ����ؼ� ���Ƽ� LISTENING ���¸� ����ؼ� �����ִ� �����̴�.
//			  ���� ������ ��û�� Ŭ���̾�Ʈ�� �ּҳ� ���̰� �ʿ������ addr�� addrlen�� NULL�� �����ϸ� �ȴ�.
// send()	: �����͸� �ü���� �۽� ���ۿ� �����Ѵ�.
//			  �۽� ���ۿ� �ִ� �����ʹ� ����� ȣ��Ʈ�� �ü���� ���� ���۷� �����Ѵ�.
//			  �۽� ���ۿ� �����Ͱ� �ִٰ� �ٷ� ������ ���� �ƴ϶� ���� �ð��� ���� �� �Ϻ� ���������� ���� ������ �ȴ�.
//			  �����ϴ� ������ Ư���� ���� �ٸ� ���۹���� ����Ѵ�.
//			  socket()���� ������ ������ �����ϸ� ���ŷ ������� �����͸� �����Ѵ�.
//			  �۽� ������ ���������� send()�� ����° ���� len���� ������ ����Ѵ�.
//			  ������ ����� �׶����� �۽� ���ۿ� �����͸� �����Ѵ�.
//			  �ͺ��ŷ �����̶�� �۽� ������ ������ �����̶� ����� �ٷιٷ� �����͸� �۽� ���ۿ� �����ϰ� �ȴ�.
//			  �ͺ��ŷ ������ ���߿� �˾ƺ���.
// recv()	: �ü���� ���� ���ۿ� ������ �����͸� ���� ���α׷� ���ۿ� �����Ѵ�.
//			  ��ȯ���� ���޹��� �������� �����̴�.
//			  ��ȯ���� 0�̸� ��밡 ���� �����ߴٴ� �ǹ��̴�.

// Ŭ���̾�Ʈ�� �޸� ������� ���� �Լ�
// connect() : ȣ��Ʈ�� ���� ��û�� �Ѵ�.
//			   ������ �㰡���ִ� ���̹Ƿ� ������� �ʴ� �Լ��̴�.
// recvn()	 : �����ϰ� recv()�� �׹�° ���� flags�� MSG_WAITALL�� �����ϸ� ����� �� �ִ�.