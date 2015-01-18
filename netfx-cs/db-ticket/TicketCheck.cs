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

        public const float MaximumScore = 5.5f;

        public double Result { get; private set; }

        /// <summary>
        /// Creates a new TicketCheck.
        /// </summary>
        /// <param name="inputStream">A FileStream representing the input file.</param>
        public TicketCheck(FileStream inputStream)
        {
            if (inputStream == null)
                throw new ArgumentNullException();

            if (inputStream.Length < 500000 || 525000 < inputStream.Length)
                throw new InvalidFileLengthException(String.Format("{0} bytes is not between 500 and 525 kilobytes.", inputStream.Length));

            bool pdfVersion = PDFSignature.IsPDFv14(inputStream);
            bool pdfSignature = PDFSignature.HasBinarySignature(inputStream);
            bool imageHex = ImageHexCode.ImageHexInStream(inputStream);
            bool producer = PDFProducer.HasProducerAtOffset(inputStream);

            Result = (pdfVersion ? 0.5 : 0.0)
                    + (pdfSignature ? 2.0 : 0.0)
                    + (imageHex ? 2.0 : 0.0)
                    + (producer ? 1.0 : 0.0);
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
