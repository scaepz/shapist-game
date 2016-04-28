using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
namespace Editor.Model
{
    public class Level
    {
        public string Name { get; set; }
        public Tile[,] tileList { get; set; }
        public char GetTileType(int x, int y)
        {
            return tileList[x, y].Id;
        }
        public void SetTile(int x, int y, char id)
        {
            tileList[x, y].Id = id;
        }
        public void CreateLevel(int x, int y, char defaultId)
        {
            tileList = new Tile[x, y];
            for (int i = 0; i < x; i++)
            {
                for (int k = 0; k < y; k++)
                {
                    tileList[i, k] = new Tile { Id = defaultId };
                }
            }
        }

    }
    public class Tile
    {
        public char Id;
    }
}
