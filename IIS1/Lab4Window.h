#pragma once
#include "ETSLayout.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <map>

struct TextFile
{
	std::string filename;
	std::unordered_map<std::string, double> stems;
};

class Lab4Window : public ETSLayoutDialog
{
public:
	explicit Lab4Window(CWnd* parent);
	~Lab4Window() = default;
private:
	DECLARE_MESSAGE_MAP()

	BOOL OnInitDialog() override;
	void initLayout();
	void DoDataExchange(CDataExchange* pDX) override;
	void initControls();

	void onFindBtnClicked();
	void onOpenBtnClicked();

	template <typename T>
	void setValue(CEdit& ctrl, T val)
	{
		if constexpr (std::is_same<T, std::string>::value)
			ctrl.SetWindowText(val.c_str());
		else if constexpr (std::is_same<T, int>::value)
			ctrl.SetWindowText(std::to_string(val).c_str());
		else
			ctrl.SetWindowText(val);
	};
	std::string getValue(CEdit& ctrl);

	void fillPropertiesList();
	void fillResultList(const std::map<std::string, std::string>& results);

	void readFiles(std::string path);
	std::vector<std::string> formRequest(const std::string& requestString);
	std::string readFile(const std::string& path);
	std::string findStem(const std::string& word);
	double getInverseFrequency(const std::string& word);


private:
	CEdit _folderEdit;
	CButton _openBtn;
	CEdit _searchEdit;
	CButton _searchBtn;

	CListCtrl _propertiesList;
	CListCtrl _resultList;

	std::pair<std::string, std::vector<TextFile>> _currentFiles;
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
