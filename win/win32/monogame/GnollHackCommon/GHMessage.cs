using System;
using System.Collections.Generic;

namespace GnollHackCommon
{
    public class GHCommandFromServer
    {
        public Guid Id { get; private set; }
        public string CommandName { get; set; }
        public List<object> CommandParameters;

        public GHCommandFromServer()
        {
            Id = Guid.NewGuid();
        }
    }
    public class GHResponseFromClient
    {
        public Guid CommandId { get; private set; }
        public Dictionary<string, object> ReturnValues { get; private set; }

        public GHResponseFromClient(Guid commandGuid)
        {
            CommandId = commandGuid;
            ReturnValues = new Dictionary<string, object>();
        }

    }
    public class GHCommandFromClient
    {
        public Guid Id { get; private set; }
        public string CommandName { get; set; }
        public List<object> CommandParameters;

        public GHCommandFromClient()
        {
            Id = Guid.NewGuid();
        }
    }
    public class GHResponseFromServer
    {
        public Guid CommandId { get; private set; }

        public GHResponseFromServer(Guid commandGuid)
        {
            CommandId = commandGuid;
        }
    }
}
