#include "MH3_DAT.hpp"

#include <fstream>


namespace
{
	constexpr char szMagicNumber[] = "MH3 V1.0 ";
	constexpr char szTerminatingFilename[] = "****";



#pragma pack(push, 1)

	typedef struct
	{
		char     szMagicNumber[48]; // "MH3 V1.0 "
		uint32_t iDataOffset;
		char     cUnknown[12];
	} MH3FileHdr;

	typedef struct
	{
		char     szFilepath[48];
		uint32_t iOffset;
		uint32_t iSize;
		char     cUnused[8];
	} MH3File;

#pragma pack(pop)



	inline void ReadBytes(std::ifstream &stream, void *pDest, size_t iLen)
	{
		stream.read(reinterpret_cast<char *>(pDest), iLen);
	}

}


Moorhuhn3VirtualFile::Moorhuhn3VirtualFile(
	const char *szFilepath, size_t iDataSize, const uint8_t *pData) :
	m_sPath(szFilepath), m_iDataSize(iDataSize), m_upData(std::make_unique<uint8_t[]>(iDataSize))
{
	if (iDataSize == 0)
		throw std::exception("Moorhuhn3VirtualFile: A size of 0 is invalid.");
}

Moorhuhn3VirtualFile::Moorhuhn3VirtualFile(const char *szFilepath, size_t iDataSize,
	std::unique_ptr<uint8_t[]> &&pData) :
	m_sPath(szFilepath), m_iDataSize(iDataSize), m_upData(std::move(pData)) { }

bool Moorhuhn3VirtualFile::saveToFile(const wchar_t *szFilePath) const
{
	std::ofstream output(szFilePath, std::ios::binary);
	if (!output)
		return false;

	output.write(reinterpret_cast<const char *>(m_upData.get()), m_iDataSize);
	return true;
}



bool Moorhuhn3DataFile::loadFile(const wchar_t *szFilepath)
{
	clear();

	std::ifstream input(szFilepath, std::ios::binary);
	if (!input)
		return false;

	input.exceptions(std::ios::badbit | std::ios::eofbit | std::ios::failbit);

	try
	{
		MH3FileHdr oFileHdr{};

		ReadBytes(input, &oFileHdr, sizeof(oFileHdr));
		if (std::strcmp(szMagicNumber, oFileHdr.szMagicNumber) != 0)
			return false;

		std::vector<MH3File> oVFiles;

		while (true)
		{
			MH3File oFile{};
			ReadBytes(input, &oFile, sizeof(oFile));
			if (std::strcmp(oFile.szFilepath, szTerminatingFilename) == 0)
				break;

			oVFiles.push_back(oFile);
		}

		m_oVFiles.reserve(oVFiles.size());

		for (size_t i = 0; i < oVFiles.size(); ++i)
		{
			const auto &oMeta = oVFiles[i];

			input.seekg(oMeta.iOffset, std::ios_base::beg);

			auto up_pFileData = std::make_unique<uint8_t[]>(oMeta.iSize);
			ReadBytes(input, up_pFileData.get(), oMeta.iSize);
			m_oVFiles.push_back(Moorhuhn3VirtualFile
				{
					oMeta.szFilepath,
					(size_t)oMeta.iSize,
					std::move(up_pFileData)
				});
		}

	}
	catch (...)
	{
		clear();
		return false;
	}

	return true;
}
