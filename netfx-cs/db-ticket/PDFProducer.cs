using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace dbticket
{
    internal static class PDFProducer
    {

        internal static bool HasProducerAtOffset(FileStream stream)
        {
            // 1748 is the offset from EOL where the Producer string usually starts
            long location = stream.Length - 1748;
            return stream.ReadCompareSection(location, Constants.Producer.Length, Constants.Producer);
        }

    }
}
