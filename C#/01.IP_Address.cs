using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static System.Console;    // WriteLine()이라고 줄여서 쓰고싶어
using System.Net;       // 인터넷 관련 기능을 쓰려면 추가

namespace CPPPP
{
    class IP_Address
    {
        public static void Main()
        {
            IPAddress ipAddr1 = IPAddress.Parse("202.179.177.21");  // IPAddress는 IP주소를 담는 클래스다.
            WriteLine(ipAddr1);

            IPAddress ipAddr2 = new IPAddress(new byte[] { 202, 179, 177, 21 });    // byte형으로 값을 하나씩 전달해도 된다.
            WriteLine(ipAddr2);

            IPAddress ipAddr3 = IPAddress.Parse("2001:0000:85a3:0042:1000:8a8e:0370:7334"); // IPv6도 자동으로 해석해 담을 수 있다.
            WriteLine(ipAddr3);

            IPAddress ipAddr4 = IPAddress.Parse("2001::7334");  // 2001과 7334 이외에 모두 0000이라면 ::으로 줄일 수 있다.
            WriteLine(ipAddr4);
        }
    }
}
