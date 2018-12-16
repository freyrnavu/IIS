#include "Application.h"
#include "MainWindow.h"

BEGIN_MESSAGE_MAP(Application, CWinAppEx)
END_MESSAGE_MAP()

Application::Application()
{
	m_bHiColorIcons = TRUE;
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
	SetAppID(_T("MFCApplication5.AppID.NoVersion"));
}

Application::~Application()
{
}

BOOL Application::InitInstance()
{
	_window = std::make_unique<MainWindow>();
	m_pMainWnd = _window.get();
	_window->DoModal();

	return TRUE;
}

Application theApp;