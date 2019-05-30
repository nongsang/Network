using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;               // 인터넷 관련 기능을 쓰려면 추가

namespace CPPPP
{
    class DNS
    {
        public static void Main()
        {
            IPAddress[] iPAddresses = Dns.GetHostAddresses("www.naver.com");    // 도메인에 해당하는 IP 주소들만을 받아온다.

            foreach (IPAddress iPAddress in iPAddresses)    // 하나의 도메인에 여러개의 IP 주소가 할당되어 있을 수 있다.
                Console.WriteLine(iPAddress);

            IPHostEntry entry = Dns.GetHostEntry("www.naver.com");      // Dns.GetHostEntry()로 도메인 이름과 IP 주소를 IPHostEntry에 저장한다.
            //IPHostEntry entry = Dns.GetHostByName("www.naver.com");     // GetHostEntry가 더 최신 기능이므로 쓰지 말자.
            //IPHostEntry entry = Dns.GetHostByAddress("64.200.123.1");   // GetHostEntry가 더 최신 기능이므로 쓰지 말자.

            foreach (string iPAddress in entry.Aliases)  // 하나의 도메인에 여러개의 IP 주소가 할당되어 있을 수 있다.
                Console.WriteLine(iPAddress);

            string myComputer = Dns.GetHostName();      // 로컬 호스트의 이름을 가져온다.

            Console.WriteLine(myComputer);              // 로컬 호스트의 이름 출력

            IPHostEntry myEntry = Dns.GetHostEntry(myComputer);     // 호스트 이름과 로컬 주소를 IPHostEntry에 저장

            foreach (IPAddress iPAddress in myEntry.AddressList)    // 로컬 주소 출력
                Console.WriteLine(iPAddress);
        }
    }
}