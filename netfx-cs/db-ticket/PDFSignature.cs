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
            return false;
        }


        internal static bool HasBinarySignature(FileStream stream)
        {
            return false;
        }
    }
}
