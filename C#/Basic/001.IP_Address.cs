using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;               // IP 주소 클래스 등 네트워크 관련 클래스를 쓰려면 추가

namespace CPPPP
{
    class IP_Address
    {
        public static void Main()
        {
            IPAddress ipAddr1 = IPAddress.Parse("202.179.177.21");  // IPAddress는 IP주소를 담는 클래스다.
            Console.WriteLine(ipAddr1);

            IPAddress ipAddr2 = new IPAddress(new byte[] { 202, 179, 177, 21 });    // byte형으로 값을 하나씩 전달해도 된다.
            Console.WriteLine(ipAddr2);

            IPAddress ipAddr3 = IPAddress.Parse("2001:0000:85a3:0042:1000:8a8e:0370:7334"); // IPAddress는 IPv6도 자동으로 해석해 저장할 수 있다.
            Console.WriteLine(ipAddr3);

            //IPAddress ipAddr4 = IPAddress.Parse("2001:0000:0000:0000:0000:0000:0000:7334"); // 아래와 같은 주소
            IPAddress ipAddr4 = IPAddress.Parse("2001::7334");  // 2001과 7334모두 0000이라면 ::으로 줄일 수 있다.
            Console.WriteLine(ipAddr4);
        }
    }
}