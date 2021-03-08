using System;

namespace GnollHackCommon
{
    public class GHMessage
    {
        public Guid Id { get; private set; }

        public GHMessage()
        {
            Id = Guid.NewGuid();
        }
    }
}
