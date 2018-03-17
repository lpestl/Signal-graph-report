#include "stdafx.h"
#include "ReportDataViewModel.h"


SignalgraphCore::ReportDataViewModel::ReportDataViewModel()
{
	dateTime = gcnew String(DateTime::Now.ToString());
	codeName = gcnew String("XXX-XXX");
}

SignalgraphCore::ReportDataViewModel::~ReportDataViewModel()
{
	if (!codeName) delete codeName;
	if (!dateTime) delete dateTime;
}
