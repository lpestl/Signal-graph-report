#pragma once

using namespace System;

namespace SignalgraphCore {
	public ref class Result
	{
	public:
		Result() :
			Time(-1),
			Area(0),
			PercentArea(0),
			Height(0),
			PercentHeight(0) {}

		float Time;
		unsigned long long Area;
		float PercentArea;
		unsigned long long Height;
		float PercentHeight;
	};
}
