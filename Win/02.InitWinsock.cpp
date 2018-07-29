#pragma comment(lib, "ws2_32")
#include <WinSock2.h>

void err_quit(char *msg)
{
	LPVOID lpMsgBuf;	// ��� Ÿ���� ����ų �� �ִ� ������
	FormatMessage(		// ������ �ڵ�� ��Ÿ���� ���� �𸣴ϱ� �޼��� ���·� �ٲ㼭 â�� ����ִ� �Լ�
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,	// �ü���� ���� ���� �޼����� �޾Ƽ� ������ ������ �˾Ƽ� �Ҵ��Ѵ�.
		NULL, WSAGetLastError(),	// ���ӿ� ���� ������ �����ϸ� ȣ��,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	// ����� ���(LANG_NEUTRAL)��, ���߿��� �����ǿ� �����Ǿ��ִ� ���(SUBLANG_DEFAULT)�� �������� ��Ÿ����
		(LPTSTR)&lpMsgBuf, 0, NULL	// ���ڿ��� �����ͷ� ����Ű�� �Ѵ�.
	);
	MessageBox(NULL, (LPCSTR)lpMsgBuf, msg, MB_ICONERROR);	// �����ͷ�, �Է¹��� ���ڿ��� ����ϰ� �Ѵ�.
	LocalFree(lpMsgBuf);		// ���� �޸𸮸� �����Ѵ�.
	exit(1);
}

int main()
{
	// ���� �ʱ�ȭ ������ 01. Server����
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	MessageBox(NULL, "���� �ʱ�ȭ ����", "�˸�", MB_OK);

	SOCKET tcp_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);		// �ּ� ü��� IPv4, ���� Ÿ���� TCP, ����� ���������� IPPROTO_TCP�� ����ص� �ǰ�, 0���� ���� TCP�� UDP�� ���������� ������ �� �ִ�.
	if (tcp_sock == INVALID_SOCKET) err_quit("socket()");		// �ν� �Ұ� �����̸� ������ �Բ� ����
	MessageBox(NULL, "���� ���� ����", "�˸�", MB_OK);

	closesocket(tcp_sock);			// ������ ���������� ������ �ݾƾ� �Ѵ�.

	WSACleanup();
	return 0;
}