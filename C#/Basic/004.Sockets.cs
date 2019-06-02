using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;               // IP 주소 클래스 등 네트워크 관련 클래스를 쓰려면 추가
using System.Net.Sockets;       // TCP, UDP 및 소켓관련 클래스를 사용하려면 추가

namespace CPPPP
{
    class Sockets
    {
        public static void Main()
        {
            Socket socket1 = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);   // IPv4, TCP 소켓에 TCP로 데이터를 주고 받는 소켓 생성
            //Socket socket1 = new Socket(2, 1, 0);                                                           // Windows Socket 처럼 정수로 값을 전달 할순 없다.
            Socket socket2 = new Socket(AddressFamily.InterNetworkV6, SocketType.Dgram, ProtocolType.Udp);  // IPv6, UDP 소켓에 UDP로 데이터를 주고 받는 소켓 생성
            Socket socket3 = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.IP);    // ProtocolType.Tcp 대신 ProtocolType.IP로 데이터를 주고 받는다.
                                                                                                            // Tcp는 신뢰성있는 데이터 전송 방식이며, IP는 UDP와 같이 신뢰성이 없는 데이터 전송 방식이다.
                                                                                                            // 그냥 소켓 특성에 맞춰서 ProtocolType을 정해주자.
        }
    }
}