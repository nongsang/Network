using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;               // 인터넷 관련 기능을 쓰려면 추가
using System.Net.Sockets;       // TCP, UDP를 이용한 연결기능 및 소켓생성을 하려면 추가

namespace CPPPP
{
    class TCP_Socket_Server
    {
        static void Main(string[] args)
        {
            Socket serverSock = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

            serverSock.Bind(new IPEndPoint(IPAddress.Any, 9000));   // 소켓의 정보를 IP 주소 0.0.0.0 에 포트번호 9000으로 설정한다.
                                                                    // IPAddress.Any는 사용가능한 IP 주소를 자동으로 설정해준다.
                                                                    // 루프백주소나 공유기주소 등
            serverSock.Listen(100);                                 // 서버 소켓이 클라이언트의 연결을 받을 준비를 한다.
                                                                    // 정수값을 정해주면 한꺼번에 연결을 요청하는 경우 정해준 정수값의 갯수씩 처리를 한다.
            Console.WriteLine("TCP 서버 연결 대기 중");

            Socket clientSock = serverSock.Accept();                // 서버 소켓에 연결 요청이 오면 연결을 받아들인다.
                                                                    // 연결된 소켓의 정보는 clientSock에 저장한다.
            Console.WriteLine("TCP 클라이언트 연결 성공 : {0}", clientSock.RemoteEndPoint);  // clientSock.RemoteEndPoint로 연결된 소켓의 IP 주소와 포트번호를 출력한다.
        }
    }
}
