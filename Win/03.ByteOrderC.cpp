#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdio.h>

int main()
{
	u_short x1 = 0x1234;		// WinSock2.h�� ���ǵǾ� �ִ� �ڷ���
	u_long	y1 = 0x12345678;	// u_short�� unsigned short, u_long�� unsigned long�̴�.
	u_short x2;
	u_long	y2;

	printf("[ȣ��Ʈ ����Ʈ -> ��Ʈ��ũ ����Ʈ]\n");	// ���� Intel CPU��� ������ �ٲ� ����� �� ���̰�, AMD CPU��� ��ȯ���� �ʰ� ��µ� ���̴�.
	printf("0x%x -> 0x%x\n", x1, x2 = htons(x1));	// host to network short, ��Ʈ��ũ�� ����Ǿ� ���� �ʾƵ� ��ȯ�ȴ�.
	printf("0x%x -> 0x%x\n", y1, y2 = htonl(y1));	// host to network long

	printf("\n[��Ʈ��ũ ����Ʈ -> ȣ��Ʈ ����Ʈ]\n");
	printf("0x%x -> 0x%x\n", x2, ntohs(x2));		// network to host short, ��Ʈ��ũ�� ����Ǿ� �־�� ��ȯ�ȴ�.
	printf("0x%x -> 0x%x\n", y2, ntohl(y2));		// network to host long

	printf("\n[�߸��� ��� ��]\n");
	printf("0x%x -> 0x%x\n", x1, htonl(x1));		// u_short���� u_long������ ��ȯ�Ͽ� �������� ���� �߸��̴�.
	
	return 0;
}