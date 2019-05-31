using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;               // 인터넷 관련 기능을 쓰려면 추가
using System.Net.Sockets;       // TCP, UDP를 이용한 연결기능 및 소켓생성을 하려면 추가

namespace CPPPP
{
    class TCP_Echo_Server
    {
        static void Main(string[] args)
        {
            TcpListener tcpListener = new TcpListener(IPAddress.Parse("127.0.0.1"), 9000);

            tcpListener.Start();
            Console.WriteLine("TCP 연결 대기 중");

            TcpClient tcpClient = tcpListener.AcceptTcpClient();    // TCP 클라이언트 연결
            Console.WriteLine("TCP 연결 성공");

            NetworkStream ns = tcpClient.GetStream();   // 연결된 클라이언트에서 보내온 메세지나 보낼 메세지를 NetworkStream에 저장하도록 설정
            byte[] receiveMassage = new byte[100];      // 받은 데이터를 저장할 임시 저장소
            ns.Read(receiveMassage, 0, 100);            // NetworkStream의 읽기전용 버퍼에 저장된 값을 처음부터 100바이트만큼 읽어서 receiveMassage에 저장
                                                        // NetworkStream의 읽기전용 버퍼에 저장된 값은 byte[]로 값을 받는다.
            string strMassage = Encoding.ASCII.GetString(receiveMassage);   // 값을 출력하려면 string으로 변환해줘야 한다.
            Console.WriteLine(strMassage);                                  // Console.WriteLine()는 string으로 출력해야 하기 때문 

            string echoMassage = "Hi~~";                                // 전달할 문자열
            byte[] sendMassage = Encoding.ASCII.GetBytes(echoMassage);  // 전달할 때도 byte[]로 전달해준다.
            ns.Write(sendMassage, 0, sendMassage.Length);               // sendMassage의 값을 처음부터 전체 길이만큼 NetworkStream에 쓰기전용 버퍼에 저장한다.
                                                                        // 쓰기전용 버퍼에 데이터가 있으면 NetworkStream에 설정한 곳으로 데이터 전달.

            ns.Close();         // NetworkStream를 먼저 닫아준다.
            tcpClient.Close();  // 연결한 클라이언트를 닫아서 끊어준다.
            tcpListener.Stop(); // 서버를 닫아서 연결요청을 받지 않게 한다.
                                // 순서는 꼭 맞춰줘야 한다.
        }
    }
}
