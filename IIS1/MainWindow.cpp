#include "MainWindow.h"
#include "resource.h"
#include "DictionaryManager.h"
#include <fstream>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include "LabWindow.h"
#include "Lab2Window.h"
#include "Lab3Window.h"
#include "Lab4Window.h"

BEGIN_MESSAGE_MAP(MainWindow, ETSLayoutDialog)
	ON_BN_CLICKED(IDC_OPEN_BTN, onOpenBtnClicked)
	ON_BN_CLICKED(IDOK, closeApplication)
	ON_BN_CLICKED(IDCANCEL, closeApplication)
	ON_BN_CLICKED(IDC_LAB_BTN, openLabWindow)
	ON_BN_CLICKED(IDC_LAB2_BTN, OnBnClickedLab2Btn)
	ON_BN_CLICKED(IDC_LAB4_BTN, OnBnClickedLab4Btn)
END_MESSAGE_MAP()

MainWindow::MainWindow(CWnd* parent) : ETSLayoutDialog(IDD_MAIN_WINDOW, parent)
{
}


MainWindow::~MainWindow()
{
}

BOOL MainWindow::OnInitDialog()
{
	ETSLayoutDialog::OnInitDialog();
	initLayout();
	return TRUE;
}

void MainWindow::initLayout()
{
	CreateRoot(VERTICAL)
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(&_fileNameEdit, ABSOLUTE_VERT)
			<< item(&_openBtn, NORESIZE)
			)
		<< item(&_textEdit)
		<< item(&_morphTextEdit)
		<< (pane(HORIZONTAL)
			<< itemGrowing(HORIZONTAL)
			<< item(IDOK, NORESIZE)
			);
	UpdateLayout();
}

void MainWindow::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILE_NAME, _fileNameEdit);
	DDX_Control(pDX, IDC_OPEN_BTN, _openBtn);
	DDX_Control(pDX, IDC_TEXT, _textEdit);
	DDX_Control(pDX, IDC_MORPH_TEXT, _morphTextEdit);
}

void MainWindow::onOpenBtnClicked()
{
	CFileDialog dlg(true);
	if (dlg.DoModal() == IDOK)
	{
		CString path = dlg.GetPathName();
		_fileNameEdit.SetWindowText(path);
		_text = readFile(path.GetString());
		DictionaryManager manager;
		std::vector<std::string> text;
		boost::algorithm::split(text, _text, [this](const auto& ch) {
			return ch == ' ' || ch == '\n';
		});
		text.erase(std::remove(text.begin(), text.end(), ""), text.end());
		_morphedText = std::vector<WordInfo>();
		for (const auto& word : text)
			_morphedText.emplace_back(manager.getWordInfo(word));
		fillControls();
	}
}

std::string MainWindow::readFile(const std::string& path)
{
	std::ifstream fileStream(path);
	std::stringstream ss;
	ss << fileStream.rdbuf();
	return ss.str();
}

void MainWindow::fillControls()
{
	const std::string morphedText = boost::algorithm::join(_morphedText | boost::adaptors::transformed([](const auto& item) {
		return item.getWordInfo();
	}), " ");
	_textEdit.SetWindowText(_text.c_str());
	_morphTextEdit.SetWindowText(morphedText.c_str());
}

void MainWindow::closeApplication()
{
	PostQuitMessage(0);
}

void MainWindow::openLabWindow()
{
	LabWindow wnd(this);
	wnd.DoModal();
}

void MainWindow::OnBnClickedLab2Btn()
{
	Lab2Window(this).DoModal();
}

void MainWindow::OnBnClickedLab4Btn()
{
	Lab4Window(this).DoModal();
}
