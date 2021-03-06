using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.SignalR;
using System.Runtime.InteropServices;


namespace GnollHackServer.Hubs
{
    public class GnollHackHub : Hub
    {
        private readonly SignInManager<IdentityUser> _signInManager;
        [DllImport(@"libgnollhack.dll", CharSet = CharSet.Unicode)]
        public static extern int DoSomeCalc2();

        public GnollHackHub(SignInManager<IdentityUser> signInManager)
        {
            _signInManager = signInManager;
        }

        public async Task SendMessage(string user, string message)
        {
            
            //Arg1 function
            //Arg2 and later can be any object
            await Clients.Caller.SendAsync("ReceiveMessage", user, message);
        }
        public async Task DoCalc()
        {
            int result = DoSomeCalc2();
            //Arg1 function
            //Arg2 and later can be any object
            await Clients.Caller.SendAsync("CalcResult", result);
        }

    }
}
