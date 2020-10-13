#pragma once

class ITaskHandle;

struct CTask
{
	UINT uStatus;			// 任务状态
	HWND hNtfWnd;			// 任务完成时，接收消息的窗口
	UINT uNtfMsg;			// 消息id
	sciter::value param;	// 请求参数
	sciter::value ret;		// 返回结果
	ITaskHandle* pHost;		// 任务处理者

	CTask()
	{
		uStatus = 0;
		pHost = NULL;
	}
};

class ITaskHandle
{
public:
	ITaskHandle* m_pNext;

	ITaskHandle() 
	{
		m_pNext = NULL;
	}

	virtual ~ITaskHandle() {}
	virtual BOOL Init() = 0;
	virtual BOOL ProcessTask(CTask*) = 0;
	virtual BOOL CancelTask(CTask*) = 0;
};