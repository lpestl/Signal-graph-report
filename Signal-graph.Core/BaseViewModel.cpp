#include "stdafx.h"
#include "BaseViewModel.h"

void SignalgraphCore::BaseViewModel::OnPropertyChanged(String ^ info)
{
	PropertyChanged(this, gcnew PropertyChangedEventArgs(info));
}
