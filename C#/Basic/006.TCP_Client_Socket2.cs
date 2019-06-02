using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;               // IP 주소 클래스 등 네트워크 관련 클래스를 쓰려면 추가
using System.Net.Sockets;       // TCP, UDP 및 소켓관련 클래스를 사용하려면 추가

namespace CPPPP
{
    class TCP_Client_Socket2
    {
        public static void Main()
        {
            Socket clientSock = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

            clientSock.Connect("127.0.0.1", 9000);

            Console.WriteLine("TCP 서버 연결 성공 : {0}", clientSock.RemoteEndPoint);

            byte[] receiveByteMessage = new byte[20];   // 연결된 소켓으로부터 받은 데이터를 저장할 임시 저장소
                                                        // 데이터를 받을 때 byte의 배열로 받기 때문
            clientSock.Receive(receiveByteMessage);     // 연결된 소켓으로부터 받은 데이터를 receiveByteMessage에 저장
            string receiveMessage = Encoding.Unicode.GetString(receiveByteMessage); // 받은 데이터를 Unicode로 변환해서 string으로 변환
                                                                                    // Unicode 뿐만 아니라 UTF-8, ASCII로도 가능
                                                                                    // string으로 변환해야 화면출력 가능
            Console.WriteLine("{0} : {1}", clientSock.RemoteEndPoint, receiveMessage);  // clientSock.RemoteEndPoint로 연결된 상대방 소켓의 IP 주소와 포트번호, 받은 데이터를 출력

            string sendMessage = "Hi";                                          // 보낼 데이터
            byte[] sendByteMessage = Encoding.Unicode.GetBytes(sendMessage);    // 보낼 데이터를 Unicode로 변환해서 byte의 배열로 변환
                                                                                // 데이터를 보낼 때 byte의 배열로 보내기 때문
            clientSock.Send(sendByteMessage);                                   // 연결된 소켓으로 데이터를 보낸다.

            clientSock.Close();
        }
    }
}