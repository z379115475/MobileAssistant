// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CMainFrame : public CUIWndBasic
{
	HWND m_hAppWnd;
public:
	CMainFrame(HWND hWnd) : m_hAppWnd(hWnd)
	{ }

	virtual BOOL OnMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled, LRESULT& lResult)
	{
		bHandled = TRUE;
		MESSAGE_HANDLER(WM_CREATE, OnCreate);
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy);

		return S_OK;
	}

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		SciterSetOption(m_hWnd, SCITER_ALPHA_WINDOW, TRUE);
		BOOL bRet = LoadFile(_T("this://app/main.htm"));
		return S_OK;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		::PostMessage(m_hAppWnd, WM_APP_CLOSE, NULL, NULL);

		return S_OK;
	}
};
