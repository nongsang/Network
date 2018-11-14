#pragma comment(lib, "ws2_32")
#include <WinSock2.h>

void err_quit(LPCWSTR msg)
{
	LPVOID lpMsgBuf;												// ���� �޼����� ������ ������

	FormatMessage(													// Error�� �߻��ϸ� �ڵ� ���·� ��µǴµ�, �ڵ带 ���ڿ��� �ٲ��ش�.
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,// �ý��ۿ� ��ϵǾ� �ִ� �޼����� ����ϸ�, �޼����� ���� �޸𸮴� �ڵ����� �Ҵ�
		NULL, WSAGetLastError(),									// WSA���� �߻��� ���� ������ ������ �����Ͽ� �����ڵ带 �߻��ϰ�,
		//MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),				// �߻��� �����ڵ�� ����� ��� �� ���� �ý��ۿ� ������ ���� �����Ͽ�,
		LANG_USER_DEFAULT,											// MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)�� ���� �ǹ̴�.
		(LPWSTR)&lpMsgBuf,											// lpMsgBuf�� �����Ѵ�. 
		0, NULL														// FORMAT_MESSAGE_ALLOCATE_BUFFER�� �޸𸮸� �ڵ� �Ҵ��߱� ������ ���� ũ��� 0�̴�.
	);

	MessageBox(NULL, (LPCWSTR)lpMsgBuf, msg, MB_ICONERROR);			// ���� �޼��� ���

	LocalFree(lpMsgBuf);											// FORMAT_MESSAGE_ALLOCATE_BUFFER�� ����Ͽ� �ڵ����� �޸𸮸� �Ҵ��ϸ�
																	// LocalFree�� �޸𸮸� ��������� �Ѵ�.
																	// ���� ũ�⸦ ���� �������ָ� ������� �ʾƵ� �ȴ�.

	exit(1);														// ���α׷� ����
}

int main()
{
	// ���� �ʱ�ȭ, ������ 01. Server����
	WSADATA wsa;

	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
		return 1;

	MessageBox(NULL, L"���� �ʱ�ȭ ����", L"�˸�", MB_OK);

	SOCKET tcp_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);// ���� ����
																// IPv4(AF_INET)�� ���
																// TCP(SOCK_STREAM) ���
																// ���������� IPPROTO_TCP
																// UDP�� ������ ���� IPPROTO_UDP�� ����ϸ� ������ IPPROTO_TCP�� ���� 0�̱� ������
																// �׳� �������� �κ��� 0���� �ص� �ȴ�.
																// ������ IPPROTO_TCP�� 6, IPPROTO_UDP�� 17, 0�� IPPROTO_HOPOPTS�ε� ��� ���ٴ°ɱ�?

	if (INVALID_SOCKET == tcp_sock) err_quit(L"socket");		// ���� ������ �����ϸ� ���� �޼���

	MessageBox(NULL, L"TCP ���� ���� ����", L"�˸�", MB_OK);

	closesocket(tcp_sock);										// ������ ������ �ݾ���� �Ѵ�.

	WSACleanup();												// ������ ���� �Ŀ� ������ �����ؾ� �Ѵ�.

	return 0;
}