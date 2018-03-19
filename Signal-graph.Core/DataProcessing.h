#pragma once
#include "KeyPoint.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Drawing;


namespace SignalgraphCore {
	public ref class DataProcessing : System::Object
	{
	private:
		static DataProcessing ^ instance;

		DateTime dateTime;
		String ^code;
		List<KeyPoint^> ^keyPoints;

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

		void DrawGrid(Bitmap ^ graph);
	};
}

