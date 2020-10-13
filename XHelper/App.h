#pragma once

/*
* \��  ��  ����App.h
* \˵��    ����Ӧ�ó�����
* \��      �ڣ�2016/04211
* \��      �ߣ�����.
* \��  ˾  ������ƽ����Ƽ����޹�˾
*/
#include "resources.cpp"

class CApp : public CWindowImpl<CApp>, public CMessageFilter, public CIdleHandler
{
	CLoginWnd* m_pLogin;
	CMainFrame* m_pMainFrame;
public:
	CApp()
	{ 
		m_pLogin = NULL;
		m_pMainFrame = NULL;
	}

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return FALSE;
	}

	virtual BOOL OnIdle()
	{
		return FALSE;
	}

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_APP_CLOSE, OnAppClose)
		MESSAGE_HANDLER(WM_APP_QUIT, OnAppQuit)
		MESSAGE_HANDLER(WM_LOGIN_NETACK, OnLoginNetAck)
	END_MSG_MAP()

	BOOL Init()
	{
		Create(HWND_MESSAGE, rcDefault, NULL, 0);		// ����һ�����ڣ�����������Ϣ

		sciter::archive::instance().open(aux::elements_of(resources));		// ������Դ

		m_pLogin = new CLoginWnd(m_hWnd);
		m_pLogin->CreateUIWindow(NULL, WS_POPUP);
		m_pLogin->ShowWindow(SW_SHOW);

		CTaskPool& pTaskPool = CTaskPool::GetInstance();
		
		pTaskPool.Register(new CNetHandle);
		pTaskPool.Register(new CDownLoad);
		pTaskPool.Register(new CADBTool);
		pTaskPool.Register(new CDBTool);

		pTaskPool.Init();
	
		return TRUE;
	}

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		return S_OK;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		// unregister message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);

		PostQuitMessage(0);

		return S_OK;
	}

	LRESULT OnLoginNetAck(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
	{
		CTask* pTask = (CTask*)lParam;

		CString strRet = pTask->ret.get_item("ret").get_chars().start;
		if (strRet == _T("true")) {
			::DestroyWindow(m_pLogin->get_hwnd());
			delete m_pLogin;
			m_pLogin = NULL;
			m_pMainFrame = new CMainFrame(m_hWnd);
			m_pMainFrame->CreateUIWindow(NULL, WS_POPUP);
			m_pMainFrame->ShowWindow(SW_SHOW);
		}
		else {
			// ���½�����ʾ��¼ʧ��
			m_pLogin->OnLoginFail(pTask);
		}

		delete pTask;

		return S_OK;
	}

	LRESULT OnAppClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		if (m_pLogin != NULL) {
			delete m_pLogin;
		}

		if (m_pMainFrame != NULL) {
			delete m_pMainFrame;
		}

		// ����һ����¼����
		CTask* pTask = new CTask();
		pTask->hNtfWnd = m_hWnd;
		pTask->uNtfMsg = WM_APP_QUIT;
		CTaskPool::GetInstance().CommitTask(pTask);

		return S_OK;
	}

	LRESULT OnAppQuit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
	{
		delete (CTask*)lParam;

		DestroyWindow();

		return S_OK;
	}

};