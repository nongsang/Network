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
            serverSock.Listen(5);   // 서버 소켓이 클라이언트의 연결을 받을 준비를 한다.
                                    // backlog는 연결을 요청하는 소켓들이 대기하는 큐다.
                                    // socket backlog와 kernel backlog가 있는데 kernel backlog는 OS마다 다르지만 리눅스는 1024가 기본이다.
                                    // socket backlog는 보통 5 ~ 16 사이의 크기로 정해준다.
                                    // 연결이 되면 backlog에서 대기하던 소켓이 빠진다.
                                    // 소켓이 빠진다고 해도 큐의 크기가 줄어드는 것은 아니다.
                                    // 대기열에 공간이 생긴것.

            Console.WriteLine("TCP 서버 연결 대기 중");

            Socket clientSock = serverSock.Accept();    // 서버 소켓에 연결 요청이 오면 연결을 받아들여 새로운 소켓을 생성하여 서로 연결해준다.
                                                        // 서버 소켓은 다시 연결 대기를 하고 clientSock과 연결된 소켓과 데이터를 주고 받는다.
                                                        // 여기서 서버 소켓은 연결대기와 연결을 담당하는 소켓, 클라이언트 소켓은 연결된 소켓과 데이터를 주고받는 역할을 하는 소켓으로 이해하면 된다.
            Console.WriteLine("TCP 클라이언트 연결 성공 : {0}", clientSock.RemoteEndPoint);  // clientSock.RemoteEndPoint로 연결된 소켓의 IP 주소와 포트번호를 출력한다.

            clientSock.Close(); // 소켓을 사용한 후에는 소켓을 닫아줘야 한다.
            serverSock.Close(); // 클라이언트 소켓이 전부 닫힌 후에 서버 소켓을 닫아줘야 한다.
        }
    }
}
