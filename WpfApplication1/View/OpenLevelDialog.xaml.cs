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
    /// Interaction logic for OpenLevelDialog.xaml
    /// </summary>
    public partial class OpenLevelDialog : Window
    {
        LevelEditViewModel vm;
        MainWindow mw;
        public OpenLevelDialog(LevelEditViewModel _vm, MainWindow _mw)
        {
            vm = _vm;
            mw = _mw;
            InitializeComponent();
        }

        private void Open(object sender, RoutedEventArgs e)
        {
            vm.OpenLevel(levelTextBox.Text);
            mw.RefreshTileMap();
            this.Close();
        }
        private void Cancel(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
