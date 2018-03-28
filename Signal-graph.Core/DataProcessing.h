#pragma once
#include "KeyPoint.h"
#include "Result.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Drawing;


namespace SignalgraphCore {

	Drawing::Rectangle rect = Drawing::Rectangle(135, 15, 1240, 490);
	double ratioX = (double)rect.Width / 35;
	double ratioY = (double)rect.Height / 2000;

	public ref class DataProcessing : System::Object
	{
	private:
		static DataProcessing ^ instance;

		DateTime dateTime;
		String ^code;
		List<KeyPoint^> ^keyPoints;
		List<Result^> ^resultTable;

	protected:
		DataProcessing();
		~DataProcessing();
		
	public:
		static DataProcessing^ GetInstance();
		void ResetInstance();

		bool SetDateTime(String^ _dateTime);
		DateTime GetDateTime();
		
		void SetCode(String^ _code);
		String^ GetCode();

		void ClearKeyPoints();
		bool AddKeyPoint(String^ _holdTime, String^ _area);
		void SortKeyPointsByHoldTime();
		void SortKeyPointsByArea();
		List<KeyPoint^> ^ GetKeyPoints();
		
		List<Result^> ^GetResult();

		void ClearGraph(Bitmap ^ graph);
		void DrawGrid(Bitmap ^ graph);
		void DrawGraph(Bitmap ^ graph);

		void DrawPickInterval(float &dx, float &x, float &y, float &xPrev, float &yPrev, float &maxX, float &maxY, int indexNextKeyPoint, Graphics^ g);
		void DrawBezier(double x0, double y0, double x1, double y1, Graphics^ g);

		static int ToPixelX(double x);
		static int ToPixelY(double y);
		static Point ToPixel(double x, double y);
	};
}

