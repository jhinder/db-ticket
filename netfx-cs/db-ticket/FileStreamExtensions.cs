using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace dbticket
{
    internal static class FileStreamExtensions
    {

        internal static bool ReadCompareSection(this FileStream stream, long offset, int length, string comparison)
        {
            if (stream == null || comparison == null)
                throw new ArgumentNullException("Neither stream nor comparison must be null.");

            if (offset < 0 || length < 0)
                throw new ArgumentOutOfRangeException("Length and Offset must be positive.");

            byte[] subsection = new byte[length];
            stream.Position = offset;
            stream.Read(subsection, 0, length);
            var encodedRef = UTF8Encoding.Default.GetBytes(comparison);
            return subsection.SequenceEqual(encodedRef);
        }

    }
}
