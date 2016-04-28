using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Editor.Model;

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

}
