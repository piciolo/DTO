#pragma once

#include <string>

namespace kukdh1 {
	class PazAnalyzer {
	public:
		static void analyzeAndLog(const std::wstring& pazFilePath, const std::wstring& logFilePath);
	};
}