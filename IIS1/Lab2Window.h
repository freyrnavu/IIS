#pragma once
#include "ETSLayout.h"
#include <afxcmn.h>
#include <string>
#include <vector>
#include "DictionaryMap.h"

class Lab2Window : public ETSLayoutDialog
{
public:
	Lab2Window(CWnd* parent);
	~Lab2Window() = default;

private:
	BOOL OnInitDialog() override;
	void initLayout();
	void DoDataExchange(CDataExchange* pDX) override;
	void initListCtrl();
	void fillListCtrl(const std::map<std::string, int, CaseInsensitiveComparator>& stems);

	void onOpenBtnClicked();
	void onLab3BtnClicked();
	std::string readFile(const std::string& path);

	DECLARE_MESSAGE_MAP()

private:
	CEdit _textEdit;
	CEdit _fileNameEdit;
	CButton _openBtn;
	CListCtrl _stemCtrl;

	std::string _text;
	std::map<std::string, int, CaseInsensitiveComparator> _stems;

	const std::vector<char> _delimiters = {
		' ',
		':',
		',',
		'-',
		'.',
		'!',
		'(',
		';',
		')',
		'?',
		'\\',
		'\'',
		'–',
		'\n',
		'«',
		'»',
		'\r'
	};
};

