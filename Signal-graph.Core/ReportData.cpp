#include "stdafx.h"

#include "ReportData.h"

SignalgraphCore::ReportData::ReportData()
{
	dateTime = gcnew DateTime();
	codeName = gcnew String("XXX-XXX");
}

SignalgraphCore::ReportData::~ReportData()
{
	delete codeName;
	delete dateTime;
}
