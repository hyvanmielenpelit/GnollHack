using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

using System.Collections.Concurrent;
using System.Threading;
using Microsoft.AspNetCore.SignalR;
using GnollHackServer.Hubs;
using GnollHackCommon;
using System.Diagnostics;

namespace GnollHackServer
{

    public class ServerGameCenter
    {
        // Singleton instance
        private readonly static Lazy<ServerGameCenter> _instance = new Lazy<ServerGameCenter>(() => new ServerGameCenter());

        private readonly ConcurrentDictionary<string, ServerGame> _serverGames = new ConcurrentDictionary<string, ServerGame>();

        private readonly object _updateServerGamesLock = new object();

        private readonly TimeSpan _updateInterval = TimeSpan.FromMilliseconds(250);
        private readonly Timer _timer;
        private volatile bool _updatingServerGames = false;

        /* Server Command Handling */
        private readonly object _outGoingServerCommandQueueLock = new object();
        private volatile bool _updatingOutGoingServerCommandQueue = false;

        private readonly object _inComingClientResponseQueueLock = new object();
        private volatile bool _updatingInComingClientResponseQueue = false;

        private readonly ConcurrentQueue<GHCommandFromServer> _outGoingServerCommandQueue = new ConcurrentQueue<GHCommandFromServer>();
        private readonly ConcurrentQueue<GHResponseFromClient> _inComingClientResponseQueue = new ConcurrentQueue<GHResponseFromClient>();

        /* Client Command Handling */
        private readonly object _inComingClientCommandQueueLock = new object();
        private volatile bool _updatingInComingClientCommandQueue = false;

        private readonly object _outGoingServerResponseQueueLock = new object();
        private volatile bool _updatingOutGoingServerResponseQueue = false;

        private readonly ConcurrentQueue<GHCommandFromClient> _inComingClientCommandQueue = new ConcurrentQueue<GHCommandFromClient>();
        private readonly ConcurrentQueue<GHResponseFromServer> _outGoingServerResponseQueue = new ConcurrentQueue<GHResponseFromServer>();

        private ServerGameCenter()
        {
            _serverGames.Clear();
            _timer = new Timer(UpdateServerGames, null, _updateInterval, _updateInterval);
        }

        private IHubContext<GnollHackHub> _hubContext;
        public void InitializeHubContext(IHubContext<GnollHackHub> hubContext)
        {
            _hubContext = hubContext;
            Clients = hubContext.Clients;
        }

        public void AddNewGame()
        {
            ServerGame newgame = new ServerGame();
            /*
            var games = new List<ServerGame>
            {
                new ServerGame { }
            };
            games.ForEach(serverGame => _serverGames.TryAdd("1", serverGame));
            */
            _serverGames.TryAdd("1", newgame);
            newgame.StartGame();
        }

        public static ServerGameCenter Instance
        {
            get
            {
                return _instance.Value;
            }
        }

        private IHubClients Clients
        {
            get;
            set;
        }

        public IEnumerable<ServerGame> GetAllServerGames()
        {
            return _serverGames.Values;
        }

        private void UpdateServerGames(object state)
        {
            lock (_updateServerGamesLock)
            {
                if (!_updatingServerGames)
                {
                    _updatingServerGames = true;

                    foreach (var serverGame in _serverGames.Values)
                    {
                        if (TryUpdateServerGame(serverGame))
                        {
                            BroadcastServerGameState(serverGame);
                        }
                    }

                    _updatingServerGames = false;
                }
            }
        }

        public bool AddCommandToOutgoingQueue(GHCommandFromServer command)
        {
            bool send_success = false;
            lock (_outGoingServerCommandQueueLock)
            {
                for(int i = 0; i < 20; i++)
                {
                    if (!_updatingOutGoingServerCommandQueue)
                    {
                        _updatingOutGoingServerCommandQueue = true;
                        BroadcastServerCommand(command);
                        send_success = true;
                        _updatingOutGoingServerCommandQueue = false;
                        break;
                    }
                    else
                    {
                        Thread.Sleep(5);
                        Debug.WriteLine("AddMessageToOutGoingQueue: Trying Again - Count: " + i);
                    }
                }

                if(send_success)
                    Debug.WriteLine("AddMessageToOutGoingQueue Successful");
                else
                    Debug.WriteLine("AddMessageToOutGoingQueue Failed!");
            }

            return send_success;

        }
        public bool AddResponseToIncomingQueue(GHResponseFromClient response)
        {
            bool append_success = false;
            lock (_inComingClientResponseQueueLock)
            {
                for (int i = 0; i < 20; i++)
                {
                    if (!_updatingInComingClientResponseQueue)
                    {
                        _updatingInComingClientResponseQueue = true;
                        _inComingClientResponseQueue.Append(response);
                        append_success = true;
                        _updatingInComingClientResponseQueue = false;
                        break;
                    }
                    else
                    {
                        Thread.Sleep(5);
                        Debug.WriteLine("AddResponseToIncomingQueue: Trying Again - Count: " + i);
                    }
                }

                if (append_success)
                    Debug.WriteLine("AddResponseToIncomingQueue Successful");
                else
                    Debug.WriteLine("AddResponseToIncomingQueue Failed!");

            }
            return append_success;
        }

        private bool TryUpdateServerGame(ServerGame serverGame)
        {
            return true;
        }
        private async void BroadcastServerGameState(ServerGame serverGame)
        {
            await Clients.All.SendAsync("GameAliveResult", serverGame.IsGameAlive()); //.All.updateStockPrice(stock);
        }
        private async void BroadcastServerCommand(GHCommandFromServer command)
        {
            await Clients.All.SendAsync("CommandFromServer", command);
        }

        public async void ServerCenter_ExitHack(ServerGame serverGame, int status)
        {
            await Clients.All.SendAsync("Client_ExitHack", serverGame.GetHashCode(), status);
        }
        public async void ServerCenter_PlayerSelection(ServerGame serverGame)
        {
            await Clients.All.SendAsync("Client_PlayerSelection", serverGame.GetHashCode());
        }
    }
}
