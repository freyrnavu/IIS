#include "Lab3Window.h"
#include "resource.h"
#include "EditorialDistance.h"
#include <boost\algorithm\string\case_conv.hpp>
#include "DictionaryMap.h"

BEGIN_MESSAGE_MAP(Lab3Window, ETSLayoutDialog)
	ON_BN_CLICKED(IDC_FIND_BUTTON, onFindBtnClicked)
END_MESSAGE_MAP()

Lab3Window::Lab3Window(CWnd* parent, std::vector<std::string> stems)
	: ETSLayoutDialog(IDD_LAB_DIALOG3, parent)
	, _stems(stems)
{
}

BOOL Lab3Window::OnInitDialog()
{
	ETSLayoutDialog::OnInitDialog();
	initListCtrl();
	initLayout();
	return TRUE;
}

void Lab3Window::initLayout()
{
	CreateRoot(VERTICAL)
		<< (pane(HORIZONTAL, ABSOLUTE_VERT)
			<< item(&_word1Edit, ABSOLUTE_VERT)
			<< item(IDC_FIND_BUTTON, ABSOLUTE_VERT))
		<< item(&_listCtrl);
	UpdateLayout();
}

void Lab3Window::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WORD1_EDIT, _word1Edit);
	DDX_Control(pDX, IDC_LIST1, _listCtrl);
}

void Lab3Window::initListCtrl()
{
	_listCtrl.InsertColumn(0, "Слова", 0, 250);
	_listCtrl.InsertColumn(1, "Дистанционное расстояние", 0, 250);
}


void Lab3Window::onFindBtnClicked()
{
	std::string word1 = getValue(_word1Edit);
	boost::to_lower(word1);
	EditorialDist dist;
	std::map<std::string, int, CaseInsensitiveComparator> res;
	for (const auto& str : _stems)
	{
		std::string lowStr = boost::to_lower_copy(str);
		if (int i = dist.findEditDist(word1, str); i < 5)
			res[str] = dist.findEditDist(word1, str);
	}
	int i = 0;
	_listCtrl.DeleteAllItems();
	_listCtrl.LockWindowUpdate();
	for (const auto& item : res)
	{
		_listCtrl.InsertItem(i, item.first.c_str());
		_listCtrl.SetItemText(i++, 1, std::to_string(item.second).c_str());
	}
	_listCtrl.UnlockWindowUpdate();
}

std::string Lab3Window::getValue(CEdit& ctrl)
{
	CString str;
	ctrl.GetWindowText(str);
	return str.GetString();
}
