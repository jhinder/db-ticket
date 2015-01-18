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
            if (args.Length < 1)
            {
                Console.WriteLine("Invalid argument count. You must pass the path to a file.\nPress a key to exit.");
                Console.ReadKey();
                return;
            }
               
            TicketCheck tc_1 = new TicketCheck(args[0]);
            Console.Write("Result: ");
            Console.Write(String.Format("{0} of {1} points\n", tc_1.Result, TicketCheck.MaximumScore));

            Console.WriteLine("Press a key to exit.");
            Console.ReadKey();
        }
    }
}
