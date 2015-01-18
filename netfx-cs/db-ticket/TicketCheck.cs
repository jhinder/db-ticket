using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace dbticket
{
    public class TicketCheck
    {

        /// <summary>
        /// Creates a new TicketCheck.
        /// </summary>
        /// <param name="inputStream">A FileStream representing the input file.</param>
        public TicketCheck(FileStream inputStream)
        {

        }

        /// <summary>
        /// Creates a new TicketCheck.
        /// </summary>
        /// <param name="inputFile">A FileInfo representing the input file.</param>
        public TicketCheck(FileInfo inputFile)
            : this(inputFile.OpenRead())
        {
        }

        /// <summary>
        /// Creates a new TicketCheck.
        /// </summary>
        /// <param name="inputFilePath">The path to the input file.</param>
        public TicketCheck(string inputFilePath)
            : this(File.Open(inputFilePath, FileMode.Open))
        {
        }

    }
}
