#pragma once

class CUIWndBasic : public CWindowImpl<CUIWndBasic>,
	public sciter::host<CUIWndBasic>,
	public sciter::event_handler
{
public:
	DECLARE_WND_CLASS_EX(_T("XtHostWnd"), CS_HREDRAW | CS_VREDRAW, COLOR_WINDOW)

	CUIWndBasic()
	{ }

	virtual ~CUIWndBasic()
	{ }

	HWND CreateUIWindow(HWND hParent, DWORD dwStyle = 0)
	{
		RECT rect = { 0, 0, 1, 1 };
		return Create(hParent, rect, NULL, dwStyle);
	}

	HWND get_hwnd()
	{
		return m_hWnd;
	}

	HINSTANCE get_resource_instance()
	{
		return _Module.m_hInst;
	}

	BOOL LoadFile(const WCHAR* file)
	{
		return FALSE != ::SciterLoadFile(m_hWnd, file);
	}

	virtual BOOL OnMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled, LRESULT& lResult)
	{
		lResult = DefWindowProc(uMsg, wParam, lParam);
		return TRUE;
	}

	BOOL MsgDelegate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled, LRESULT& lResult)
	{
		if (uMsg == WM_CREATE) {
			setup_callback();									// 向Sciter引擎注册callback
			sciter::attach_dom_event_handler(hWnd, this);
		}

		if (uMsg == WM_DESTROY) {
			// sciter::detach_dom_event_handler(hWnd, this); // 该处引擎在view.close()已经调用
		}

		return this->OnMessage(hWnd, uMsg, wParam, lParam, bHandled, lResult);
	}

	BEGIN_MSG_MAP(CWindowImpl<CUIWndBasic>)
		lResult = SciterProcND(hWnd, uMsg, wParam, lParam, &bHandled);
		if (bHandled)
			return lResult;
		MsgDelegate(hWnd, uMsg, wParam, lParam, bHandled, lResult);
	END_MSG_MAP()
};