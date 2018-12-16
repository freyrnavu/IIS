#define _CRT_SECURE_NO_WARNINGS

#include "DictionaryManager.h"
#include "Tmorf.h"
#include <cassert>
#include <atlstr.h>
#include <algorithm>
#include <fstream>
#include <boost\algorithm\string\trim.hpp>
#include <unordered_set>
#include <boost\algorithm\string\split.hpp>
#include <boost\algorithm\string\predicate.hpp>

DictionaryManager::DictionaryManager()
{
	GetModuleFileName(NULL, (LPSTR)_path, 512);
	for (int i = strlen((LPSTR)_path); i; i--) {
		if (_path[i] == '\\') {
			_path[i + 1] = 0;
			break;
		}
	}
	strcat((LPSTR)_path, "database");

	_manager = std::make_unique<LibraryManager>("mtlemm.dll");
	_func = (MtLemmFunction)GetProcAddress(_manager->getLibrary(), "MtLemmFunc1");
	assert(_func);
}

WordInfo DictionaryManager::getWordInfo(const std::string& word)
{
	byte buff[256];

	CString str(word.c_str());
	strcpy((char*)buff, str.GetBuffer(256));

	struct TMorfAnswer TMorfAnswer1[100];
	int res = _func(buff, _path, &TMorfAnswer1[0]);

	TMorfAnswer result = TMorfAnswer1[0];
	auto it = std::find_if(std::begin(TMorfAnswer1), std::end(TMorfAnswer1), [](const auto& ans) {
		return ans.SClass != esUndefined;
	});

	if (it != std::end(TMorfAnswer1))
		result = *it;

	return WordInfo(word.c_str(), result);
}

WordInfoLab DictionaryManager::getWordInfoLab(const std::string& word)
{
	byte buff[256];

	CString str(word.c_str());
	strcpy((char*)buff, str.GetBuffer(256));

	struct TMorfAnswer TMorfAnswer1[100];
	int res = _func(buff, _path, &TMorfAnswer1[0]);

	TMorfAnswer result = TMorfAnswer1[0];
	auto it = std::find_if(std::begin(TMorfAnswer1), std::end(TMorfAnswer1), [](const auto& ans) {
		return ans.SClass != esUndefined;
	});

	if (it != std::end(TMorfAnswer1))
		result = *it;

	return WordInfoLab(word, result);
}

DictionaryMap DictionaryManager::getInfo(const std::vector<std::string> words)
{
	if (!_isFileRead)
		readFileWithWordForms();

	std::unordered_set<std::string> mainForms;
	DictionaryMap result;
	for (const auto& word : words)
	{
		auto wordInfoLab = getWordInfoLab(word);
		if (!wordInfoLab.getPartOfSpeechString().empty())
			mainForms.insert(wordInfoLab.getMainForm());
	}

	for (auto mainForm : mainForms)
	{
		auto it = std::find_if(_file.begin(), _file.end(), [&mainForm](const std::string& line) {
			return boost::starts_with(line, mainForm + " ") || boost::ends_with(line, " " + mainForm) || line.find(" " + mainForm + " ") != std::string::npos;
		});
		if (it != _file.end())
		{
			std::vector<std::string> words;
			boost::algorithm::split(words, *it, [](const auto& ch) {
				return ch == ' ';
			});
			auto it = std::remove_if(words.begin(), words.end(), [](const auto& str) {
				return str.empty();
			});
			if (it != words.end())
				words.erase(it);
			for (const auto& word : words)
				result.emplace(mainForm, getWordInfoLab(word));
		}
		else
			result.emplace(mainForm, getWordInfoLab(mainForm));
	}

	return result;
}

void DictionaryManager::readFileWithWordForms()
{
	std::ifstream filestream("d:\\stems.txt");
	
	_file.reserve(79987);
	std::string line;
	while (std::getline(filestream, line))
	{
		std::replace(line.begin(), line.end(), ',', ' ');
		std::replace(line.begin(), line.end(), '-', ' ');
		boost::trim(line);
		_file.push_back(line);
	}

	_isFileRead = true;
}
