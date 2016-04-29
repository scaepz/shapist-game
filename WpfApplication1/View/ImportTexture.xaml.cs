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

namespace Editor.View
{
    /// <summary>
    /// Interaction logic for ImportTexture.xaml
    /// </summary>
    public partial class ImportTexture : Window
    {
        public ImportTexture()
        {
            InitializeComponent();
        }
        string filePath;
        private void Browse(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();

            // Set filter for file extension and default file extension 
            dlg.DefaultExt = ".png";
            dlg.Filter = "All files (*.*)|*.*";
            dlg.InitialDirectory = Model.DataProvider.GetShapistPath();

            // Display OpenFileDialog by calling ShowDialog method 
            Nullable<bool> result = dlg.ShowDialog();

            // Get the selected file name and display in a TextBox 
            if (result == true)
            {
                // Open document 
                string filename = dlg.FileName;
                nameTextBox.Text = filename.Substring(Model.DataProvider.GetShapistPath().Length);     
                var uri = new Uri(filename);
                BitmapImage bmp = new BitmapImage(uri);
                img.Source = bmp;
                filePath = filename;
            }
        }

        private void Cancel(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void Import(object sender, RoutedEventArgs e)
        {
            int frames;
            if (!int.TryParse(frameTextBox.Text, out frames)) return;
            if (!string.IsNullOrEmpty(filePath))
                Model.DataProvider.ImportTexture(nameTextBox.Text, frames, filePath);
        }


    }
}
