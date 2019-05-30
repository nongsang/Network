using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;               // 인터넷 관련 기능을 쓰려면 추가
using System.Net.Sockets;       // TCP, UDP를 이용한 연결기능 및 소켓생성을 하려면 추가

namespace CPPPP
{
    class TCP_Client
    {
        public static void Main()
        {
            TcpClient tcpClient = new TcpClient("127.0.0.1", 9000); // IP 주소와 포트번호로 생성자를 호출하여 생성
                                                                    // 생성과 동시에 주어진 IP 주소와 포트번호로 연결을 요청한다.
                                                                    // TCP 리스너는 IP 주소를 IPAddress로 받지만 TCP 클라이언트는 string으로 받는다.

            //tcpClient.Connect(IPEndPoint);                // TCP 클라이언트는 생성하자 마자
            //tcpClient.Connect(IPAddress, portNum);        // 주어진 IP 주소와 포트번호로 연결 요청을 한다.
            //tcpClient.Connect(IPAddress[], portNum);      // 나중에 연결 요철을 하고 싶으면
            //tcpClient.Connect(DomainNameorIP, portNum);   // 자신이 편한 매개변수로 정의된 Connect()를 호출해서 연결 요청을 하자.

            if (tcpClient.Connected)                // 연결 성공하면
                Console.WriteLine("TCP 연결 성공");
            else                                    // 연결 실패하면
                Console.WriteLine("TCP 연결 실패");

            tcpClient.Close();  // TCP 클라이언트는 Stop()이 아니라 Close()다.
        }
    }
}