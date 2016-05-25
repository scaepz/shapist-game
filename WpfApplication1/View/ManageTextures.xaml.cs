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
using Editor.Model;
namespace Editor.View
{
    /// <summary>
    /// Interaction logic for ManageTextures.xaml
    /// </summary>
    public partial class ManageTextures : Window
    {
        ManageTexturesViewModel vm;
        public ManageTextures()
        {
            vm = new ManageTexturesViewModel();
            DataContext = vm;
            InitializeComponent();
            TextureListBox.SelectionChanged += TextureListBox_SelectionChanged;
            if (vm.Textures.Any())
            {
                TextureListBox.SelectedItem = vm.Textures.First();
            }
            
        }

        void TextureListBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            vm.CurrentTexture = (DataProvider.TextureViewModel)TextureListBox.SelectedItem;
            UpdateTextBoxes();
        }
        void UpdateTextBoxes()
        {
            NameTextBox.Text = vm.CurrentTexture.Name;
            FrameTextBox.Text = vm.CurrentTexture.Frames.ToString();
        }

        private void Close(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void Browse(object sender, RoutedEventArgs e)
        {

        }
    }
}
