#pragma once

class CDownLoad : public ITaskHandle
{
	virtual BOOL Init()
	{
		return TRUE;
	}

	virtual BOOL ProcessTask(CTask* pTask)
	{
		if (pTask->uNtfMsg == 2) {
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