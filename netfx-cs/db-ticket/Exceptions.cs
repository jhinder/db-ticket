using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace dbticket
{

    [Serializable]
    public class InvalidFileLengthException : Exception
    {
        public InvalidFileLengthException() { }
        public InvalidFileLengthException(string message) : base(message) { }
        public InvalidFileLengthException(string message, Exception inner) : base(message, inner) { }
        protected InvalidFileLengthException(
          System.Runtime.Serialization.SerializationInfo info,
          System.Runtime.Serialization.StreamingContext context) : base(info, context)
        { }
    }
}
