#pragma once

using namespace System;

namespace SignalgraphCore {
	public ref class KeyPoint
	{
	public:
		KeyPoint(float _holdTime, double _area) :
			holdTime(_holdTime),
			area(_area) 
		{}
		
		float GetHoldTime() { return holdTime; }
		void SetHoldTime(float _holdTime) { holdTime = _holdTime; }

		double GetArea() { return area; }
		void SetArea(double _area) { area = _area; }

	private:
		float holdTime;
		double area;
	};
}

