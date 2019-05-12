#include "stdafx.h"
#include <msclr/marshal_cppstd.h>
#include "DataProcessing.h"
#include "Result.h"

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

List<SignalgraphCore::Result^> ^SignalgraphCore::DataProcessing::GetResult()
{
	return resultTable;
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
	resultTable->Clear();

	Graphics ^g = Graphics::FromImage(graph);

	Pen^ greenPen = gcnew Pen(Color::DarkGreen);
	Pen^ redPen = gcnew Pen(Color::Red);
	Pen^ bluePen = gcnew Pen(Color::Blue);

	// Font and brash for text on notches
	Font^ mainFont = gcnew Font("Arial", 18);
	SolidBrush^ blackBrush = gcnew SolidBrush(Color::Black);
	// Set format of string.
	StringFormat^ mainFormat = gcnew StringFormat();
	mainFormat->Alignment = StringAlignment::Near;
	mainFormat->LineAlignment = StringAlignment::Center;

	Random^ rnd = gcnew Random();
	
	// Main line graph params ( y(x) = -1/z * (x-mX)^2 + mY
	float z = 20;
	float mx = 25;
	float my = 230 + 20 * rnd->NextDouble();

	float offsetX = 0;
	float offsetY = (float)(-1) / z * mx * mx + my;

	Point pStart = ToPixel(offsetX, offsetY);

	int projectionLen = 25;
	float smaller = 900;
	
	while (offsetX <= 35) {		
		float dX;
		
		float lastOffsetX = offsetX;
		float lastOffsetY = offsetY;

		float maxX = offsetX;
		float maxY = offsetY;

		if (offsetX < keyPoints[0]->GetHoldTime()) {
			dX = (float)35 / (60 + rnd->NextDouble() * 20);

			offsetX += dX;
			offsetY = (float)(-1) / z * (offsetX - mx) * (offsetX - mx) + my;

			if (offsetX >= keyPoints[0]->GetHoldTime()) {
				offsetY = (900 + 600 * rnd->NextDouble()) / 2;
				DrawPickInterval(dX, offsetX, offsetY, lastOffsetX, lastOffsetY, maxX, maxY, 0, g);
			}
		}
		else if (offsetX < keyPoints[1]->GetHoldTime()) {
			dX = 0.05f + 0.3f * rnd->NextDouble();
			
			offsetX += dX;
			offsetY = 900 + 600 * rnd->NextDouble();			

			DrawPickInterval(dX, offsetX, offsetY, lastOffsetX, lastOffsetY, maxX, maxY, 1, g);
		}
		else {
			if (offsetX < keyPoints[keyPoints->Count - 1]->GetHoldTime()) {
			//if (((keyPoints->Count == 4) && (offsetX < keyPoints[3]->GetHoldTime()))
			//	|| (((keyPoints->Count == 5) && (offsetX < keyPoints[4]->GetHoldTime())))) {
				dX = 0.2f + 0.3f * rnd->NextDouble();

				offsetX += dX;
				offsetY = (float)(-1) / z * (offsetX - mx) * (offsetX - mx) + my + smaller * rnd->NextDouble();

				DrawPickInterval(dX, offsetX, offsetY, lastOffsetX, lastOffsetY, maxX, maxY, keyPoints->Count - 1, g);

				smaller = smaller / 8 * 7;
			}
			else {
				dX = 0.3f + 0.9f * rnd->NextDouble();

				offsetX += dX;
				offsetY = (float)(-1) / z * (lastOffsetX - mx) * (lastOffsetX - mx) + my;

				DrawPickInterval(dX, offsetX, offsetY, lastOffsetX, lastOffsetY, maxX, maxY, -1, g);
			}
		}		
		
		Point pEnd = ToPixel(offsetX, offsetY);

		auto mainLineY1 = (float)(-1) / z * (lastOffsetX - mx) * (lastOffsetX - mx) + my;
		mainLineY1 = ToPixelY(mainLineY1);
		auto mainLineY2 = (float)(-1) / z * (offsetX - mx) * (offsetX - mx) + my;
		mainLineY2 = ToPixelY(mainLineY2);
		auto maxLineY = (float)(-1) / z * (maxX - mx) * (maxX - mx) + my;

		if (ToPixelY(maxLineY) != ToPixelY(maxY)) {
			g->DrawLine(redPen, pStart, Point(pStart.X, pStart.Y + projectionLen));
			g->DrawLine(bluePen, pEnd, Point(pEnd.X, pEnd.Y - projectionLen));
			g->DrawLine(redPen, Point(pStart.X, mainLineY1), Point(pEnd.X, mainLineY2));
			g->DrawLine(redPen, pStart, Point(pStart.X, mainLineY1));
			g->DrawLine(redPen, pEnd, Point(pEnd.X, mainLineY2));
			
			if (maxY < 1600) g->TranslateTransform(ToPixelX(maxX), ToPixelY(maxY));
			else g->TranslateTransform(ToPixelX(maxX), ToPixelY(0));
			g->RotateTransform(-90);
			g->DrawString(maxX.ToString("0.000"), mainFont, blackBrush, Point(projectionLen, 0), mainFormat);
			g->ResetTransform();
		}
		else {
			g->DrawLine(greenPen, pStart, pEnd);
		}

		pStart = pEnd;
	}

}

void SignalgraphCore::DataProcessing::DrawPickInterval(float &dx, float & x, float & y, float & xPrev, float & yPrev, float & maxX, float & maxY, int indexNextKeyPoint, Graphics^ g)
{
	Result^ res = gcnew Result();
	Random^ rnd = gcnew Random();

	bool isKeyPoint = false;
	for (auto i = 0; i < keyPoints->Count; ++i) {
		if ((x >= keyPoints[i]->GetHoldTime()) && (xPrev < keyPoints[i]->GetHoldTime())) {
			res->Time = keyPoints[i]->GetHoldTime();
			res->Area = keyPoints[i]->GetArea();
			isKeyPoint = true;
			break;
		}
	}

	if (!isKeyPoint) {
		res->Time = dx / 3 * rnd->NextDouble() + xPrev + dx / 3;

		if (indexNextKeyPoint == 1) 
			res->Area = rnd->Next(10000000, 500000000);
		else if ((indexNextKeyPoint >= 0) && (indexNextKeyPoint < keyPoints->Count)) 
			res->Area = rnd->Next(  100000,   3000000);
		else if (indexNextKeyPoint == -1)
			res->Area = rnd->Next(    1000,  20000000);
	}

	maxX = res->Time;

	if (indexNextKeyPoint == 1) {
		res->Height = (double)res->Area * 0.141f /*- 4644900*/;
		maxY = max(yPrev, y) + Math::Abs((double)res->Height / 13500000 * 2000) * rnd->NextDouble() / 2;
	}
	else if ((indexNextKeyPoint >= 0) && (indexNextKeyPoint < keyPoints->Count)) {
		res->Height = (double)res->Area * 0.0787f /*- 155040*/;
		maxY = max(yPrev, y) + Math::Abs((double)res->Height / 13500000 * 2000) * rnd->NextDouble();
	}
	else if (indexNextKeyPoint == -1) {
		res->Height = (double)res->Area * 0.0455 + 14769.6971;
		maxY = max(yPrev, y) + 20 * rnd->NextDouble();
	}

	DrawBezier(xPrev, yPrev, maxX, maxY, g);
	DrawBezier(maxX, maxY, x, y, g);

	resultTable->Add(res);
}

void SignalgraphCore::DataProcessing::DrawBezier(double x0, double y0, double x1, double y1, Graphics^ g)
{
	Random^ rnd = gcnew Random();
	double px0, py0, px1, py1;

	if (y1 > y0) {
		px0 = x0 + rnd->NextDouble() * (x1 - x0);
		py0 = y0;
		px1 = x1 - (x1 - px0) / 3 * rnd->NextDouble();
		py1 = y1;
	}
	else {
		py0 = y0;
		px1 = x1 - rnd->NextDouble() * (x1 - x0);
		py1 = y1;
		px0 = x0 + (px1 - x0) / 3 * rnd->NextDouble();
	}
	Pen^ greenPen = gcnew Pen(Color::DarkGreen);
	
	auto p0 = ToPixel(x0, y0);
	auto p1 = ToPixel(px0, py0);
	auto p2 = ToPixel(px1, py1);
	auto p3 = ToPixel(x1, y1);
	g->DrawBezier(greenPen, p0, p1, p2, p3);
}

int SignalgraphCore::DataProcessing::ToPixelX(double x)
{
	return Math::Round(rect.X + x * ratioX);
}

int SignalgraphCore::DataProcessing::ToPixelY(double y)
{
	return Math::Round(rect.Y + rect.Height - y * ratioY);
}

Point SignalgraphCore::DataProcessing::ToPixel(double x, double y)
{
	return Point(ToPixelX(x), ToPixelY(y));
}
