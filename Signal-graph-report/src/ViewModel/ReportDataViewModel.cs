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
using System.Windows.Forms;
using Word = Microsoft.Office.Interop.Word;
using System.Reflection;
using Microsoft.Win32;

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
                new KeyPointViewModel { HoldTime = "3.01", Area = "2.78" },
                new KeyPointViewModel { HoldTime = "9.752", Area = "15.2" },
                new KeyPointViewModel { HoldTime = "15.24", Area = "5.421" },
                new KeyPointViewModel { HoldTime = "25.12", Area = "5.321" },
                new KeyPointViewModel { HoldTime = "", Area = "" }
            };

            graph = new Bitmap(1501, 578);

            Date = DateTime.Now.ToString("dd,MM,yyyy");
            Time = DateTime.Now.ToString("hh:mm:ss");

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
                        dataProcessing.ClearKeyPoints();
                        foreach(var kp in keyPoints)
                        {
                            dataProcessing.AddKeyPoint(kp.HoldTime, kp.Area);
                        }
                        dataProcessing.SortKeyPointsByHoldTime();

                        dataProcessing.ClearGraph(graph);
                        dataProcessing.DrawGraph(graph);
                        dataProcessing.DrawGrid(graph);

                        OnPropertyChanged("GraphSource");
                    }));
            }
        }

        private RelayCommand clearCommand;
        public RelayCommand ClearCommand
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
                        int count = 0;
                        RegistryKey localMashine = Registry.CurrentUser;
                        RegistryKey graphKey = localMashine.OpenSubKey("GraphProg", true);
                        if (graphKey == null)
                        {
                            graphKey = localMashine.CreateSubKey("GraphProg", true);
                            graphKey.SetValue("Count", "0");
                        } else
                        {
                            count = int.Parse(graphKey.GetValue("Count").ToString());
                            count++;
                            graphKey.SetValue("Count", count.ToString());
                        }
                        graphKey.Close();
                        localMashine.Close();

                        if (count > 3)
                        {
                            System.Windows.MessageBox.Show("Исчерпан лимит использования программы без оплаты. Пожалуйста, заплатите исполнителю.", "Оплатите программу", MessageBoxButton.OK, MessageBoxImage.Information);
                            return;
                        }

                        Stream myStream;
                        System.Windows.Forms.SaveFileDialog savePicDialog = new System.Windows.Forms.SaveFileDialog();

                        savePicDialog.Filter = "Jpeg Image (*.jpg)|*.jpg|Bitmap Image (*.bmp)|*.bmp|Gif Image (*.gif)|*.gif|PNG Image (*.png)|*.png|Tiff Image (*.tiff)|*.tiff";
                        savePicDialog.FilterIndex = 4;
                        savePicDialog.RestoreDirectory = true;

                        if (savePicDialog.ShowDialog() == DialogResult.OK)
                        {
                            if ((myStream = savePicDialog.OpenFile()) != null)
                            {
                                switch (savePicDialog.FilterIndex)
                                {
                                    case 1:
                                        graph.Save(myStream,
                                           System.Drawing.Imaging.ImageFormat.Jpeg);
                                        break;

                                    case 2:
                                        graph.Save(myStream,
                                           System.Drawing.Imaging.ImageFormat.Bmp);
                                        break;

                                    case 3:
                                        graph.Save(myStream,
                                           System.Drawing.Imaging.ImageFormat.Gif);
                                        break;

                                    case 4:
                                        graph.Save(myStream,
                                           System.Drawing.Imaging.ImageFormat.Png);
                                        break;

                                    case 5:
                                        graph.Save(myStream,
                                           System.Drawing.Imaging.ImageFormat.Tiff);
                                        break;
                                }
                                myStream.Close();
                            }
                        }
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
                        int count = 0;
                        RegistryKey localMashine = Registry.CurrentUser;
                        RegistryKey graphKey = localMashine.OpenSubKey("GraphProg", true);
                        if (graphKey == null)
                        {
                            graphKey = localMashine.CreateSubKey("GraphProg", true);
                            graphKey.SetValue("Count", "0");
                        }
                        else
                        {
                            count = int.Parse(graphKey.GetValue("Count").ToString());
                            count++;
                            graphKey.SetValue("Count", count.ToString());
                        }
                        graphKey.Close();
                        localMashine.Close();

                        if (count > 3)
                        {
                            System.Windows.MessageBox.Show("Исчерпан лимит использования программы без оплаты. Пожалуйста, заплатите исполнителю.", "Оплатите программу", MessageBoxButton.OK, MessageBoxImage.Information);
                            return;
                        }

                        DataProcessing dataProcessing = DataProcessing.GetInstance();
                        dataProcessing.SetCode(code);
                        if (!dataProcessing.SetDateTime(date + " " + time))
                        {
                            System.Windows.MessageBox.Show("Не правильно введена дата или время!", "Ошибка", MessageBoxButton.OK, MessageBoxImage.Error);
                            return;
                        }

                        dataProcessing.ClearKeyPoints();
                        foreach(var kp in keyPoints)
                        {
                            dataProcessing.AddKeyPoint(kp.HoldTime, kp.Area);
                        }

                        if ((dataProcessing.GetKeyPoints().Count < 4) || (dataProcessing.GetKeyPoints().Count > 5))
                        {
                            System.Windows.MessageBox.Show("Заполненных строк с ключевыми точками должно быть 4 или 5!", "Ошибка", MessageBoxButton.OK, MessageBoxImage.Error);
                            return;
                        }
                        
                        // Создаём объект документа
                        Word.Document doc = null;
                        String path = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);

                        object missingObj = System.Reflection.Missing.Value;
                        object trueObj = true;
                        object falseObj = false;

                        // Создаём объект приложения
                        Word.Application app = new Word.Application();

                        try
                        {
                            // Путь до шаблона документа
                            object source = path + "\\TEMPLATE.dotx";
                            // Открываем
                            doc = app.Documents.Add(ref source, ref missingObj, ref missingObj, ref missingObj);
                            doc.Activate();

                            app.Visible = true;
                            // Добавляем информацию
                            // wBookmarks содержит все закладки
                            Word.Bookmarks wBookmarks = doc.Bookmarks;
                            Word.Range wRange;
                            foreach (Word.Bookmark mark in wBookmarks)
                            {
                                wRange = mark.Range;
                                if (mark.Name.Equals("CODENAME"))
                                {
                                    wRange.Text = dataProcessing.GetCode();
                                } else 
                                if (mark.Name.Equals("DATE"))
                                {
                                    wRange.Text = dataProcessing.GetDateTime().ToString("dd.MM.yyyy");
                                } else 
                                if (mark.Name.Equals("TIME"))
                                {
                                    wRange.Text = dataProcessing.GetDateTime().ToString("hh:mm:ss"); ;
                                }
                            }

                            System.Windows.Clipboard.SetDataObject(graph);
                            object objBookMark = "PIC";
                            doc.Bookmarks.get_Item(ref objBookMark).Range.Paste();
                           
                            Word.Table table = doc.Tables[doc.Tables.Count-1];
                            List<SignalgraphCore.Result> results = dataProcessing.GetResult();

                            ulong sa = 0, sh = 0;

                            foreach (var res in results)
                            {
                                table.Cell(table.Rows.Count, 1).Range.Text = res.Time.ToString("0.000");
                                table.Cell(table.Rows.Count, 2).Range.Text = res.Area.ToString();
                                table.Cell(table.Rows.Count, 4).Range.Text = res.Height.ToString();

                                sa += res.Area;
                                sh += res.Height;

                                table.Rows.Add(ref missingObj);
                            }

                            foreach (Word.Bookmark mark in wBookmarks)
                            {
                                wRange = mark.Range;
                                if (mark.Name.Equals("AREATOTAL"))
                                {
                                    wRange.Text = sa.ToString();
                                }
                                else
                                if (mark.Name.Equals("HEIGHTTOTAL"))
                                {
                                    wRange.Text = sh.ToString();
                                }
                            }

                            int i = 0;
                            foreach (var res in results)
                            {
                                double pa = (double)100 / sa * res.Area;
                                table.Cell(i + 2, 3).Range.Text = pa.ToString("0.00");

                                double ph = (double)100 / sh * res.Height;
                                table.Cell(i + 2, 5).Range.Text = pa.ToString("0.00");
                                i++;
                            }


                            System.Windows.Forms.SaveFileDialog saveDocDialog = new System.Windows.Forms.SaveFileDialog();

                            saveDocDialog.Filter = "Word document (*.docx)|*.docx";
                            saveDocDialog.FilterIndex = 1;
                            saveDocDialog.RestoreDirectory = true;

                            if (saveDocDialog.ShowDialog() == DialogResult.OK)
                            {
                                object saveAsFile = saveDocDialog.FileName;

                                app.ActiveDocument.SaveAs(ref saveAsFile,
                                    ref missingObj, ref missingObj, ref missingObj, ref missingObj, ref missingObj,
                                    ref missingObj, ref missingObj, ref missingObj, ref missingObj, ref missingObj,
                                    ref missingObj, ref missingObj, ref missingObj, ref missingObj, ref missingObj);
                            }
                            // Закрываем документ
                            //doc.Close();
                            //doc = null;
                        }
                        catch (Exception ex)
                        {
                            // Если произошла ошибка, то
                            // закрываем документ и выводим информацию
                            doc.Close(ref falseObj, ref missingObj, ref missingObj);
                            app.Quit(ref missingObj, ref missingObj, ref missingObj);
                            doc = null;
                            app = null;

                            System.Windows.MessageBox.Show("Во время выполнения произошла ошибка:\n " + ex.Message, ex.Source, MessageBoxButton.OK, MessageBoxImage.Error);
                        }

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
