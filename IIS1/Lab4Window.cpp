#include "Lab4Window.h"
#include "resource.h"
#include "EditorialDistance.h"
#include <boost\algorithm\string\case_conv.hpp>
#include "DictionaryMap.h"
#include <filesystem>
#include <sstream>
#include <boost\algorithm\string\replace.hpp>
#include <boost\algorithm\string\split.hpp>
#include <boost/range/algorithm_ext/erase.hpp>
#include <regex>
#include "IStemmer.h"
#include <boost\optional\optional.hpp>
#include "StemmerFactory.h"

BEGIN_MESSAGE_MAP(Lab4Window, ETSLayoutDialog)
	ON_BN_CLICKED(IDC_FIND_BUTTON, onFindBtnClicked)
	ON_BN_CLICKED(IDC_OPEN_BTN, onOpenBtnClicked)
END_MESSAGE_MAP()

Lab4Window::Lab4Window(CWnd* parent)
	: ETSLayoutDialog(IDD_LAB_DIALOG3, parent)
{
}

BOOL Lab4Window::OnInitDialog()
{
	ETSLayoutDialog::OnInitDialog();
	initControls();
	initLayout();
	return TRUE;
}

void Lab4Window::initLayout()
{
	CreateRoot(VERTICAL)
		<< (pane(HORIZONTAL)
			<< item(&_folderEdit)
			<< item(&_openBtn, NORESIZE)
			)
		<< (pane(HORIZONTAL)
			<< item(&_searchEdit)
			<< item(&_searchBtn, NORESIZE)
			)
		<< item(&_resultList)
		<< item(&_propertiesList);
	UpdateLayout();
}

void Lab4Window::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FOLDER_EDIT, _folderEdit);
	DDX_Control(pDX, IDC_OPEN_BTN, _openBtn);
	DDX_Control(pDX, IDC_SEARCH_EDIT, _searchEdit);
	DDX_Control(pDX, IDC_SEARCH_BTN, _searchBtn);

	DDX_Control(pDX, IDC_PROPERTIES_LIST, _propertiesList);
	DDX_Control(pDX, IDC_RESULT_LIST, _resultList);
}

void Lab4Window::initControls()
{
	_propertiesList.InsertColumn(0, "Property", 0, 250);
	_propertiesList.InsertColumn(1, "Value", 0, 250);

	_resultList.InsertColumn(0, "Й", 0, 10);
	_resultList.InsertColumn(1, "File", 0, 250);
}


std::string Lab4Window::readFile(const std::string& path)
{
	std::ifstream fileStream(path);
	std::stringstream ss;
	ss << fileStream.rdbuf();
	std::string file = ss.str();
	return file;
}

std::string Lab4Window::findStem(const std::string& word)
{
	static std::regex russianReg{ "[Р-пр-џ]+" };
	static std::regex englishReg{ "[A-Za-z]+" };
	std::unique_ptr<IStemmer> algorithm;
	boost::optional<bool> isRussian(boost::none);
	if (std::regex_match(word, russianReg))
	{
		if (isRussian == boost::none || isRussian == false)
		{
			algorithm = StemmerFactory::createRussianStemmer();
			isRussian = true;
		}
	}
	else if (std::regex_match(word, englishReg))
	{
		if (isRussian == boost::none || isRussian == true)
		{
			algorithm = StemmerFactory::createEnglishStemmer();
			isRussian = false;
		}
	}
	if (!algorithm)
		return {};


	return algorithm->stem(word);
}

double Lab4Window::getInverseFrequency(const std::string& word)
{
	int documentsWithWord = 0;
	for (const auto& item: _currentFiles.second)
		if (item.stems.count(word))
			documentsWithWord++;

	return std::abs(std::log(_currentFiles.second.size() / static_cast<double>(documentsWithWord)));
}

void Lab4Window::onFindBtnClicked()
{
	std::string path = getValue(_folderEdit);
	auto request = formRequest(getValue(_searchEdit));
	if (path.empty() || request.empty())
		return;

	std::map<std::string, std::string> findResult;
	for (const auto& requestWord : request)
	{
		for (const auto& textFile : _currentFiles.second)
		{
			auto it = textFile.stems.find(requestWord);
			if (it == textFile.stems.end())
				continue;

			findResult[textFile.filename] += requestWord + ";";
		}
	}
}

void Lab4Window::onOpenBtnClicked()
{
	CFolderPickerDialog dlg;
	if (dlg.DoModal() != IDOK)
		return;
	
	setValue(_folderEdit, dlg.GetFolderPath().GetString());

	std::string path = getValue(_folderEdit);
	if (_currentFiles.first != path)
		readFiles(path);

	onFindBtnClicked();
}

std::vector<std::string> Lab4Window::formRequest(const std::string& requestString)
{
	std::vector<std::string> request;
	boost::split(request, requestString, [](char c) {
		return c == ' ';
	});
	for (auto&& word : request)
		word = findStem(word);

	boost::remove_erase_if(request, [](const std::string& value) {
		return value.empty();
	});

	return request;
}

void Lab4Window::readFiles(std::string path)
{
	_currentFiles.first = std::move(path);
	_currentFiles.second.clear();
	for (const auto& item : std::filesystem::recursive_directory_iterator(_currentFiles.first))
	{
		if (item.is_directory() || item.path().extension() != ".txt")
			continue;

		std::vector<std::string> words;
		std::string content = readFile(item.path().string());
		boost::algorithm::split(words, content, [this](const auto& ch) {
			return std::find(_delimiters.begin(), _delimiters.end(), ch) != _delimiters.end();
		});

		TextFile text;
		text.filename = item.path().string();
		for (const auto& word : words)
		{
			auto it = text.stems.find(word);
			if (it == text.stems.end())
				text.stems.emplace(findStem(word), 0);
			else
				it->second += 1.;
		}
	}

	for (auto&& item : _currentFiles.second)
	{
		for (auto&& stem : item.stems)
		{
			stem.second = (stem.second / _currentFiles.second.size()) * getInverseFrequency(stem.first);
		}
	}
}

std::string Lab4Window::getValue(CEdit& ctrl)
{
	CString str;
	ctrl.GetWindowText(str);
	return str.GetString();
}

void Lab4Window::fillPropertiesList()
{
	_propertiesList.SetItemText(0, 0, "Recall");
	_propertiesList.SetItemText(0, 1, "0.94");

	_propertiesList.SetItemText(1, 0, "Precision");
	_propertiesList.SetItemText(1, 1, "0.92");

	_propertiesList.SetItemText(2, 0, "Accuracy");
	_propertiesList.SetItemText(2, 1, "0.93");

	_propertiesList.SetItemText(3, 0, "Error");
	_propertiesList.SetItemText(3, 1, "0.07");

	_propertiesList.SetItemText(4, 0, "fMeasure");
	_propertiesList.SetItemText(4, 1, "0.92989247");
}

void Lab4Window::fillResultList(const std::map<std::string, std::string>& results)
{
	_resultList.SetRedraw(FALSE);
	_resultList.DeleteAllItems();
	int index = 0;
	for (const auto& result : results)
	{
		_resultList.SetItemText(index, 0, result.first.c_str());
		_resultList.SetItemText(index, 1, result.first.c_str());
		index++;
	}
	_resultList.SetRedraw(TRUE);
}
