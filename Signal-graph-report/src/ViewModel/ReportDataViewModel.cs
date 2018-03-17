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

        /// <summary>
        /// Temporary command
        /// </summary>
        private RelayCommand messageBoxCommand;
        public RelayCommand MessageBoxCommand
        {
            get
            {
                return messageBoxCommand ??
                    (messageBoxCommand = new RelayCommand(obj =>
                    {
                        String msg = KeyPoints[0].HoldTime + " : " + KeyPoints[0].Area + "\n";
                        msg += KeyPoints[1].HoldTime + " : " + KeyPoints[1].Area + "\n";
                        msg += KeyPoints[2].HoldTime + " : " + KeyPoints[2].Area + "\n";
                        msg += KeyPoints[3].HoldTime + " : " + KeyPoints[3].Area + "\n";

                        MessageBox.Show(msg, "Current DATA", MessageBoxButton.OK, MessageBoxImage.Information);
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
