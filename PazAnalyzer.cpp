#include "PazAnalyzer.h"
#include "PazFile.h"
#include <fstream>
#include "json/json.h"

void kukdh1::PazAnalyzer::analyzeAndLog(const std::wstring& pazFilePath, const std::wstring& logFilePath) {
	std::string pazPathStr(pazFilePath.begin(), pazFilePath.end());
	kukdh1::PazFile paz;

	if (!paz.Open(pazPathStr.c_str())) {
		throw std::runtime_error("Impossibile aprire il file PAZ.");
	}

	Json::Value logRoot;
	logRoot["paz_file"] = pazPathStr;
	logRoot["entry_count"] = static_cast<Json::UInt64>(paz.GetFileCount());

	Json::Value entries(Json::arrayValue);

	for (int i = 0; i < paz.GetFileCount(); i++) {
		const kukdh1::FileInfo& entry = paz.GetEntry(i);

		Json::Value entryData;
		entryData["file_name"] = entry.sFullPath;
		entryData["compressed_size"] = static_cast<Json::UInt64>(entry.uiCompressedSize);
		entryData["uncompressed_size"] = static_cast<Json::UInt64>(entry.uiOriginalSize);

		std::string fileExt = "";
		size_t dotPos = entry.sFullPath.find_last_of(".");
		if (dotPos != std::string::npos && dotPos + 1 < entry.sFullPath.length()) {
			fileExt = entry.sFullPath.substr(dotPos + 1);
		}
		entryData["extension"] = fileExt;

		entries.append(entryData);
	}

	logRoot["entries"] = entries;

	std::ofstream logFile(logFilePath);
	if (!logFile.is_open()) {
		throw std::runtime_error("Impossibile aprire il file di log.");
	}

	Json::StreamWriterBuilder writer;
	writer["indentation"] = "  ";
	logFile << Json::writeString(writer, logRoot);
	logFile.close();
}