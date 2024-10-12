#pragma once
#ifndef ROBINLE_MOORHUHN_3_DATA_FILE_READER
#define ROBINLE_MOORHUHN_3_DATA_FILE_READER





#include <cstdint>
#include <memory>
#include <string>
#include <vector>



class Moorhuhn3VirtualFile
{
public: // methods

	Moorhuhn3VirtualFile(const char *szFilepath, size_t iDataSize, const uint8_t *pData);
	Moorhuhn3VirtualFile(const char *szFilepath, size_t iDataSize,
		std::unique_ptr<uint8_t[]> &&pData);

	Moorhuhn3VirtualFile() = default;
	Moorhuhn3VirtualFile(const Moorhuhn3VirtualFile &other) = default;
	Moorhuhn3VirtualFile(Moorhuhn3VirtualFile &&rval) = default;
	~Moorhuhn3VirtualFile() = default;

	Moorhuhn3VirtualFile &operator=(const Moorhuhn3VirtualFile &other) = default;
	Moorhuhn3VirtualFile &operator=(Moorhuhn3VirtualFile &&rval) = default;


	const auto &path() const { return m_sPath; }
	const auto datasize() const { return m_iDataSize; }
	const uint8_t *data() const { return m_upData.get(); }

	bool saveToFile(const wchar_t *szFilePath) const;


private: // variables

	std::string m_sPath;
	size_t m_iDataSize = 0;
	std::unique_ptr<uint8_t[]> m_upData;
};

class Moorhuhn3DataFile
{
public: // methods

	bool loadFile(const wchar_t *szFilepath);

	void clear() { m_oVFiles.clear(); };

	const auto &getFiles() const { return m_oVFiles; }


private: // variables

	std::vector<Moorhuhn3VirtualFile> m_oVFiles;

};





#endif // ROBINLE_MOORHUHN_3_DATA_FILE_READER