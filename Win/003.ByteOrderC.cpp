#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdio.h>

int main()
{
	u_short x1 = 0x1234;		// WinSock2.h에 정의되어 있는 자료형
	u_long	y1 = 0x12345678;	// u_short는 unsigned short, u_long는 unsigned long이다.
	u_short x2;
	u_long	y2;

	printf("[호스트 바이트 -> 네트워크 바이트]\n");	// 만약 Intel CPU라면 순서가 바뀌어서 출력이 될 것이고, AMD CPU라면 변환되지 않고 출력될 것이다.
	printf("0x%x -> 0x%x\n", x1, x2 = htons(x1));	// host to network short, 네트워크가 연결되어 있지 않아도 변환된다.
	printf("0x%x -> 0x%x\n", y1, y2 = htonl(y1));	// host to network long

	printf("\n[네트워크 바이트 -> 호스트 바이트]\n");
	printf("0x%x -> 0x%x\n", x2, ntohs(x2));		// network to host short, 네트워크가 연결되어 있어야 변환된다.
	printf("0x%x -> 0x%x\n", y2, ntohl(y2));		// network to host long

	printf("\n[잘못된 사용 예]\n");
	printf("0x%x -> 0x%x\n", x1, htonl(x1));		// u_short값을 u_long값으로 변환하여 보내려는 것이 잘못이다.
	
	return 0;
}