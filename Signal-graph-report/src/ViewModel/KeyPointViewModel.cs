using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Signal_graph_report.src.ViewModel
{
    public class KeyPointViewModel : BaseViewModel
    {
        private string holdTime;
        private string area;

        public string HoldTime
        {
            get { return holdTime; }
            set
            {
                holdTime = value;
                OnPropertyChanged("HoldTime");
            }
        }

        public string Area
        {
            get { return area; }
            set
            {
                area = value;
                OnPropertyChanged("Area");
            }
        }
    }
}
