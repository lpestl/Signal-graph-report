#include "stdafx.h"
#include <msclr/marshal_cppstd.h>
#include "DataProcessing.h"

using namespace System;
using namespace System::Windows;
using namespace System::Collections::Generic;

SignalgraphCore::DataProcessing::DataProcessing()
{
	code = String::Empty;
	keyPoints = gcnew List<KeyPoint^>();
}

SignalgraphCore::DataProcessing::~DataProcessing()
{	
	for (auto i = 0; i < keyPoints->Count; i++) {
		delete keyPoints[i];
	}
	keyPoints->Clear();
	delete keyPoints;
	delete code;
}

SignalgraphCore::DataProcessing ^ SignalgraphCore::DataProcessing::GetInstance()
{
	if (!instance) instance = gcnew SignalgraphCore::DataProcessing();
	return instance;
}

void SignalgraphCore::DataProcessing::ResetInstance()
{
	delete instance;
}

bool SignalgraphCore::DataProcessing::SetDateTime(String ^ _dateTime)
{
	if (DateTime::TryParse(_dateTime, dateTime)) {
		return true;
	}
	else {
		return false;
	}	
}

DateTime SignalgraphCore::DataProcessing::GetDateTime()
{
	return dateTime;
}

void SignalgraphCore::DataProcessing::SetCode(String ^ _code)
{
	code = _code;
}

String ^ SignalgraphCore::DataProcessing::GetCode()
{
	return code;
}

void SignalgraphCore::DataProcessing::ClearKeyPoints()
{
	keyPoints->Clear();
}

bool SignalgraphCore::DataProcessing::AddKeyPoint(String ^ _holdTime, String ^ _area)
{
	float hTime, ar;
	_holdTime = _holdTime->Replace('.', ',');
	if (!float::TryParse(_holdTime, hTime)) return false;
	_area = _area->Replace('.', ',');
	if (!float::TryParse(_area, ar)) return false;
	
	auto parts = _area->Split(',');
	long limit = Math::Pow(10, 7 - parts[parts->Length - 1]->Length);
	
	Random^ rnd = gcnew Random();
	auto remainder = rnd->Next(-limit / 2, limit / 2);
	double trueArea = (int)(ar * Math::Pow(10, 7)) + remainder;

	keyPoints->Add(gcnew KeyPoint(hTime, trueArea));
	return true;
}

void SignalgraphCore::DataProcessing::SortKeyPointsByHoldTime()
{
	throw gcnew System::NotImplementedException();
}

void SignalgraphCore::DataProcessing::SortKeyPointsByArea()
{
	throw gcnew System::NotImplementedException();
}

List<SignalgraphCore::KeyPoint^>^ SignalgraphCore::DataProcessing::GetKeyPoints()
{
	return keyPoints;
}

void SignalgraphCore::DataProcessing::DrawGrid(Bitmap ^ graph)
{
	Graphics ^g = Graphics::FromImage(graph);
}


