using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Windows.Media.Imaging;
using System.ComponentModel;

namespace Editor.Model
{
    public static class DataProvider
    {
        static string shapistPath = "C:\\Users\\Jonathan\\Documents\\shapist-game\\SideScroller";
        public static string GetShapistPath()
        {
            return shapistPath;
        }
        public static string GetTexturePath(char c, string level)
        {
            string file = GetTextureFileForLevel(level);
            List<string> lines = file.Split('\n').ToList();
            int index = lines.IndexOf("[" + c + "]");
            return lines.ElementAt(index + 1);
        }
        public static string GetTextureFileForLevel(string level)
        {
            string path = Path.Combine(Environment.CurrentDirectory, "data", "levels", level) + "\\";
            return System.IO.File.ReadAllText(path + "entityData.txt");
        }
        public static string GetLevelFileForLevel(string level)
        {
            string path = Path.Combine(Environment.CurrentDirectory, "data", "levels", level) + "\\";
            return System.IO.File.ReadAllText(path + "level.txt");
        }
        public static int GetLevelWidth(string lvl)
        {
            return lvl.Split('\n').First().Length;
        }
        public static int GetLevelHeight(string lvl)
        {
            return lvl.Split('\n').Count() - 1;
        }
        public static void SaveLevel(Level level)
        {
            string path = Path.Combine(Environment.CurrentDirectory, "data", "levels", level.Name) + "\\";
            Directory.CreateDirectory(Path.GetDirectoryName(path));
            StreamWriter writer = new StreamWriter(path + "/level.txt", false);
            for (int y = 0; y < level.tileList.GetLength(0); y++)
            {
                for (int x = 0; x < level.tileList.GetLength(1); x++)
                {
                    writer.Write(level.tileList[x, y].Id.ToString());
                }
                writer.Write("\n");
            }
            writer.Close();
        }
        public static Level OpenLevel(string name)
        {
            Level level = new Level();
            level.Name = name;
            string levelData = GetLevelFileForLevel(name);
            if (string.IsNullOrEmpty(levelData)) throw new Exception("No level data");
            level.tileList = new Tile[GetLevelWidth(levelData), GetLevelHeight(levelData)];

            int x = 0;
            int y = 0;
            foreach (char c in levelData)
            {
                if (c == '\n')
                {
                    y++;
                    x = 0;
                }
                else
                {
                    level.tileList[x, y] = new Tile() { Id = c };
                    x++;
                }
            }
            return level;
        }
        private static string GetTextureFilePath()
        {

            string path = Path.Combine(Environment.CurrentDirectory, "data", "textures") + "\\";
            Directory.CreateDirectory(Path.GetDirectoryName(path));
            return path + "textureList.txt";
        }
        public static void ImportTexture(string name, int frames, string relativePath)
        {
            //check if name already exists
            if (ExistsTexture(name))
            {
                UpdateTexture(name, frames, relativePath);
                return;
            }
            StreamWriter writer = new StreamWriter(GetTextureFilePath(), true);
            writer.WriteLine("Name=" + name);
            writer.WriteLine("Path=" + relativePath);
            writer.WriteLine("Frms=" + frames);
            writer.Close();
        }

        public static void UpdateTexture(string name, int frames, string relativePath)
        {
            StreamReader reader = new StreamReader(GetTextureFilePath());
            var file = reader.ReadToEnd();
            reader.Close();
            var lines = file.Split('\n');
            for (int i = 0; i < lines.Count(); i++)
            {
                if (lines[i].StartsWith("Name="))
                {
                    if (lines[i].EndsWith(name + '\r') || lines[i].EndsWith(name + '\n') || lines[i].EndsWith(name))
                    {
                        lines[i + 1] = ("Path=" + relativePath);
                        lines[i + 2] = ("Frms=" + frames);
                    }
                }
            }
            StreamWriter writer = new StreamWriter(GetTextureFilePath(), false);
            foreach (var line in lines)
            {
                line.Replace("\n", "");
                line.Replace("\r", "");
                writer.WriteLine(line);
            }
            writer.Close();
        }

        public static bool ExistsTexture(string name)
        {
            try
            {
                StreamReader reader = new StreamReader(GetTextureFilePath());
                string file = reader.ReadToEnd();
                reader.Close();
                string[] lines = file.Split('\n');
                foreach (var line in lines)
                {
                    if (line.StartsWith("Name="))
                    {
                        if (line.EndsWith(name + '\r') || line.EndsWith(name + '\n') || line.EndsWith(name))
                            return true;
                    }
                }
                return false;
            }
            catch
            {
                return false;
            }

        }
        public static List<TextureViewModel> GetAllTextures()
        {
            string path = Path.Combine(Environment.CurrentDirectory, "data", "textures") + "\\";

            StreamReader reader = new StreamReader(path + "textureList.txt");
            string file = reader.ReadToEnd();
            reader.Close();
            List<string> lines = file.Split('\n').ToList();
            int counter = 0;
            List<TextureViewModel> txList = new List<TextureViewModel>();
            while (counter < lines.Count)
            {
                if (lines[counter].StartsWith("Name=") == false) { counter++; continue; }
                TextureViewModel tex = new TextureViewModel();
                tex.Name = lines.ElementAt(counter).Substring(5);
                counter++;
                tex.Path = lines.ElementAt(counter).Substring(5);
                counter++;
                tex.Frames = int.Parse(lines.ElementAt(counter).Substring(5));
                counter++;
                txList.Add(tex);
            }
            return txList;
        }

        public class TextureViewModel : INotifyPropertyChanged
        {
            public int Frames { get;set; }
            public string Path { get; set; }
            public string Name { get; set; }
            public override string ToString()
            {
                return Name;
            }
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

        public static BitmapImage GetImageFromPath(string path)
        {
            Uri uri = new Uri(path);
            try
            {
                BitmapImage bmp = new BitmapImage(uri);
                return bmp;
            }
            catch
            {
                throw new NotImplementedException("Implement 404 image");
            }
        }
    }
}
