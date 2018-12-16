#include "LabWindow.h"
#include "resource.h"
#include "DictionaryManager.h"
#include <fstream>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/range/adaptor/transformed.hpp>

BEGIN_MESSAGE_MAP(LabWindow, ETSLayoutDialog)
	ON_BN_CLICKED(IDC_OPEN_BTN, onOpenBtnClicked)
	ON_LBN_SELCHANGE(IDC_MAIN_FORMS_LIST, onMainFormsSelectionChanged)
	ON_LBN_SELCHANGE(IDC_FORMS_LIST, onWordFormsSelectionChanged)
END_MESSAGE_MAP()

LabWindow::LabWindow(CWnd* parent) : ETSLayoutDialog(IDD_LAB_DIALOG, parent)
{
}

BOOL LabWindow::OnInitDialog()
{
	ETSLayoutDialog::OnInitDialog();
	initLayout();
	return TRUE;
}

void LabWindow::initLayout()
{
	CreateRoot(VERTICAL)
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(&_fileNameEdit, ABSOLUTE_VERT)
			<< item(&_openBtn, NORESIZE)
			)
		<< item(&_textEdit, ABSOLUTE_VERT)
		<< (pane(HORIZONTAL)
			<< item(IDC_MAIN_FORMS_LIST)
			<< item(IDC_FORMS_LIST)
			<< item(IDC_PROPERTIES_LIST)
			);
	UpdateLayout();
}

void LabWindow::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILE_NAME, _fileNameEdit);
	DDX_Control(pDX, IDC_OPEN_BTN, _openBtn);
	DDX_Control(pDX, IDC_TEXT, _textEdit);
	DDX_Control(pDX, IDC_MAIN_FORMS_LIST, _mainFormsList);
	DDX_Control(pDX, IDC_FORMS_LIST, _wordFormsList);
	DDX_Control(pDX, IDC_PROPERTIES_LIST, _propertiesList);
}

void LabWindow::onOpenBtnClicked()
{
	CFileDialog dlg(true);
	if (dlg.DoModal() == IDOK)
	{
		CString path = dlg.GetPathName();
		_fileNameEdit.SetWindowText(path);
		_text = readFile(path.GetString());
		_textEdit.SetWindowText(_text.c_str());
		DictionaryManager manager;
		std::vector<std::string> text;
		boost::algorithm::split(text, _text, [this](const auto& ch) {
			return ch == ' ';
		});
		text.erase(std::remove(text.begin(), text.end(), ""), text.end());
		_info = manager.getInfo(text);
		fillMainFormsList();
	}
}

void LabWindow::onMainFormsSelectionChanged()
{
	fillFormsList();
}

void LabWindow::onWordFormsSelectionChanged()
{
	fillPropertiesList();
}

void LabWindow::fillMainFormsList()
{
	_mainFormsList.ResetContent();
	_wordFormsList.ResetContent();
	_propertiesList.ResetContent();

	_mainFormsList.LockWindowUpdate();
	int i = 0;
	for (auto it = _info.begin(); it != _info.end(); it = _info.upper_bound(it->first))
		_mainFormsList.AddString(it->first.c_str());
	_mainFormsList.UnlockWindowUpdate();
}

void LabWindow::fillFormsList()
{
	_wordFormsList.ResetContent();
	_propertiesList.ResetContent();
	
	std::string selText = getSelectedText(_mainFormsList);
	if (selText.empty())
		return;

	_wordFormsList.LockWindowUpdate();
	auto values = _info.equal_range(selText);
	for (auto it = values.first; it != values.second; ++it)
		_wordFormsList.AddString(it->second.getWord().c_str());
	_wordFormsList.UnlockWindowUpdate();
}

void LabWindow::fillPropertiesList()
{
	_propertiesList.ResetContent();
	std::string selMainForm = getSelectedText(_mainFormsList);
	std::string selWordForm = getSelectedText(_wordFormsList);
	auto values = _info.equal_range(selMainForm);
	auto it = std::find_if(values.first, values.second, [&selWordForm](const auto& node) {
		return node.second.getWord() == selWordForm;
	});

	if (it == values.second)
		return;
	_propertiesList.LockWindowUpdate();
	auto setIfNotEmpty = [this](const std::string& str) {
		if (str.empty())
			return;

		_propertiesList.AddString(str.c_str());
	};
	setIfNotEmpty(it->second.getPartOfSpeechString());
	setIfNotEmpty(it->second.getAnimationString());
	setIfNotEmpty(it->second.getAspectString());
	setIfNotEmpty(it->second.getCaseString());
	setIfNotEmpty(it->second.getComparativeString());
	setIfNotEmpty(it->second.getGenderString());
	setIfNotEmpty(it->second.getNumberString());
	setIfNotEmpty(it->second.getPersonString());
	setIfNotEmpty(it->second.getReflectionString());
	setIfNotEmpty(it->second.getTimeString());
	setIfNotEmpty(it->second.getVoiceString());
	_propertiesList.UnlockWindowUpdate();
}

std::string LabWindow::getSelectedText(CListBox& item)
{
	int sel = item.GetCurSel();
	if (sel == LB_ERR)
		return "";
	
	CString str;
	item.GetText(sel, str);
	return str.GetString();
}

std::string LabWindow::readFile(const std::string& path)
{
	std::ifstream fileStream(path);
	std::stringstream ss;
	ss << fileStream.rdbuf();
	return ss.str();
}

