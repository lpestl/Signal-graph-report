#pragma once

using namespace System;

namespace SignalgraphCore {
	public ref class ReportData
	{
	public:
		DateTime ^dateTime;
		String ^codeName;

		ReportData();
		~ReportData();
	};
}
