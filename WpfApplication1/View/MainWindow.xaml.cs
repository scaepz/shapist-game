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
using System.Windows.Navigation;
using System.Windows.Shapes;
using Editor.ViewModel;
using Editor.View;

namespace Editor.View
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        int cellSize;
        char brush;
        LevelEditViewModel vm;
        Dictionary<char, BitmapImage> imgMap;
        public MainWindow()
        {
            vm = new LevelEditViewModel();
            InitImgMap();
            InitializeComponent();
            cellSize = 32;
            brush = '1';
        }
        private void InitImgMap()
        {
            imgMap = new Dictionary<char, BitmapImage>();
            string path = System.IO.Path.Combine(Environment.CurrentDirectory, "Resources", "space.bmp");
            var uri = new Uri(path);
            BitmapImage bmpToAdd = new BitmapImage(uri);
            imgMap.Add('0', bmpToAdd);
            path = System.IO.Path.Combine(Environment.CurrentDirectory, "Resources", "tile.bmp");
            uri = new Uri(path);
            bmpToAdd = new BitmapImage(uri);
            imgMap.Add('1', bmpToAdd);
        }


        private void OpenNewLevelDialog(object sender, RoutedEventArgs e)
        {
            NewLevelDialog win2 = new NewLevelDialog(vm, this);
            win2.Show();
        }
        public void RefreshTileMap()
        {
            int size = 32;
            for (int x = 0; x < vm.lvl.tileList.GetLength(0); x++)
            {
                for (int y = 0; y < vm.lvl.tileList.GetLength(1); y++)
                {
                    char type = vm.lvl.tileList[x, y].Id;
                    if (!imgMap.Keys.Contains(type))
                    {
                        string path = Model.DataProvider.GetTexturePath(type, vm.lvl.Name);
                        var uri = new Uri(path);
                        BitmapImage bmpToAdd = new BitmapImage(uri);
                    }
                    Image img = new Image();
                    BitmapImage bmp;
                    imgMap.TryGetValue(type, out bmp);
                    img.Source = bmp;
                    img.Height = size;
                    img.Width = size;
                    img.SetValue(Canvas.LeftProperty, (double)(x * size));
                    img.SetValue(Canvas.TopProperty, (double)(y * size));
                    img.MouseLeftButtonDown += img_MouseLeftButtonDown;
                    img.MouseRightButtonDown += img_MouseRightButtonDown;
                    img.MouseEnter += img_MouseEnter;
                    levelCanvas.Children.Add(img);
                }
            }
        }

        void img_MouseEnter(object sender, MouseEventArgs e)
        {
            if (Mouse.LeftButton == MouseButtonState.Pressed) img_MouseLeftButtonDown(sender, null);
            else if (Mouse.RightButton == MouseButtonState.Pressed) img_MouseRightButtonDown(sender, null);
        }
        void RefreshTile(Image tile, int x, int y)
        {
            tile.Source = imgMap[vm.lvl.GetTileType(x, y)];
        }
        void img_MouseRightButtonDown(object sender, MouseButtonEventArgs e)
        {
            Point pos = Mouse.GetPosition(levelCanvas);

            int x = (int)(pos.X / cellSize);
            int y = (int)(pos.Y / cellSize);

            if (x < 0 || x > vm.lvl.tileList.GetLength(0)) return;
            if (y < 0 || y > vm.lvl.tileList.GetLength(1)) return;

            if (vm.lvl.GetTileType(x, y) == '0') return;
            vm.lvl.SetTile(x, y, '0');
            RefreshTile((Image)sender, (int)(pos.X / cellSize), (int)(pos.Y / cellSize));
        }

        void img_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            Point pos = Mouse.GetPosition(levelCanvas);

            int x = (int)(pos.X / cellSize);
            int y = (int)(pos.Y / cellSize);
            if (vm.lvl.GetTileType(x, y) == brush) return;

            if (x < 0 || x > vm.lvl.tileList.GetLength(0)) return;
            if (y < 0 || y > vm.lvl.tileList.GetLength(1)) return;

            vm.lvl.SetTile(x, y, brush);
            RefreshTile((Image)sender, (int)(pos.X / cellSize), (int)(pos.Y / cellSize));
        }
        private void OpenOpenLevelDialog(object sender, RoutedEventArgs e)
        {
            OpenLevelDialog d = new OpenLevelDialog(vm, this);
            d.Show();
        }
        private void SaveCurrentLevel(object sender, RoutedEventArgs e)
        {
            if (vm.lvl != null)
                vm.SaveLevel();
        }

    }
}
