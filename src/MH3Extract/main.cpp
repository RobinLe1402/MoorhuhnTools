#include "MH3_DAT.hpp"

typedef struct IUnknown IUnknown;

#include <cstdio>
#include <ShlObj.h>

void PrintUsage(const wchar_t* szArgv0);

int wmain(int argc, wchar_t *argv[])
{
	if (argc != 3)
	{
		PrintUsage(argv[0]);
		return 1;
	}

	const wchar_t *szInputFile = argv[1];
	const wchar_t *szOutputDir = argv[2];

	Moorhuhn3DataFile oFile;
	if (!oFile.loadFile(szInputFile))
	{
		std::printf("Error parsing the data file \"%ls\".\n", szInputFile);
		return 1;
	}
	else
		std::printf("Successfully parsed input file.\n");

	std::wstring sOutputPath = szOutputDir;
	if (sOutputPath.length() == 0)
	{
		std::printf("Please specify an output directory.\n");
		return 1;
	}
	if (sOutputPath[sOutputPath.length() - 1] != '\\')
		sOutputPath += '\\';
	const size_t iDirLen = sOutputPath.length();
	for (const auto &oFile : oFile.getFiles())
	{
		std::printf("Extracting virtual file \"%s\" ...\n", oFile.path().c_str());

		sOutputPath = sOutputPath.substr(0, iDirLen);
		sOutputPath.reserve(sOutputPath.length() + oFile.path().length());
		for (char c : oFile.path())
		{
			sOutputPath += c;
		}

		std::wstring sFolder = sOutputPath.substr(0, sOutputPath.find_last_of('\\'));
		SHCreateDirectoryExW(NULL, sFolder.c_str(), NULL);

		if (!oFile.saveToFile(sOutputPath.c_str()))
			std::printf("  Couldn't write file \"%ls\".\n", sOutputPath.c_str());
		else
			std::printf("  Successfully extracted file \"%ls\".\n", sOutputPath.c_str());
		std::printf("\n");
	}



	return 0;
}



void PrintUsage(const wchar_t *szArgv0)
{
	std::printf(
		"Extract data from a Moorhuhn 3 moorhuhn3.dat.\n"
		"USAGE:\n"
		"%ls InputFile OutputDir"
		"\n"
		"  InputFile      The datafile.dat file from the\n"
		"                 Moorhuhn 3 installation directory.\n"
		"  OutputDir      The directory that the files should\n"
		"                 get extracted to.\n",
		szArgv0
	);
}
