using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.SignalR;

namespace GnollHackServer.Hubs
{
    public class GnollHackHub : Hub
    {
        private readonly SignInManager<IdentityUser> _signInManager;

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

        public async Task Login(string userName, string password)
        {
            var result = await _signInManager.PasswordSignInAsync(userName, password, true, lockoutOnFailure: false);

            if (result == SignInResult.Success)
            {
                await Clients.Caller.SendAsync("LoginMessage", userName, "Login Successful");
            }
            else
            {
                await Clients.Caller.SendAsync("LoginMessage", userName, "Login Failed");
            }
        }

    }
}
