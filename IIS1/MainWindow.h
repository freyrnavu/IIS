#pragma once
#include "ETSLayout.h"
#include <string>
#include <vector>
#include "WordInfo.h"

class MainWindow :public ETSLayoutDialog
{
public:
	MainWindow(CWnd* parent = NULL);
	~MainWindow();

protected:
	BOOL OnInitDialog() override;
	void initLayout();
	void DoDataExchange(CDataExchange* pDX) override;

	void onOpenBtnClicked();
	std::string readFile(const std::string& path);

	void fillControls();
	void closeApplication();
	void openLabWindow();
	void OnBnClickedLab2Btn();
	void OnBnClickedLab3Btn();

	DECLARE_MESSAGE_MAP()

private:
	CEdit _fileNameEdit;
	CEdit _textEdit;
	CEdit _morphTextEdit;
	CButton _openBtn;
	std::string _text;
	std::vector<WordInfo> _morphedText;
	std::vector<WordInfo> _words;
};
