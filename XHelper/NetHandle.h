#pragma once

#include "HttpHelp.h"
class CNetHandle : public ITaskHandle
{
	virtual BOOL Init()
	{
		return TRUE;
	}

	virtual BOOL ProcessTask(CTask* pTask)
	{
		if (pTask->uNtfMsg == WM_LOGIN_NETACK) {
			pTask->pHost = this;

			CString strUrl = TEXT("/helperApi/login");

			CString strRetData;
			BOOL bRet = CHttpHelp::GetInstance().PostReq(strUrl, pTask->param.get_item("body").get_chars().start, strRetData, TEXT("sutuiapp.me"));
			if (bRet) {
				sciter::value jsonValue = sciter::value::from_string((aux::wchars)strRetData.GetBuffer(), CVT_JSON_LITERAL);
				CString stsCode = jsonValue.get_item("status_code").get_chars().start;
				if (stsCode == _T("200")) {
					pTask->ret.set_item("ret", "true");
					pTask->ret.set_item("data", jsonValue.get_item("member_data"));
				}
				else {
					pTask->ret.set_item("ret", "false");
					pTask->ret.set_item("data", jsonValue.get_item("status_message"));
				}
			}
			else {
				pTask->ret.set_item("ret", "false");
				pTask->ret.set_item("data", "ÍøÂçÒì³£.");
			}

			::PostMessage(pTask->hNtfWnd, pTask->uNtfMsg, NULL, (LPARAM)pTask);
			return TRUE;

		}

		return FALSE;
	}

	virtual BOOL CancelTask(CTask* pTask)
	{
		return TRUE;
	}
};