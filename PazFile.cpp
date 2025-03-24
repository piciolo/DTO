#include "PazFile.h"

namespace kukdh1 {

	FileInfo::FileInfo(uint8_t* buffer) {
		memcpy(&uiCRC, buffer + 0, 4);
		memcpy(&uiFolderID, buffer + 4, 4);
		memcpy(&uiFileID, buffer + 8, 4);
		memcpy(&uiOffset, buffer + 12, 4);
		memcpy(&uiCompressedSize, buffer + 16, 4);
		memcpy(&uiOriginalSize, buffer + 20, 4);
	}

	FileInfo::FileInfo() {
		// DO NOTHING
	}

	PazFile::PazFile(wchar_t* wpszPazFolder, uint32_t uiPazIndex, CryptICE& cipher) {
		std::wstring path(wpszPazFolder);
		std::wstringstream wss(path, std::ios_base::out | std::ios_base::in | std::ios_base::ate);

		wss << L"\\PAD" << std::setw(5) << std::setfill(L'0') << uiPazIndex << L".PAZ";
		path = wss.str();

		std::fstream file;
		file.open(path, std::ios::binary | std::ios::in);

		if (file.is_open()) {
			uint8_t buffer[64];
			uint32_t uiFileCount;
			uint32_t uiPathLength;

			// Read Header
			file.read((char*)buffer, 12);
			memcpy(&uiCRC, buffer + 0, 4);
			memcpy(&uiFileCount, buffer + 4, 4);
			memcpy(&uiPathLength, buffer + 8, 4);

			// Read file info
			uint8_t* infos = (uint8_t*)calloc(uiFileCount * 24, 1);
			file.read((char*)infos, uiFileCount * 24);

			for (uint32_t i = 0; i < uiFileCount; i++) {
				vFileInfo.push_back(FileInfo(infos + i * 24));
				vFileInfo.back().wsPazFullPath = path;
			}

			free(infos);

			// Read encrypted file names
			uint8_t* encrypted = (uint8_t*)calloc(uiPathLength, 1);
			uint8_t* decrypted = nullptr;

			file.read((char*)encrypted, uiPathLength);

			// Decrypt
			cipher.decrypt(encrypted, uiPathLength, &decrypted, &uiPathLength);

			// Parse
			std::vector<std::string> paths;
			char* ptr = (char*)decrypted;

			while (ptr < (char*)decrypted + uiPathLength) {
				paths.push_back(ptr);
				ptr += paths.back().length() + 1;
			}

			for (auto& info : vFileInfo) {
				info.sFullPath = paths.at(info.uiFolderID);
				info.sFullPath.append(paths.at(info.uiFileID));
			}

			free(encrypted);
			free(decrypted);
		}
	}

	// -----------------------------
	// Funzioni aggiuntive per PazAnalyzer
	// -----------------------------

	bool PazFile::Open(const char* path) {
		// In questa implementazione demo, ignora il nome e carica PAD00000.PAZ da "."
		CryptICE cipher(ICE_KEY, ICE_KEY_LEN);
		std::wstring folder = L".";
		*this = PazFile(const_cast<wchar_t*>(folder.c_str()), 0, cipher);
		return true;
	}

	int PazFile::GetFileCount() const {
		return static_cast<int>(vFileInfo.size());
	}

	const FileInfo& PazFile::GetEntry(int index) const {
		return vFileInfo.at(index);
	}

}