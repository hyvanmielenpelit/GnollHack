using System;
using System.Collections.Generic;
using System.Text;
using Xamarin.Forms;

namespace GnollHackX
{
    public class GHButton : Button
    {
        public int GHCommand 
        { 
            get 
            {
                if (RawCommand != 0)
                    return RawCommand;
                if(ApplyMeta) 
                    return GHUtils.Meta((int)Letter);
                if (ApplyCtrl)
                    return GHUtils.Ctrl((int)Letter);
                else
                    return (int)Letter;
            }
        }

        public static readonly BindableProperty LetterProperty = BindableProperty.Create(nameof(Letter), typeof(char), typeof(GHButton), (char)0);
        public static readonly BindableProperty ApplyMetaProperty = BindableProperty.Create(nameof(ApplyMeta), typeof(bool), typeof(GHButton), false);
        public static readonly BindableProperty ApplyCtrlProperty = BindableProperty.Create(nameof(ApplyCtrl), typeof(bool), typeof(GHButton), false);
        public static readonly BindableProperty RawCommandProperty = BindableProperty.Create(nameof(RawCommand), typeof(int), typeof(GHButton), 0);

        public char Letter
        {
            get => (char)GetValue(GHButton.LetterProperty);
            set => SetValue(GHButton.LetterProperty, value);
        }
        public bool ApplyMeta
        {
            get => (bool)GetValue(GHButton.ApplyMetaProperty);
            set => SetValue(GHButton.ApplyMetaProperty, value);
        }
        public bool ApplyCtrl
        {
            get => (bool)GetValue(GHButton.ApplyCtrlProperty);
            set => SetValue(GHButton.ApplyCtrlProperty, value);
        }
        public int RawCommand
        {
            get => (int)GetValue(GHButton.RawCommandProperty);
            set => SetValue(GHButton.RawCommandProperty, value);
        }

        public GHButton()
        {

        }

    }
}
