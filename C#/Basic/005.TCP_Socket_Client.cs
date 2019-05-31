﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;               // 인터넷 관련 기능을 쓰려면 추가
using System.Net.Sockets;       // TCP, UDP를 이용한 연결기능 및 소켓생성을 하려면 추가

namespace CPPPP
{
    class TCP_Socket_Client
    {
        public static void Main()
        {
            Socket clientSock = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

            clientSock.Connect("127.0.0.1", 9000);      // IP 주소 127.0.0.1에 포트번호 9000으로 연결 요청
                                                        //clientSock.Connect("192.168.1.2", 9000);
                                                        //clientSock.Connect(new IPEndPoint(IPAddress.Loopback, 9000));   // 위와 같은 의미

            Console.WriteLine("TCP 서버 연결 성공 : {0}", clientSock.RemoteEndPoint); // clientSock.RemoteEndPoint로 연결된 소켓의 IP 주소와 포트번호를 출력한다.
        }
    }
}