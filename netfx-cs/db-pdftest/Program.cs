using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using dbticket;

namespace db_pdftest
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length < 2)
            {
                Console.WriteLine("Invalid argument count. You must pass the path to a file.");
                return;
            }
               
            TicketCheck tc_1 = new TicketCheck(args[1]);
        }
    }
}
