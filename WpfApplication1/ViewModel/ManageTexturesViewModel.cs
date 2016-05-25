using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Editor.Model;
using System.Windows.Media.Imaging;
using System.ComponentModel;

namespace Editor.ViewModel
{
    public class ManageTexturesViewModel : INotifyPropertyChanged
    {
        public ManageTexturesViewModel()
        {
            Textures = DataProvider.GetAllTextures();
            if (Textures.Any())
            {
                CurrentTexture = Textures.First();
                CurrentImage = DataProvider.GetImageFromPath(CurrentTexture.Path);
            }
        }
        public List<DataProvider.TextureViewModel> Textures { get; set; }
        public BitmapImage CurrentImage { get; set; }
        public DataProvider.TextureViewModel CurrentTexture { get; set; }

        public event PropertyChangedEventHandler PropertyChanged;
        private void OnPropertyChanged(string info)
        {
            PropertyChangedEventHandler handler = PropertyChanged;
            if (handler != null)
            {
                handler(this, new PropertyChangedEventArgs(info));
            }
        }
    }

}
