using SignalgraphCore;
using System;
using System.Drawing;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media.Imaging;
using System.IO;

namespace Signal_graph_report.src.ViewModel
{
    public class ReportDataViewModel : BaseViewModel
    {
        private string date;
        private string time;
        private string code;
        private Bitmap graph;

        private ObservableCollection<KeyPointViewModel> keyPoints;
        public ObservableCollection<KeyPointViewModel> KeyPoints {
            get { return keyPoints; }
            set { keyPoints = value; }
        }

        public ReportDataViewModel()
        {
            keyPoints = new ObservableCollection<KeyPointViewModel>()
            {
                new KeyPointViewModel { HoldTime = "", Area = "" },
                new KeyPointViewModel { HoldTime = "", Area = "" },
                new KeyPointViewModel { HoldTime = "", Area = "" },
                new KeyPointViewModel { HoldTime = "", Area = "" }
            };

            graph = new Bitmap(1501, 578);
            
            DataProcessing dataProcessing = DataProcessing.GetInstance();
            dataProcessing.ClearGraph(graph);
            dataProcessing.DrawGrid(graph);
        }

        private RelayCommand drawCommand;
        public RelayCommand DrawCommand
        {
            get
            {
                return drawCommand ??
                    (drawCommand = new RelayCommand(obj =>
                    {
                        DataProcessing dataProcessing = DataProcessing.GetInstance();
                        dataProcessing.ClearGraph(graph);
                        dataProcessing.DrawGraph(graph);
                        dataProcessing.DrawGrid(graph);

                        OnPropertyChanged("GraphSource");
                    }));
            }
        }

        private RelayCommand clearCommand;
        public RelayCommand CleatCommand
        {
            get
            {
                return clearCommand ??
                    (clearCommand = new RelayCommand(obj =>
                    {
                        DataProcessing dataProcessing = DataProcessing.GetInstance();
                        dataProcessing.ClearGraph(graph);
                        dataProcessing.DrawGrid(graph);

                        OnPropertyChanged("GraphSource");
                    }));
            }
        }

        private RelayCommand savePicCommand;
        public RelayCommand SavePicCommand
        {
            get
            {
                return savePicCommand ??
                    (savePicCommand = new RelayCommand(obj =>
                    {

                    }));
            }
        }

        private RelayCommand saveDocCommand;
        public RelayCommand SaveDocCommand
        {
            get
            {
                return saveDocCommand ??
                    (saveDocCommand = new RelayCommand(obj =>
                    {

                    }));
            }
        }        

        public string Date
        {
            get { return date; }
            set
            {
                date = value;
                OnPropertyChanged("Date");
            }
        }

        public string Time
        {
            get { return time; }
            set
            {
                time = value;
                OnPropertyChanged("Time");
            }
        }

        public string Code
        {
            get { return code; }
            set
            {
                code = value;
                OnPropertyChanged("Code");
            }
        }

        public BitmapImage GraphSource
        {
            get
            {
                using (MemoryStream memory = new MemoryStream())
                {
                    graph.Save(memory, System.Drawing.Imaging.ImageFormat.Bmp);
                    memory.Position = 0;
                    BitmapImage bitmapimage = new BitmapImage();
                    bitmapimage.BeginInit();
                    bitmapimage.StreamSource = memory;
                    bitmapimage.CacheOption = BitmapCacheOption.OnLoad;
                    bitmapimage.EndInit();

                    return bitmapimage;
                }
            }
        }
    }
}
