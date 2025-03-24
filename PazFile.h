// PazFile.h
#pragma once

#ifndef _PAZ_FILE_H_
#define _PAZ_FILE_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>

#include "Crypt.h"

namespace kukdh1 {
	struct FileInfo {
		uint32_t uiCRC;
		uint32_t uiFolderID;
		uint32_t uiFileID;
		uint32_t uiOffset;
		uint32_t uiCompressedSize;
		uint32_t uiOriginalSize;

		std::string sFullPath;
    std::wstring wsPazFullPath;

		FileInfo(uint8_t *buffer);
    FileInfo();
	};

	struct PazFile {
		uint32_t uiCRC;
		std::vector<FileInfo> vFileInfo;
	
		PazFile() {}
		PazFile(wchar_t *wpszPazFolder, uint32_t uiPazIndex, CryptICE &cipher);

    // Metodi per compatibilità con PazAnalyzer
    bool Open(const char* path);
    int GetFileCount() const;
    const FileInfo& GetEntry(int index) const;
	};
}

#endif