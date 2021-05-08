using Microsoft.AspNetCore.SignalR;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace GnollHackServer.Hubs
{
    public class GameLoginHub : Hub
    {
        public async Task Login(string userName, string password)
        {

        }
    }
}
