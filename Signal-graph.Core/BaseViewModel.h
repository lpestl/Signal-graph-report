#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Runtime::CompilerServices;

namespace SignalgraphCore {
	public ref class BaseViewModel : INotifyPropertyChanged
	{
	public:
		virtual event PropertyChangedEventHandler^ PropertyChanged;
		void OnPropertyChanged(String^ info);
	};
}
