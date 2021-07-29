using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace GnollHackClient.Pages.Game
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class TopScorePage : ContentPage
    {
        private string _fileName;
        private List<GHTopScoreItem> _topScores = null;

        public TopScorePage(string fileName)
        {
            InitializeComponent();
            _fileName = fileName;
            Assembly assembly = GetType().GetTypeInfo().Assembly;
            CloseButtonImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
        }

        public TopScorePage()
        {
            InitializeComponent();
            _fileName = "";
            NoScoresLabel.IsVisible = true;
            ScoresView.IsVisible = false;
            Assembly assembly = GetType().GetTypeInfo().Assembly;
            CloseButtonImage.Source = ImageSource.FromResource("GnollHackClient.Assets.button_normal.png", assembly);
        }

        private async void CloseButton_Clicked(object sender, EventArgs e)
        {
            await App.Current.MainPage.Navigation.PopModalAsync();
        }

        public bool ReadFile(out string errorMessage)
        {
            string res = "";
            try
            {
                string[] lines = File.ReadAllLines(_fileName);
                List<GHTopScoreItem> newTopScores = new List<GHTopScoreItem>();
                foreach (string line in lines)
                {
                    string[] lineitems = line.Split('\t');
                    GHTopScoreItem tsi = new GHTopScoreItem();
                    foreach(string lineitem in lineitems)
                    {
                        tsi.AddXlogLineItemData(lineitem);
                    }
                    newTopScores.Add(tsi);
                }

                /* Sort top scores */
                var q = from s in newTopScores orderby s.Score descending where (App.DeveloperMode || s.Mode == "normal") select s;
                _topScores = q.ToList();
                int rank = 1;
                int rankadd = 1;
                for (int i = 0; i < _topScores.Count; i++)
                {
                    if (i > 0 && _topScores[i - 1].Score > _topScores[i].Score)
                    {
                        rank += rankadd;
                        rankadd = 1;
                    }
                    else
                        rankadd++;

                    _topScores[i].Rank = rank;
                }

                /* Display in collection view */
                ScoresView.ItemsSource = _topScores;
                NoScoresLabel.IsVisible = (_topScores.Count == 0);
                ScoresView.IsVisible = (_topScores.Count != 0);
            }
            catch (Exception e)
            {
                errorMessage = e.Message;
                return false;
            }
            errorMessage = res;
            return true;
        }

        private double _currentPageWidth = 0;
        private double _currentPageHeight = 0;
        protected override void OnSizeAllocated(double width, double height)
        {
            base.OnSizeAllocated(width, height);
            if (width != _currentPageWidth || height != _currentPageHeight)
            {
                _currentPageWidth = width;
                _currentPageHeight = height;
                //Any dynamic behavior
            }
        }
    }
}