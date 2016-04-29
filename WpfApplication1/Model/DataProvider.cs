using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace Editor.Model
{
    static class DataProvider
    {
        static string shapistPath = "C:\\Users\\Jonathan\\Documents\\shapist-game\\SideScroller";
        public static string GetShapistPath()
        {
            return shapistPath;
        }
        public static string GetTexturePath(char c, string level)
        {
            string file = GetTextureFile(level);
            List<string> lines = file.Split('\n').ToList();
            int index = lines.IndexOf("[" + c + "]");
            return lines.ElementAt(index + 1);
        }
        public static string GetTextureFile(string level)
        {
            string path = Path.Combine(Environment.CurrentDirectory, "data", "levels", level) + "\\";
            return System.IO.File.ReadAllText(path + "entityData.txt");
        }
        public static string GetLevelFile(string level)
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
            string levelData = GetLevelFile(name);
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
        public static void ImportTexture(string name, int frames, string relativePath)
        {
            //check if name already exists
            string path = Path.Combine(Environment.CurrentDirectory, "data", "textures") + "\\";
            Directory.CreateDirectory(Path.GetDirectoryName(path));
            StreamWriter writer = new StreamWriter(path + "textureList.txt", true);
            writer.WriteLine("Name=" + name);
            writer.WriteLine("Path=" + relativePath);
            writer.WriteLine("Frms=" + frames);
        }

        public static List<TextureViewModel> GetAllTextures()
        {
            string path = Path.Combine(Environment.CurrentDirectory, "data", "textures") + "\\";
            StreamReader reader = new StreamReader(path + "textureList.txt");
            string file = reader.ReadToEnd();
            List<string> lines = file.Split('\n').ToList();
            int counter = 0;
            List<TextureViewModel> txList = new List<TextureViewModel>();
            while (counter < lines.Count)
            {
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

        public class TextureViewModel
        {
            public int Frames { get; set; }
            public string Path { get; set; }
            public string Name { get; set; }
        }
    }
}
