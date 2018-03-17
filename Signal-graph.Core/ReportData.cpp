#include "stdafx.h"

#include "ReportData.h"

SignalgraphCore::ReportData::ReportData()
{
	dateTime = gcnew DateTime();
	dateTime = DateTime::Now;
	codeName = gcnew String("XXX-XXX");
}

SignalgraphCore::ReportData::~ReportData()
{
	if (!codeName) delete codeName;
	if (!dateTime) delete dateTime;
}
