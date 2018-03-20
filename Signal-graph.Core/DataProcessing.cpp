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
	resultTable = gcnew List<Result^>();
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
	for (auto i = 0; i < keyPoints->Count; ++i) {
		int n_min = i;
		for (auto j = i; j < keyPoints->Count; ++j) {
			if (keyPoints[n_min]->GetHoldTime() > keyPoints[j]->GetHoldTime()) {
				n_min = j;
			}
		}
		KeyPoint^ temp = keyPoints[n_min];
		keyPoints[n_min] = keyPoints[i];
		keyPoints[i] = temp;
	}
}

void SignalgraphCore::DataProcessing::SortKeyPointsByArea()
{
	throw gcnew System::NotImplementedException();
}

List<SignalgraphCore::KeyPoint^>^ SignalgraphCore::DataProcessing::GetKeyPoints()
{
	return keyPoints;
}

void SignalgraphCore::DataProcessing::ClearGraph(Bitmap ^ graph)
{
	Graphics ^g = Graphics::FromImage(graph);

	g->Clear(Color::White);
}

void SignalgraphCore::DataProcessing::DrawGrid(Bitmap ^ graph)
{
	Graphics ^g = Graphics::FromImage(graph);

	// Draw grid rect
	Pen^ blackpen = gcnew Pen(Color::Black);
	SolidBrush^ whiteBrush = gcnew SolidBrush(Color::White);	
	
	g->FillRectangle(whiteBrush, Drawing::Rectangle(0, 0, graph->Width, rect.Y));
	g->FillRectangle(whiteBrush, Drawing::Rectangle(0, 0, rect.X, graph->Height)); 
	g->FillRectangle(whiteBrush, Drawing::Rectangle(0, rect.Y + rect.Height, graph->Width, graph->Height - (rect.Y + rect.Height)));
	g->FillRectangle(whiteBrush, Drawing::Rectangle(rect.X + rect.Width, 0, graph->Width - (rect.X + rect.Width), graph->Height));

	g->DrawRectangle(blackpen, rect);

	// Draw notches on the axis
	int bigNotchLen = 10;
	int smalNotchLen = 5;

	// Font and brash for text on notches
	Font^ mainFont = gcnew Font("Arial", 14);
	SolidBrush^ blackBrush = gcnew SolidBrush(Color::Black);
	
	// Set format of string.
	StringFormat^ lchFormat = gcnew StringFormat();
	lchFormat->Alignment = StringAlignment::Far;
	lchFormat->LineAlignment = StringAlignment::Center;

	StringFormat^ rchFormat = gcnew StringFormat();
	rchFormat->Alignment = StringAlignment::Near;
	rchFormat->LineAlignment = StringAlignment::Center;

	for (auto i = 0; i <= 20; ++i) {
		// Draw notches om left vertical line
		int y = Math::Round(rect.Y + (double)rect.Height / 20 * i);
		Point p1 = Point(rect.X, y), p3 = Point(rect.X + rect.Width, y);
		Point p2, p4;

		if (i % 5 == 0)
		{
			p2 = Point(rect.X - bigNotchLen, y);
			p4 = Point(rect.X + rect.Width + bigNotchLen, y);
		}
		else {
			p2 = Point(rect.X - smalNotchLen, y);
			p4 = Point(rect.X + rect.Width + smalNotchLen, y);
		}
		g->DrawLine(blackpen, p1, p2);
		g->DrawLine(blackpen, p3, p4);

		if (i % 10 == 0) {
			g->DrawString(((20 - i) * 100).ToString(), mainFont, blackBrush, Point(p2.X - smalNotchLen, y), lchFormat);
			g->DrawString(((20 - i) * 100).ToString(), mainFont, blackBrush, Point(p4.X + smalNotchLen, y), rchFormat);
		}
	}

	StringFormat^ cnhFormat = gcnew StringFormat();
	cnhFormat->Alignment = StringAlignment::Center;
	cnhFormat->LineAlignment = StringAlignment::Near;

	for (auto i = 0; i <= 35; ++i) {
		// Draw notches on bottom horizontal line
		int x = Math::Round(rect.X + (double)rect.Width / 35 * i);
		Point p1 = Point(x, rect.Y + rect.Height);
		Point p2;
		if (i % 5 == 0) p2 = Point(x, p1.Y + bigNotchLen);
		else p2 = Point(x, p1.Y + smalNotchLen);
		g->DrawLine(blackpen, p1, p2);

		if (i % 5 == 0) {
			g->DrawString(i.ToString(), mainFont, blackBrush, Point(p2.X, p2.Y + smalNotchLen), cnhFormat);
		}
	}

	// Draw Signatures to the axes
	StringFormat^ ccvFormat = gcnew StringFormat();
	ccvFormat->Alignment = StringAlignment::Center;
	ccvFormat->LineAlignment = StringAlignment::Center;

	g->RotateTransform(-90);
	g->DrawString("Volts", mainFont, blackBrush, Point((-1) * (rect.Y + rect.Height / 2), 1.5 * bigNotchLen), ccvFormat);
	g->DrawString("Volts", mainFont, blackBrush, Point((-1) * (rect.Y + rect.Height / 2), graph->Width - 1.5 * bigNotchLen), ccvFormat);
	g->ResetTransform();

	g->DrawString("Минут", mainFont, blackBrush, Point(rect.X + rect.Width / 2, graph->Height - 1.5 * bigNotchLen), ccvFormat);


	Font^ bigFont = gcnew Font("Arial", 18);
	StringFormat^ nnhFormat = gcnew StringFormat();
	nnhFormat->Alignment = StringAlignment::Near;
	nnhFormat->LineAlignment = StringAlignment::Far;

	g->DrawString("Время удерживания", bigFont, blackBrush, Point(rect.X + 50, rect.Y + 60), nnhFormat);

	Pen^ greenpen = gcnew Pen(Color::DarkGreen);
	SolidBrush^ grayBrush = gcnew SolidBrush(Color::LightGray);

	g->DrawLine(greenpen, Point(rect.X + 50, rect.Y + 15), Point(rect.X + 100, rect.Y + 15));

	Font^ boldMainFont = gcnew Font("Arial", 14, FontStyle::Bold);
	StringFormat^ nchFormat = gcnew StringFormat();
	nchFormat->Alignment = StringAlignment::Near;
	nchFormat->LineAlignment = StringAlignment::Center;

	g->DrawString("Ближний сигнал", boldMainFont, grayBrush, Point(rect.X + 115, rect.Y + 15), nchFormat);
}

void SignalgraphCore::DataProcessing::DrawGraph(Bitmap ^ graph)
{
	Graphics ^g = Graphics::FromImage(graph);

	Pen^ greenPen = gcnew Pen(Color::DarkGreen);
	Pen^ redPen = gcnew Pen(Color::Red);
	Pen^ bluePen = gcnew Pen(Color::Blue);

	Random^ rnd = gcnew Random();

	double ratioX = (double)rect.Width / 35;
	double ratioY = (double)rect.Height / 2000;

	// Main line graph params ( y(x) = -1/z * (x-mX)^2 + mY
	float z = 12 + 20 * rnd->NextDouble(); // rnd->Next(12, 32);
	float mx = 15 + 10 * rnd->NextDouble(); // rnd->Next(15, 25);
	float my = 260 + 40 * rnd->NextDouble(); // rnd->Next(260, 300);

	float offsetX = 0;
	float offsetY = (float)(-1) / z * mx * mx + my;
	Point pStart = Point(rect.X + offsetX * ratioX, rect.Y + rect.Height - offsetY * ratioY);

	while (offsetX <= 35) {		
		float deltaOffsetX = (float)35 / (60 + rnd->NextDouble() * 20);
		
		offsetX += deltaOffsetX;
		offsetY = (float)(-1) / z * (offsetX - mx) * (offsetX - mx) + my;
		Point pEnd = Point(rect.X + offsetX * ratioX, rect.Y + rect.Height - offsetY * ratioY);

		g->DrawLine(greenPen, pStart, pEnd);

		pStart = pEnd;
	}

}


