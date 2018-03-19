using SignalgraphCore;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace Signal_graph_report.src.ViewModel
{
    public class ReportDataViewModel : BaseViewModel
    {
        private string date;
        private string time;
        private string code;

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

                        if (dataProcessing.SetDateTime(date + " " + time))
                        {
                            DateTime dt = dataProcessing.GetDateTime();
                            MessageBox.Show(dt.ToString("dd.MM.yyyy hh:mm"), "Date Time", MessageBoxButton.OK, MessageBoxImage.Information);
                        } else
                        {
                            MessageBox.Show("Date or Time not can parse!", "Error date or time parse", MessageBoxButton.OK, MessageBoxImage.Error);
                            return;
                        }

                        dataProcessing.ClearKeyPoints();
                        if (dataProcessing.AddKeyPoint(keyPoints[0].HoldTime, keyPoints[0].Area))
                        {
                            List<SignalgraphCore.KeyPoint> points = dataProcessing.GetKeyPoints();

                            MessageBox.Show(points[0].GetHoldTime().ToString() + " " + points[0].GetArea().ToString(), "Tadam!!!", MessageBoxButton.OK, MessageBoxImage.Information);
                        } else
                        {
                            MessageBox.Show("WTF. KeyPoints not parsed!", "Error key points", MessageBoxButton.OK, MessageBoxImage.Error);
                            return;
                        }
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

    }
}
