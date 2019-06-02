using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;               // IP 주소 클래스 등 네트워크 관련 클래스를 쓰려면 추가

namespace CPPPP
{
    class IP_End_Point
    {
        public static void Main()
        {
            IPAddress ipAddr = IPAddress.Parse("192.168.1.10");
            IPEndPoint endpoint = new IPEndPoint(ipAddr, 9000); // EndPoint는 '접점', '종점', '종단점' 등으로 해석한다.
                                                                // TCP/IP 통신에서 접점은 'IP 주소' + '포트번호'를 나타낸다.
                                                                // IPEndPoint 클래스로 이 정보들을 묶어줄 수 있다.
            Console.WriteLine(endpoint);    // IP 주소와 포트 번호가 모두 출력된다.

            Console.WriteLine(endpoint.Address);        // 주소 따로 출력
            Console.WriteLine(endpoint.Port);           // 포트번호 따로 출력
            Console.WriteLine(endpoint.AddressFamily);  // 패밀리도 출력할 수 있다.
        }
    }
}