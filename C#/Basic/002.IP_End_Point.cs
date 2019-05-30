using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;               // 인터넷 관련 기능을 쓰려면 추가

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
            Console.WriteLine(endpoint);    // IP 주소와 포트 번호가 출력된다.
        }
    }
}