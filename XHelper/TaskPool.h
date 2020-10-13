#pragma once

class CTaskQueue
{
	CSafeQueue m_TaskQueue;
	sciter::sync::event m_event;

public:
	CTaskQueue() {}

	~CTaskQueue()
	{
		while (!m_TaskQueue.IsEmpty()) {
			delete static_cast<CTask*>(m_TaskQueue.pop());
		}
	}

	BOOL GetTask(CTask** p)
	{
		if (m_TaskQueue.IsEmpty()) {
			m_event.wait();
		}

		*p = static_cast<CTask*>(m_TaskQueue.pop());
		if ((*p)->uNtfMsg == WM_APP_QUIT) {			// APP退出消息
			return FALSE;
		}

		return TRUE;
	}

	HANDLE CommitTask(CTask* pReqTask)
	{
		HANDLE h = m_TaskQueue.push(pReqTask);
		m_event.signal();

		return h;
	}
};

class CTaskPool
{
	HANDLE       m_hThread;
	CTaskQueue   m_tQueue;
	ITaskHandle* m_TaskHandle;

	CTaskPool() 
	{
		m_hThread = NULL;
		m_TaskHandle = NULL;
	}
public:
	static CTaskPool& GetInstance()
	{
		static CTaskPool obj;

		return obj;
	}

	~CTaskPool() 
	{ }

	BOOL Init()
	{
		// 创建工作线程
		if (m_TaskHandle != NULL) {
			ITaskHandle* pThis = m_TaskHandle;
			while (pThis->Init() && pThis->m_pNext != NULL) {
				pThis = pThis->m_pNext;
			}
		}

		m_hThread = (HANDLE)_beginthreadex(NULL, NULL, WorkTreadex, this, 0, NULL);

		return TRUE;
	}

	BOOL HandleTask(CTask* pTask)
	{
		if (m_TaskHandle == NULL) {
			return FALSE;
		}

		// 循环方案
		ITaskHandle* pThis = m_TaskHandle;
		while (!pThis->ProcessTask(pTask) && pThis->m_pNext != NULL) {
			pThis = pThis->m_pNext;
		}

		return TRUE;
	}

	BOOL Run()
	{
		CTask* pTask = NULL;
		while (m_tQueue.GetTask(&pTask)) {					// 任务循环
			HandleTask(pTask);								// 任务处理
		}

		HandleTask(pTask);
		::PostMessage(pTask->hNtfWnd, pTask->uNtfMsg, NULL, (LPARAM)pTask);		// 退出消息

		return TRUE;
	}

	static UINT WINAPI WorkTreadex(LPVOID lpParameter)
	{
		CTaskPool* pThis = static_cast<CTaskPool*>(lpParameter);
		pThis->Run();

		return 0;
	}

	HANDLE CommitTask(CTask* pReqTask)
	{
		return m_tQueue.CommitTask(pReqTask);
	}

	BOOL Register(ITaskHandle* pTaskHandle)
	{
		if (m_TaskHandle == NULL) {
			m_TaskHandle = pTaskHandle;
		}
		else {
			ITaskHandle* pHandle = m_TaskHandle;
			while (pHandle->m_pNext != NULL) {
				pHandle = pHandle->m_pNext;
			}

			pHandle->m_pNext = pTaskHandle;
		}

		return  TRUE;
	}
};