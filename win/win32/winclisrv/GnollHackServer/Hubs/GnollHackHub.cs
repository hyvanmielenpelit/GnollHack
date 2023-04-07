using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.SignalR;
using System.Runtime.InteropServices;
using GnollHackCommon;
using Microsoft.AspNetCore.Http;

namespace GnollHackServer.Hubs
{
    [Authorize]
    public class GnollHackHub : Hub
    {
        private readonly ServerGameCenter _serverGameCenter;
        private readonly IHttpContextAccessor _httpContextAccessor;

        [DllImport(@"gnollhackwin.dll", CharSet = CharSet.Unicode)]
        public static extern int DoSomeCalc2();

        /*
        public GnollHackHub()
        {

        }
        */
        public GnollHackHub(IHubContext<GnollHackHub> hubContext, IHttpContextAccessor httpContextAccessor)
        {
            _serverGameCenter = ServerGameCenter.Instance;
            _serverGameCenter.InitializeHubContext(hubContext);
            _httpContextAccessor = httpContextAccessor;
        }

        /*
        public GnollHackHub(IHubContext<GnollHackHub> hubContext) : this(hubContext, ServerGameCenter.Instance) { }

        public GnollHackHub(IHubContext<GnollHackHub> hubContext, ServerGameCenter serverGameCenter)
        {
            _serverGameCenter = serverGameCenter;
            _serverGameCenter.InitializeHubContext(hubContext);
        }

        public IEnumerable<ServerGame> GetAllServerGames()
        {
            return _serverGameCenter.GetAllServerGames();
        }
        */
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
        public async Task AddNewServerGame()
        {
            var userName = _httpContextAccessor.HttpContext.User.Identity.Name;
            int result = 1;
            _serverGameCenter.AddNewGame();
            //Arg1 function
            //Arg2 and later can be any object
            await Clients.Caller.SendAsync("AddNewGameResult", result);
        }
        public async Task ResponseFromClient(GHResponseFromClient response)
        {
            bool success = _serverGameCenter.AddResponseToIncomingQueue(response);
            await Clients.Caller.SendAsync("ResponseFromClientResult", response.CommandId, success ? 1 : 0);
        }

    }
}
