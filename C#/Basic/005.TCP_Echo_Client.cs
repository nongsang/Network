using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;               // 인터넷 관련 기능을 쓰려면 추가
using System.Net.Sockets;       // TCP, UDP를 이용한 연결기능 및 소켓생성을 하려면 추가

namespace CPPPP
{
    class TCP_Echo_Client
    {
        public static void Main()
        {
            TcpClient tcpClient = new TcpClient("127.0.0.1", 9000);

            if (tcpClient.Connected)
            {
                Console.WriteLine("TCP 연결 성공");

                NetworkStream ns = tcpClient.GetStream();               // 연결된 서버에서 보내온 메세지나 보낼 메세지를 NetworkStream에 저장하도록 설정
                string Message = "Hello World!";                        // 보낼 메세지
                byte[] sendMessage = Encoding.ASCII.GetBytes(Message);  // NetworkStream에 값을 저장하려면 byte[]형으로 저장해야 한다.
                ns.Write(sendMessage, 0, sendMessage.Length);           // sendMassage의 값을 처음부터 전체 길이만큼 NetworkStream에 쓰기전용 버퍼에 저장한다.
                                                                        // 쓰기전용 버퍼에 데이터가 있으면 NetworkStream에 설정한 곳으로 데이터 전달.

                byte[] receiveByteMessage = new byte[32];               // 받은 데이터를 저장할 임시 저장소
                ns.Read(receiveByteMessage, 0, 32);                     // NetworkStream의 읽기전용 버퍼에 저장된 값을 처음부터 32바이트만큼 읽어서 receiveByteMessage에 저장
                string receiveMessage = Encoding.ASCII.GetString(receiveByteMessage);   // receiveByteMessage는 byte[]이므로 string으로 변환해서
                Console.WriteLine(receiveMessage);                                      // 출력한다.
            }
            else
            {
                Console.WriteLine("TCP 연결 실패");
            }

            tcpClient.Close();
        }
    }
}