#pragma once

class ITaskHandle;

struct CTask
{
	UINT uStatus;			// ����״̬
	HWND hNtfWnd;			// �������ʱ��������Ϣ�Ĵ���
	UINT uNtfMsg;			// ��Ϣid
	sciter::value param;	// �������
	sciter::value ret;		// ���ؽ��
	ITaskHandle* pHost;		// ��������

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