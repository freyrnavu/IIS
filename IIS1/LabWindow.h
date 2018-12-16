#pragma once
#include "ETSLayout.h"
#include <string>
#include <vector>
#include "WordInfoPract.h"
#include "DictionaryMap.h"
#include "Lab3Window.h"

class LabWindow : public ETSLayoutDialog
{
public:
	LabWindow(CWnd* parent = NULL);
	~LabWindow() = default;

private:
	BOOL OnInitDialog() override;
	void initLayout();
	void DoDataExchange(CDataExchange* pDX) override;

	void onOpenBtnClicked();
	void onMainFormsSelectionChanged();
	void onWordFormsSelectionChanged();

	void fillMainFormsList();
	void fillFormsList();
	void fillPropertiesList();
	std::string getSelectedText(CListBox& item);
	std::string readFile(const std::string& path);

	DECLARE_MESSAGE_MAP()

private:
	CEdit _textEdit;
	CEdit _fileNameEdit;
	CButton _openBtn;
	std::string _text;
	CListBox _mainFormsList;
	CListBox _wordFormsList;
	CListBox _propertiesList;

	DictionaryMap _info;
};
