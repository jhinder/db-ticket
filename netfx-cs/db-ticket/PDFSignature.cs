using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace dbticket
{
    internal static class PDFSignature
    {

        internal static bool IsPDFv14(FileStream stream)
        {
            return stream.ReadCompareSection(0, 8, Constants.PDFVersion);
        }


        internal static bool HasBinarySignature(FileStream stream)
        {
            return stream.ReadCompareSection(9, 5, Constants.PDFBinary);
        }
    }
}
