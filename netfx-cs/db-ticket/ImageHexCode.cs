using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace dbticket
{
    internal static class ImageHexCode
    {

        internal static bool ImageHexInStream(FileStream stream)
        {
            return stream.ReadCompareSection(0x1A6, 0x11, Constants.ImageHex);
        }

    }
}
