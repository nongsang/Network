using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;               // 인터넷 관련 기능을 쓰려면 추가
using System.Net.Sockets;       // TCP, UDP를 이용한 연결기능 및 소켓생성을 하려면 추가

namespace CPPPP
{
    class TCP_Server
    {
        static void Main(string[] args)
        {
            TcpListener tcpListener = new TcpListener(IPAddress.Parse("127.0.0.1"), 9000);  // IP 주소와 포트번호로 생성자를 호출하여 생성
                                                                                            // TCP 클라이언트는 IP 주소를 string으로 받지만 TCP 리스너는 IPAddress로 받는다.

            tcpListener.Start();                // TCP 클라이언트의 연결 요청을 대기
            Console.WriteLine("TCP 연결 대기 중");

            TcpClient tcpClient = tcpListener.AcceptTcpClient();    // TCP 클라이언트의 연결 요청이 오면 받아들인다.
                                                                    // 연결된 TCP 클라이언트의 정보는 tcpClient에 저장한다.
            Console.WriteLine("TCP 연결");

            tcpListener.Stop(); // TCP 리스너는 Close()가 아니라 Stop()으로 멈춘다.
        }
    }
}
