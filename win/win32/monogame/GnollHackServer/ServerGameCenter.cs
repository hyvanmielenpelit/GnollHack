using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

using System.Collections.Concurrent;
using System.Threading;
using Microsoft.AspNetCore.SignalR;
using GnollHackServer.Hubs;

namespace GnollHackServer
{

    public class ServerGameCenter
    {
        // Singleton instance
        private readonly static Lazy<ServerGameCenter> _instance = new Lazy<ServerGameCenter>(() => new ServerGameCenter());

        private readonly ConcurrentDictionary<string, ServerGame> _serverGames = new ConcurrentDictionary<string, ServerGame>();

        private readonly object _updateServerGamesLock = new object();

        private ServerGameCenter()
        {
            _serverGames.Clear();
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
            /*
            lock (_updateStockPricesLock)
            {
                if (!_updatingStockPrices)
                {
                    _updatingStockPrices = true;

                    foreach (var stock in _stocks.Values)
                    {
                        if (TryUpdateStockPrice(stock))
                        {
                            BroadcastStockPrice(stock);
                        }
                    }

                    _updatingStockPrices = false;
                }
            }
            */
        }

        private bool TryUpdateServerGame(ServerGame serverGame)
        {
            return true;
        }

        private async void BroadcastServerGameStates(ServerGame serverGame)
        {
            await Clients.All.SendAsync("SendGameState", serverGame); //.All.updateStockPrice(stock);
        }

    }
}
