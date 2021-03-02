using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.SignalR;

namespace GnollHackServer.Hubs
{
    public class GnollHackHub : Hub
    {
        public async Task SendMessage(string user, string message)
        {
            //Arg1 function
            //Arg2 and later can be any object
            await Clients.All.SendAsync("ReceiveMessage", user, message);
        }

        public async Task Login(string userName, string password)
        {
            await Clients.Caller.SendAsync("ReceiveMessage", userName, "Login successful");
        }
    }
}
