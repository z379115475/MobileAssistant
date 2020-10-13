#pragma once

class CLoginWnd : public CUIWndBasic
{
	HWND m_hAppWnd;
public:
	CLoginWnd(HWND hWnd) : m_hAppWnd(hWnd)
	{ }

	~CLoginWnd(){}
	virtual BOOL OnMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled, LRESULT& lResult)
	{
		bHandled = TRUE;
		MESSAGE_HANDLER(WM_CREATE, OnCreate);
		
		return S_OK;
	}

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		SciterSetOption(m_hWnd, SCITER_ALPHA_WINDOW, TRUE);
		BOOL bRet = LoadFile(_T("this://app/login.htm"));

		SCITER_VALUE r;
		bRet = ::SciterEval(m_hWnd, L"view.msgbox(#alert, \"as\");", 27, &r);

		CString strScript = L"include library \"tiscript-sqlite\";";
		bRet = ::SciterEval(m_hWnd, strScript, strScript.GetLength(), &r);

		return S_OK;
	}

	// 添加UI事件处理函数
	BEGIN_FUNCTION_MAP
		FUNCTION_4("OnLoginClick", OnLoginClick); // 登录btn响应函数
		FUNCTION_0("OnCloseClick", OnCloseClick); // 关闭按钮响应函数
	END_FUNCTION_MAP

	sciter::value OnLoginClick(sciter::value uname, sciter::value pword, sciter::value doneCb, sciter::value onFail)
	{
		// 创建一个登录任务
		CTask* pTask = new CTask();
		pTask->hNtfWnd = m_hAppWnd;
		pTask->uNtfMsg = WM_LOGIN_NETACK;

		CString strTemp;
		strTemp.Format(_T("username=%s&password=%s"), uname.get_chars().start, pword.get_chars().start);
		pTask->param.set_item("body", strTemp.GetBuffer());
		pTask->param.set_item("onFail", onFail);

		if (CTaskPool::GetInstance().CommitTask(pTask)) {
			doneCb.call();
		}

		return 0;
	}

	sciter::value OnCloseClick()
	{
		::PostMessage(m_hAppWnd, WM_APP_CLOSE, NULL, NULL);

		return 0;
	}

	VOID OnLoginFail(CTask* pTask)
	{
		pTask->param.get_item("onFail").call(pTask->ret.get_item("data"));
	}

};