#pragma once

class CDBTool : public ITaskHandle
{
public:
	CDBTool()
	{ }

	~CDBTool(){}

	virtual BOOL OnMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled, LRESULT& lResult)
	{
		bHandled = TRUE;
		//MESSAGE_HANDLER(WM_CREATE, OnCreate);

		return S_OK;
	}

	//LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//{
	//	SciterSetOption(m_hWnd, SCITER_ALPHA_WINDOW, TRUE);
	//	BOOL bRet = LoadFile(_T("this://app/login.htm"));

	//	

	//	return S_OK;
	//}

	virtual BOOL Init()
	{
		//CreateUIWindow(NULL, WS_POPUP);

		return TRUE;
	}

	virtual BOOL ProcessTask(CTask* pTask)
	{
		if (pTask->uNtfMsg == WM_LOGIN_NETACK) {
			pTask->pHost = this;

			CString strUrl = TEXT("/helperApi/login");
			CString strBody;
			//strBody.Format(TEXT("username=%s&password=%s"), strUserName, strPassWord);

			CString strRetData;
			//BOOL bRet = CHttpHelp::GetInstance().PostReq(strUrl, strBody, strRetData, TEXT("www.sutuiapp.com"));

			return TRUE;

		}

		return FALSE;
	}

	virtual BOOL CancelTask(CTask* pTask)
	{
		return TRUE;
	}

};