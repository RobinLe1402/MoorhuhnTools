#include "MHJNR-XXL.hpp"

#include <fstream>


namespace
{
	constexpr char szMagicNumber[] = "MHJNR-XXL";



#pragma pack(push, 1)

	typedef struct
	{
		char     szMagicNumber[32]; // "MHJNR-XXL"
		uint32_t iFileCount;
		char     cUnknown[28];
	} MHJNRFileHdr;

	typedef struct
	{
		char     szFilepath[104];
		uint32_t iOffset;
		uint32_t iSize;
		char     cUnused[16];
	} MHJNRFile;

#pragma pack(pop)



	inline void ReadBytes(std::ifstream &stream, void *pDest, size_t iLen)
	{
		stream.read(reinterpret_cast<char *>(pDest), iLen);
	}

}


MoorhuhnJnRVirtualFile::MoorhuhnJnRVirtualFile(
	const char *szFilepath, size_t iDataSize, const uint8_t *pData) :
	m_sPath(szFilepath), m_iDataSize(iDataSize), m_upData(std::make_unique<uint8_t[]>(iDataSize))
{
	if (iDataSize == 0)
		throw std::exception("MoorhuhnJnRVirtualFile: A size of 0 is invalid.");
}

MoorhuhnJnRVirtualFile::MoorhuhnJnRVirtualFile(const char *szFilepath, size_t iDataSize,
	std::unique_ptr<uint8_t[]> &&pData) :
	m_sPath(szFilepath), m_iDataSize(iDataSize), m_upData(std::move(pData)) { }

bool MoorhuhnJnRVirtualFile::saveToFile(const wchar_t *szFilePath) const
{
	std::ofstream output(szFilePath, std::ios::binary);
	if (!output)
		return false;

	output.write(reinterpret_cast<const char *>(m_upData.get()), m_iDataSize);
	return true;
}



bool MoorhuhnJnRDataFile::loadFile(const wchar_t *szFilepath)
{
	clear();

	std::ifstream input(szFilepath, std::ios::binary);
	if (!input)
		return false;

	input.exceptions(std::ios::badbit | std::ios::eofbit | std::ios::failbit);

	try
	{
		MHJNRFileHdr oFileHdr{};

		ReadBytes(input, &oFileHdr, sizeof(oFileHdr));
		if (std::strcmp(szMagicNumber, oFileHdr.szMagicNumber) != 0)
			return false;

		if (oFileHdr.iFileCount == 0)
			return true;

		auto up_oVFileMeta = std::make_unique<MHJNRFile[]>(oFileHdr.iFileCount);
		ReadBytes(input, up_oVFileMeta.get(), sizeof(MHJNRFile) * oFileHdr.iFileCount);
		m_oVFiles.reserve(oFileHdr.iFileCount);

		for (size_t i = 0; i < oFileHdr.iFileCount; ++i)
		{
			const auto &oMeta = up_oVFileMeta.get()[i];

			input.seekg(oMeta.iOffset, std::ios_base::beg);

			auto up_pFileData = std::make_unique<uint8_t[]>(oMeta.iSize);
			ReadBytes(input, up_pFileData.get(), oMeta.iSize);
			m_oVFiles.push_back(MoorhuhnJnRVirtualFile
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
