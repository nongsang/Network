using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;               // 인터넷 관련 기능을 쓰려면 추가

namespace CPPPP
{
    class IP_Address
    {
        public static void Main()
        {
            IPAddress ipAddr1 = IPAddress.Parse("2001:0000:85a3:0042:1000:8a8e:0370:7334"); // IPAddress는 IPv6도 자동으로 해석해 저장할 수 있다.
            Console.WriteLine(ipAddr1);

            IPAddress ipAddr2 = IPAddress.Parse("2001::7334");  // 2001과 7334모두 0000이라면 ::으로 줄일 수 있다.
            Console.WriteLine(ipAddr2);
        }
    }
}