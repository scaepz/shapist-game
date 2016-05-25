using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Editor.Model;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel;

namespace Editor.ViewModel
{
    public class LevelEditViewModel
    {
        public Level lvl;
        public LevelEditViewModel()
        { }
        public void CreateLevel(int x, int y, char defaultChar, string name)
        {
            lvl = new Level();
            lvl.CreateLevel(x,y, defaultChar);
            lvl.Name = name;
        }
        public void OpenLevel(string name)
        {
            lvl = DataProvider.OpenLevel(name);
        }
        public void SaveLevel()
        {
            DataProvider.SaveLevel(lvl);
        }

    }
    public class ImportTextureViewModel : INotifyPropertyChanged
    {
        [Validation.UniqueTextureName]
        [Required(ErrorMessage = "Name is required")]
        public string Name { get; set; }
        
        [Range(1, 200,ErrorMessage="Frame amount must be in the range of [1, 200]")]
        [Required(ErrorMessage = "Frame amount is required")]
        public int Frames { get; set; }

        public void Import()
        {
            DataProvider.ImportTexture(Name, Frames, Path);
        }
        public string Path { get; set; }


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
