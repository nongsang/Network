#pragma comment(lib, "ws2_32")
//#include <WinSock2.h>			// WS2tcpip.h�� ���ԵǾ� �����Ƿ� ���� �߰� �����൵ �ȴ�.
#include <WS2tcpip.h>
#include <stdio.h>

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

int main()
{
	WSADATA wsa;

	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
		return 1;

	MessageBox(NULL, L"���� �ʱ�ȭ", L"�˸�", MB_OK);

	int retval;		// ���� �ڵ带 ������ ����

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);		// �ּ�ü��� IPv4, Ÿ���� SOCK_STREAM�� ���� ����
	if (INVALID_SOCKET == listen_sock) err_quit(L"socket()");	// ���� ������ �ȵǸ� ���� �޼����� �Բ� ����

	SOCKADDR_IN serveraddr;						// IPv4 ���� �ּ� ����ü
	memset(&serveraddr, 0, sizeof(serveraddr));	// �� ZeroMemory()�� �ƴ϶� memset������ 008.DomainCPP.cpp�� ���� �ȴ�.

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);	// ������ ��� Ŭ���̾�Ʈ�� IPv4, IPv6 � �ּҷε� ������ �� �ֵ���
													// INADDR_ANY�� �����ִ� ���� �ٶ����ϴ�.
													// �����̹Ƿ� ������ �ּҸ� �����ϴ°� �ƴ϶� �ƹ� �����̳� �޴´ٴ� �ǹ�
	serveraddr.sin_port = htons(SERVERPORT);		// �������� ����� ������ ��Ʈ��ȣ 

	//retval = bine(listen_sock, (SOCKADDR*)&serveraddr, sizeof(SOCKADDR_IN));	// ���� ��ó�� �ڷ����� ũ�⸦ �������ִ� ������ �ڵ��� �ϸ�
																				// SOCKADDR_IN�� �ƴ϶� SOCKADDR_INDA�� �ٲ���� ��
																				// sizeof(SOCKADDR_INDA)�� �ٲٴ� ���ŷο��� �ִ�.
																				// �׳� sizeof(������)���� ����.
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));	// ������ ���ϰ� �ּ�, ��Ʈ��ȣ�� ���� ���� ����ü�� ���´�.
	if (SOCKET_ERROR == retval) err_quit(L"bind()");						// �����ϸ� ����, �����ϸ� 0 ��ȯ

	retval = listen(listen_sock, SOMAXCONN);			// ������ TCP ��Ʈ ���¸� LISTENING���� �ٲ۴�.
														// SOMAXCONN�� ���� ������ Ŭ���̾�Ʈ�� �ִ� ������ ��Ÿ����.
	if (SOCKET_ERROR == retval) err_quit(L"listen()");	// �����ϸ� ����, �����ϸ� 0 ��ȯ

	SOCKET client_sock;			// ������ Ŭ���̾�Ʈ�� ����
	SOCKADDR_IN clientaddr;		// ������ Ŭ���̾�Ʈ�� ����
	int addrlen;				// SOCKADDR_IN�� ���̸� ���� ����
								// ������ �� �ּҰ����� ������ �Ŵϱ� ������ �ȵȴ�.
	char buf[BUFSIZE];

	while (1)
	{
		// ������ Ŭ���̾�Ʈ�� �ִ��� �������� Ȯ��
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);	// LISTENING���� �Ǿ��ִ� ����(listen_sock)��
																				// ������ ��û�� client_sock�� ���� �㰡�� ������.
																				// ������ ��û�� Ŭ���̾�Ʈ�� IP ������ clientaddr�� ����ȴ�.
		if (INVALID_SOCKET == client_sock)	// ���� �㰡�� �����ϸ�
		{
			err_display(L"accept()");
			break;
		}

		printf("[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ� = %s, ��Ʈ ��ȣ = %d\n",	// Ŭ���̾�Ʈ�� ���ӿ� �����ϸ� ������ Ŭ���̾�Ʈ�� IP �ּҿ� ��Ʈ ��ȣ�� ���
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// ������ʹ� ������ ������ Ŭ���̾�Ʈ���� ���� ������ ��ȯ
		while (1)
		{
			retval = recv(client_sock, buf, BUFSIZE, 0);
			if (INVALID_SOCKET == retval)		// �����͸� �޴µ� ������ ��
			{
				err_display(L"recv()");
				break;
			}
			else if (0 == retval)				// ������ �������� ��
				break;

			buf[retval] = '\0';					// retval�� recv()�� ���������� ������, ���� �������� ���� �����Ѵ�.
												// buf[retval]�̶� ���� ���� �������� ���� �ǹ��Ѵ�.
												// ���� �����Ͱ� 8����Ʈ�� retval�� 8�̸� buf[8] = '\0'�� �ǰ�,
												// buf[0] ~ buf[7]���� �� ���ڰ� ����Ǿ� �ִ�.
			printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);	// ���� ������ ���

			retval = send(client_sock, buf, retval, 0);		// Ŭ���̾�Ʈ �������� buf�� ������ retval ���� ��ŭ ������.
															// ��� Ŭ���̾�Ʈ�� Ŭ���̾�Ʈ�� ���´� ������ �ٽ� ������ ���
			if (SOCKET_ERROR == retval)						// �����͸� �����µ� ������ ��
			{
				err_display(L"send()");
				break;
			}
		}

		closesocket(client_sock);				// Ŭ���̾�Ʈ�� �����ϸ� Ŭ���̾�Ʈ�� ���� ������ �ݾ���� �Ѵ�.
		printf("[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ� = %s, ��Ʈ ��ȣ = %d\n",	// Ŭ���̾�Ʈ�� �����ϸ� ������ Ŭ���̾�Ʈ�� IP �ּҿ� ��Ʈ ��ȣ�� ���
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}

	closesocket(listen_sock);		// ���α׷��� �����ϱ� ���� ���� �㰡�� �� ������ �ݾ���� �Ѵ�.

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