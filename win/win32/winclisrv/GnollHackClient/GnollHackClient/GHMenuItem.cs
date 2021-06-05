using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;

namespace GnollHackClient
{
    public class GHMenuItem : IEquatable<GHMenuItem>, INotifyPropertyChanged
    {
        public Int64 Identifier { get; set; }
        private int _count;
        public int Count
        {
            get
            {
                return _count;
            }
            set
            {
                CountSet = true;
                _count = value;
                if (PropertyChanged != null)
                {
                    PropertyChanged(this, new PropertyChangedEventArgs("Count"));
                    PropertyChanged(this, new PropertyChangedEventArgs("ConvertedCount"));
                    PropertyChanged(this, new PropertyChangedEventArgs("IsLabelVisible"));
                    PropertyChanged(this, new PropertyChangedEventArgs("LabelString"));
                }
            }
        }
        public bool IsLabelVisible { get { return (Count > 0 && MaxCount > 0 && Selected == true); } }
        public string LabelString { get { return (MaxCount <= 0 ? "N/A" : Count == -1 ? "All" : Count.ToString()); } }
        public int ConvertedCount { get { return (Count < 0 ? MaxCount + 1 : Count); } set { Count = (value >= MaxCount + 1 ? -1 : value); } }
        public int MaxCount { get; set; }
        public char Accelerator { get; set; }
        public char GroupAccelerator { get; set; }
        public string Text { get; set; }
        public int Attributes { get; set; }
        public int Glyph { get; set; }
        public bool Selected { get; set; }
        public int NHColor { get; set; }
        public bool Is_Heading { get; set; }
        public char HeadingGroupAccelerator { get; set; }
        public UInt32 MenuFlags { get; set; }
        public UInt64 Oid { get; set; }
        public UInt64 Mid { get; set; }

        public bool CountSet { get; set; }


        private int _selectedPickerIndex;
        public int SelectedPickerIndex
        {
            get
            {
                return _selectedPickerIndex;
            }
            set
            {
                _selectedPickerIndex = value;
                if (PropertyChanged != null)
                {
                    PropertyChanged(this, new PropertyChangedEventArgs("SelectedPickerIndex"));
                }
            }
        }
        private int _selectedSliderValue;
        public int SelectedSliderValue
        {
            get
            {
                return _selectedSliderValue;
            }
            set
            {
                _selectedSliderValue = value;
                if (PropertyChanged != null)
                {
                    PropertyChanged(this, new PropertyChangedEventArgs("SelectedSliderValue"));
                }
            }
        }

        public bool Equals(GHMenuItem other)
        {
            if (other == null) 
                return false;
            if(this.Identifier != 0)
                return (this.Identifier.Equals(other.Identifier));
            else
                return (this.Text.Equals(other.Text));
        }

        public GHMenuItem()
        {

        }

        public event PropertyChangedEventHandler PropertyChanged;
    }
}
