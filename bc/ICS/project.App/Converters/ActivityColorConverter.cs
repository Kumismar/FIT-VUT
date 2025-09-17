using System.Globalization;

namespace project.App.Converters;

public class ActivityColorConverter : IValueConverter
{
    public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
    {
        var color = value.ToString();
        Console.WriteLine(color);
        switch (color)
        {
            case "Color [A=255, R=255, G=0, B=0]":
                return Colors.Red;
            case "Color [A=255, R=0, G=0, B=255]":
                return Colors.Blue;
            case "Color [A=255, R=255, G=255, B=0]":
                return Colors.Yellow;
            case "Color [A=255, R=128, G=0, B=128]":
                return Colors.Purple;
            case "Color [A=255, R=255, G=192, B=203]":
                return Colors.Pink;
            case "Color [A=255, R=255, G=165, B=0]":
                return Colors.Orange;
            case "Color [A=255, R=165, G=42, B=42]":
                return Colors.Brown;
            default:
                return Colors.Green;
        }
    }
    public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
    {
        return null;
    }
}
