using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

using System.Collections.Concurrent;
using System.Threading;
using Microsoft.AspNetCore.SignalR;
using GnollHackServer.Hubs;
using GnollHackCommon;

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

        private bool TryUpdateServerGame(ServerGame serverGame)
        {
            return true;
        }
        private async void BroadcastServerGameState(ServerGame serverGame)
        {
            await Clients.All.SendAsync("GameAliveResult", serverGame.IsGameAlive()); //.All.updateStockPrice(stock);
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
