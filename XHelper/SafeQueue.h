#pragma once

struct link
{
	link* m_pNext;
	void* m_pData;

	link()
	{
		m_pNext = NULL;
		m_pData = NULL;
	}
};

class CSafeQueue
{
	link* m_pHead;
	link* m_pTail;
	sciter::sync::mutex m_lock;

public:
	CSafeQueue()
	{
		m_pHead = new link;
		m_pTail = m_pHead;
	}

	~CSafeQueue()
	{ 
		delete m_pHead;
	}

	bool IsEmpty()
	{
		m_lock.lock();

		bool bRet = (m_pHead == m_pTail);

		m_lock.unlock();

		return bRet;
	}

	void* push(void* obj)
	{
		m_lock.lock();
		
		link* pLink = new link;
		pLink->m_pData = obj;
		m_pTail->m_pNext = pLink;
		m_pTail = pLink;

		m_lock.unlock();

		return pLink;
	}

	void* pop()
	{
		m_lock.lock();

		void* pData = NULL;
		link* pLink = m_pHead->m_pNext;
		pData = pLink->m_pData;
		if (m_pHead->m_pNext == m_pTail) {
			m_pTail = m_pHead;
		}
		m_pHead->m_pNext = pLink->m_pNext;
		delete pLink;
		
		m_lock.unlock();

		return pData;
	}
};