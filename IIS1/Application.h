#pragma once
#include <afxwinappex.h>
#include <memory>
//#ifndef __AFXWIN_H__
//#include "stdafx.h"
//#endif

class MainWindow;

class Application : public CWinAppEx
{
public:
	Application();
	~Application();
	BOOL InitInstance() override;

protected:
	DECLARE_MESSAGE_MAP()

private:
	// Реализация
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	std::unique_ptr<MainWindow> _window;

};

