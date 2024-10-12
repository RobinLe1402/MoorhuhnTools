#pragma once
#ifndef ROBINLE_MOORHUHN_JNR_DATA_FILE_READER
#define ROBINLE_MOORHUHN_JNR_DATA_FILE_READER





#include <cstdint>
#include <memory>
#include <string>
#include <vector>



class MoorhuhnJnRVirtualFile
{
public: // methods

	MoorhuhnJnRVirtualFile(const char *szFilepath, size_t iDataSize, const uint8_t *pData);
	MoorhuhnJnRVirtualFile(const char *szFilepath, size_t iDataSize,
		std::unique_ptr<uint8_t[]> &&pData);

	MoorhuhnJnRVirtualFile() = default;
	MoorhuhnJnRVirtualFile(const MoorhuhnJnRVirtualFile &other) = default;
	MoorhuhnJnRVirtualFile(MoorhuhnJnRVirtualFile &&rval) = default;
	~MoorhuhnJnRVirtualFile() = default;

	MoorhuhnJnRVirtualFile &operator=(const MoorhuhnJnRVirtualFile &other) = default;
	MoorhuhnJnRVirtualFile &operator=(MoorhuhnJnRVirtualFile &&rval) = default;


	const auto &path() const { return m_sPath; }
	const auto datasize() const { return m_iDataSize; }
	const uint8_t *data() const { return m_upData.get(); }

	bool saveToFile(const wchar_t *szFilePath) const;


private: // variables

	std::string m_sPath;
	size_t m_iDataSize = 0;
	std::unique_ptr<uint8_t[]> m_upData;
};

class MoorhuhnJnRDataFile
{
public: // methods

	bool loadFile(const wchar_t *szFilepath);

	void clear() { m_oVFiles.clear(); };

	const auto &getFiles() const { return m_oVFiles; }


private: // variables

	std::vector<MoorhuhnJnRVirtualFile> m_oVFiles;

};





#endif // ROBINLE_MOORHUHN_JNR_DATA_FILE_READER