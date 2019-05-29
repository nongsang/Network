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
            IPAddress ipAddr1 = IPAddress.Parse("202.179.177.21");  // IPAddress는 IP주소를 담는 클래스다.
            Console.WriteLine(ipAddr1);

            IPAddress ipAddr2 = new IPAddress(new byte[] { 202, 179, 177, 21 });    // byte형으로 값을 하나씩 전달해도 된다.
            Console.WriteLine(ipAddr2);
        }
    }
}