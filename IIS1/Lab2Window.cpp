#include "Lab2Window.h"
#include "resource.h"
#include <fstream>
#include <sstream>
#include <boost\algorithm\string\split.hpp>
#include "IStemmer.h"
#include <regex>
#include "StemmerFactory.h"
#include <boost\optional\optional.hpp>
#include <boost\algorithm\string\replace.hpp>
#include "Lab3Window.h"

BEGIN_MESSAGE_MAP(Lab2Window, ETSLayoutDialog)
	ON_BN_CLICKED(IDC_OPEN_BTN, onOpenBtnClicked)
	ON_BN_CLICKED(IDC_BUTTON1, onLab3BtnClicked)
END_MESSAGE_MAP()

Lab2Window::Lab2Window(CWnd* parent = NULL) :
	ETSLayoutDialog(IDD_LAB2_DIALOG, parent)
{
}

BOOL Lab2Window::OnInitDialog()
{
	ETSLayoutDialog::OnInitDialog();
	initListCtrl();
	initLayout();
	return TRUE;
}

void Lab2Window::initLayout()
{
	CreateRoot(VERTICAL)
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(&_fileNameEdit, ABSOLUTE_VERT)
			<< item(&_openBtn, NORESIZE)
			)
		<< item(&_textEdit, ABSOLUTE_VERT)
		<< item(&_stemCtrl)
		<< (pane(HORIZONTAL)
			<< itemGrowing(HORIZONTAL)
			<< item(IDC_BUTTON1, NORESIZE)
			);
	UpdateLayout();
}

void Lab2Window::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILE_NAME, _fileNameEdit);
	DDX_Control(pDX, IDC_OPEN_BTN, _openBtn);
	DDX_Control(pDX, IDC_TEXT, _textEdit);
	DDX_Control(pDX, IDC_LIST2, _stemCtrl);
}

void Lab2Window::initListCtrl()
{
	_stemCtrl.InsertColumn(0, "Основа слова", 0, 500);
	_stemCtrl.InsertColumn(1, "Частота встречаемости", 0 , 100);
}

void Lab2Window::fillListCtrl(const std::map<std::string, int, CaseInsensitiveComparator>& stems)
{
	_stemCtrl.DeleteAllItems();
	_stemCtrl.LockWindowUpdate();
	{
		int i = 0;
		for (const auto& item : stems)
		{
			_stemCtrl.InsertItem(i, item.first.c_str());
			_stemCtrl.SetItemText(i++, 1, std::to_string(item.second).c_str());
		}
	}
	_stemCtrl.UnlockWindowUpdate();
}

void Lab2Window::onOpenBtnClicked()
{
	CFileDialog dlg(true);
	if (dlg.DoModal() == IDOK)
	{
		CString path = dlg.GetPathName();
		_fileNameEdit.SetWindowText(path);
		_text = readFile(path.GetString());
		_textEdit.SetWindowText(_text.c_str());
		std::vector<std::string> text;
		boost::algorithm::split(text, _text, [this](const auto& ch) {
			return std::find(_delimiters.begin(), _delimiters.end(), ch) != _delimiters.end();
		});
		text.erase(std::remove(text.begin(), text.end(), ""), text.end());
		std::regex russianReg{ "[А-Яа-я]+" };
		std::regex englishReg{ "[A-Za-z]+" };
		std::unique_ptr<IStemmer> algorithm;
		_stems.clear();
		boost::optional<bool> isRussian(boost::none);
		for (const auto& word : text)
		{
			if (std::regex_match(word, russianReg))
			{
				if ( isRussian == boost::none || isRussian == false)
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
			else
			{
				continue;
			}

			_stems[algorithm->stem(word)]++;
		}
		fillListCtrl(_stems);
	}
}

void Lab2Window::onLab3BtnClicked()
{
	std::vector<std::string> res;
	for (const auto& stem : _stems)
		res.push_back(stem.first);
	Lab3Window(this, res).DoModal();
}

std::string Lab2Window::readFile(const std::string& path)
{
	std::ifstream fileStream(path);
	std::stringstream ss;
	ss << fileStream.rdbuf();
	std::string file = ss.str();
	boost::replace_all(file, "\n", "\r\n");
	return file;
}
