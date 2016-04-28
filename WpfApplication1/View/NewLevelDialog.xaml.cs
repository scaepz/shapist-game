using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using Editor.ViewModel;

namespace Editor.View
{
    /// <summary>
    /// Interaction logic for Window1.xaml
    /// </summary>
    /// 
    public partial class NewLevelDialog : Window
    {
        LevelEditViewModel vm;
        MainWindow mw;
        public NewLevelDialog(LevelEditViewModel _vm, MainWindow _mw)
        {
            mw = _mw;
            vm = _vm;
            InitializeComponent();
        }
        private void Create(object sender, RoutedEventArgs e)
        {
            string name = nameTextBox.Text;
            if (string.IsNullOrEmpty(name)) return;
            int width;
            int height;
            if (!int.TryParse(widthTextBox.Text, out width))
            {
                return;
            }
            if (!int.TryParse(widthTextBox.Text, out height))
            {
                return;
            }
            bool? fill = fillCheckBox.IsChecked;
            char c = '0';
            if (fill.HasValue)
            {
                if (fill == true)
                {
                    c = '1';
                }
            }
            vm.CreateLevel(width, height, c, name);
            mw.RefreshTileMap();
            this.Close();
        }
        private void Cancel(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
