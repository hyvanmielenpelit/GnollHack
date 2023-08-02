using FFImageLoading.Forms;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Reflection;
using System.Text;
using Xamarin.Forms;

namespace GnollHackX.Controls
{
    public class FFImageSourceConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            var sourceString = value as String;

            if (sourceString.StartsWith("resource://", StringComparison.OrdinalIgnoreCase))
            {
                return new EmbeddedResourceImageSource(new Uri(sourceString));
            }
            else
            {
                return new DataUrlImageSource(sourceString);
            }
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
