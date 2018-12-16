#pragma once
#include "ETSLayout.h"
#include <string>
#include <vector>
class Lab3Window : public ETSLayoutDialog
{
public:
	Lab3Window(CWnd* parent, std::vector<std::string> stems);
	~Lab3Window() = default;
private:
	DECLARE_MESSAGE_MAP()

	BOOL OnInitDialog() override;
	void initLayout();
	void DoDataExchange(CDataExchange* pDX) override;

	void onFindBtnClicked();

	std::string getValue(CEdit& ctrl);

	template <typename T>
	void setValue(CEdit& ctrl, T val)
	{
		if constexpr (std::is_same<T, std::string>::value)
			ctrl.SetWindowText(val.c_str());
		else if constexpr (std::is_same<T, int>::value)
			ctrl.SetWindowText(std::to_string(val).c_str());
	};

	void initListCtrl();

private:
	CEdit _word1Edit;
	CEdit _resultEdit;
	CButton _btn;

	CListCtrl _listCtrl;
	std::vector<std::string> _stems;
};

