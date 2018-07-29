#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdio.h>

int main()
{
	// ���� �ʱ�ȭ ������ 01. Server����
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	MessageBox(NULL, "���� �ʱ�ȭ ����", "�˸�", MB_OK);

	u_short x1 = 0x1234;		// unsigned short ��
	u_long	y1 = 0x12345678;	// unsigned long ��
	u_short x2;
	u_long	y2;

	// ȣ��Ʈ ����Ʈ -> ��Ʈ��ũ ����Ʈ
	printf("[ȣ��Ʈ ����Ʈ -> ��Ʈ��ũ ����Ʈ]\n");		// ���� Intel CPU��� ������ �ٲ� ����� �� ���̰�, AMD CPU��� ��ȯ���� �ʰ� ��µ� ���̴�.
	printf("0x%x -> 0x%x\n", x1, x2 = htons(x1));	// host to network short	// ��Ʈ��ũ�� ����Ǿ� ���� �ʾƵ� ��ȯ�ȴ�.
	printf("0x%x -> 0x%x\n", y1, y2 = htonl(y1));	// host to network long		// ��Ʈ��ũ�� ����Ǿ� ���� �ʾƵ� ��ȯ�ȴ�.

	// ��Ʈ��ũ ����Ʈ -> ȣ��Ʈ ����Ʈ
	printf("\n[��Ʈ��ũ ����Ʈ -> ȣ��Ʈ ����Ʈ]\n");	// ���� Intel CPU��� ������ �ٲ� ����� �� ���̰�, AMD CPU��� ��ȯ���� �ʰ� ��µ� ���̴�.
	printf("0x%x -> 0x%x\n", x2, ntohs(x2));		// network to host short	// ��Ʈ��ũ�� ����Ǿ� �־�� ��ȯ�ȴ�.
	printf("0x%x -> 0x%x\n", y2, ntohl(y2));		// network to host long		// ��Ʈ��ũ�� ����Ǿ� �־�� ��ȯ�ȴ�.

	// �߸��� ��� ��
	printf("\n[�߸��� ��� ��]\n");
	printf("0x%x -> 0x%x\n", x1, htonl(x1));		// u_short���� u_long������ ��ȯ�Ͽ� �������� ���� �߸��̴�.

	WSACleanup();
	
	return 0;
}