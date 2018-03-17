#pragma once
#include "BaseViewModel.h"
#include "ReportData.h"

using namespace System;

namespace SignalgraphCore {
	public ref class ReportDataViewModel : BaseViewModel
	{
	private:
		ReportData _reportData;
		String ^dateTime;
		String ^codeName;

	public:
		ReportDataViewModel();
		~ReportDataViewModel();

	};
}
