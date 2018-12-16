#pragma once
#include <memory>
#include <string>
#include "WordInfo.h"
#include "LibraryManager.h"
#include "DictionaryMap.h"
#include "WordInfoPract.h"

class LibraryManager;
typedef DWORD(CALLBACK* MtLemmFunction)(byte *buff, byte *datapath, struct TMorfAnswer *TMorfAnswerPtr);

class DictionaryManager
{
public:
	DictionaryManager();
	~DictionaryManager() = default;

	WordInfo getWordInfo(const std::string& word);

	WordInfoLab getWordInfoLab(const std::string& word);

	DictionaryMap getInfo(const std::vector<std::string> words);

private:
	void readFileWithWordForms();

private:
	byte _path[512];
	std::unique_ptr<LibraryManager> _manager;
	MtLemmFunction _func;
	std::vector<std::string> _file;
	bool _isFileRead = false;

};

