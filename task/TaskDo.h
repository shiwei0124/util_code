#ifndef __TASKDO_H__
#define __TASKDO_H__

#include "hpr/HPR_ThreadPoolFlex.h"
#include "hpr/HPR_Thread.h"
#include "hpr/HPR_Event.h"
#include "hpr/HPR_Guard.h"
#include <queue>


#define MAX_THREAD_NUM 1
#define DEFAULT_QUEUE_MAX_SIZE  100000


class CBaseTask
{
	CBaseTask() {};
	virtual ~CBaseTask() {};
};

//需要析构函数进行成员的释放
typedef struct st_taskinfo
{
	int nTaskType;
	//void* pTaskInfo
	CBaseTask * pTaskInfo;
    st_taskinfo* pNext;
	st_taskinfo()
	{
		nTaskType = 0;
		pTaskInfo = NULL;
		pNext = NULL;
	}
	~st_taskinfo()
	{
		if (pTaskInfo != NULL)
		{
			//对于void*指针对象A而言，进行delete会有不同的效果，比如A内部还有new，则delete的时候不会释放内存
			//对于A内部没有new,则delete的时候会释放内存。
			//所以建议尽量手动delete pTaskInfo的对象，并且将该指针转换成真是对象的指针在进行delete
			//将此处的pTaskInfo改成CBaseTask类型的基类后，将Task都继承该基类，可以避免此类问题
			delete pTaskInfo;
			pTaskInfo = NULL;
		}
		if (pNext != NULL)
		{
			delete pNext;
			pNext = NULL;
		}
	}
}TASKINFO;

template<typename T>
class CTaskQueue
{
public:
	CTaskQueue(void){}
	~CTaskQueue(void){}
	
	void RemoveAll()
	{
		T *pTask = NULL;
		HPR_Guard guard(&m_CriticalTask);

		while(!m_queueTask.empty())
		{
			pTask = m_queueTask.front();

			if(pTask)
			{
				delete pTask;
				pTask = NULL;
			}

			m_queueTask.pop();
		}
	}

	T* GetHead()
	{
		T *pTask = NULL;
		HPR_Guard guard(&m_CriticalTask);

		if(m_queueTask.empty())
		{
			return pTask;
		}

		pTask = m_queueTask.front();
		m_queueTask.pop();

		return pTask;
	}

	HPR_BOOL Add(T* pTask)
	{
		if (pTask == NULL)
		{
			return HPR_FALSE;
		}

		HPR_Guard guard(&m_CriticalTask);

		m_queueTask.push(pTask);

		return HPR_TRUE;
	}

	unsigned int GetSize()
	{
		HPR_Guard guard(&m_CriticalTask);
		unsigned int iSize = m_queueTask.size();
		return iSize;
	}

private:
	std::queue<T*> m_queueTask;
	HPR_Mutex m_CriticalTask;

};


class CTaskDo
{
public:
	CTaskDo(void);
	virtual ~CTaskDo(void);
public:

	static void* CALLBACK CommitTask(void* lpParam);

	HPR_BOOL Task_Start(void);

	HPR_BOOL Task_Stop(void);

	/**	@fn	HPR_BOOL CTaskDo::Task_Add(TASKINFO* pTask)
	 *	@brief 添加任务队列，
	 *	@param[in] pTask 
	 *	@return	添加成功则返回HPR_TRUE  如果队列超过了设置的最大值，则返回HPR_FALSE
	 */
	HPR_BOOL Task_Add(TASKINFO* pTask);

	HPR_BOOL Task_Clear(void);

	void FreeTaskInfo(TASKINFO* OperTask);

	unsigned int GetQueueSize();

	virtual HPR_BOOL Task_Do(TASKINFO* OperTask);
	/**	@fn	HPR_UINT32 CTaskDo::SetMaxQueueSize(HPR_INT32 nMaxSize)
	 *	@brief 设置处理队列最大的数目，默认为DEFAULT_QUEUE_MAX_SIZE
	 *	@param[in] nMaxSize 
	 *	@return	
	 */
	void SetMaxQueueSize(int nMaxSize) { m_nMaxSize = nMaxSize; }

	HPR_HANDLE m_hTaskEvent;
	HPR_BOOL m_bRunning;

private:
	CTaskQueue<TASKINFO> m_TaskQueue;
    int m_nMaxSize;
	HPR_HANDLE m_hTaskThread[MAX_THREAD_NUM];
};
#endif // __TASKDO_H__
