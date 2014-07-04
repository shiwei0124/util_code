#include "TaskDo.h"

static const unsigned int g_ThreadPoolMaxThreadNum = 16;
static const unsigned int g_ThreadPoolIdleTime = 2*1000;


CTaskDo::CTaskDo(void)
{
	for (int i=0 ;i< MAX_THREAD_NUM;i++)
	{
		m_hTaskThread[i] = HPR_INVALID_THREAD;
	}

	m_hTaskEvent = HPR_CreateEvent(HPR_FALSE);
	HPR_ResetEvent(m_hTaskEvent);
    m_nMaxSize = DEFAULT_QUEUE_MAX_SIZE;
	m_bRunning = HPR_FALSE;
}

CTaskDo::~CTaskDo(HPR_VOID)
{
	Task_Clear();
	Task_Stop();

	if(m_hTaskEvent)
	{
		HPR_CloseEvent(m_hTaskEvent);
		m_hTaskEvent = NULL;
	}

}

/**	@fn	HPR_BOOL CTaskDo::Task_Start(HPR_VOID)
*	@brief 
*	@param[in]  
*	@return	
*/
HPR_BOOL CTaskDo::Task_Start( HPR_VOID )
{
	Task_Stop();
	m_bRunning = HPR_TRUE;
	for (int i=0;i<MAX_THREAD_NUM;i++)
	{
		m_hTaskThread[i] = HPR_Thread_Create(CommitTask,this,0);
		if (HPR_INVALID_THREAD == m_hTaskThread[i])
		{
			//printf("Create CommitTask thread[%i] failed!",i);
		}
	}
	return HPR_TRUE;
}

/**	@fn	HPR_BOOL CTaskDo::Task_Stop(HPR_VOID)
*	@brief 
*	@param[in]  
*	@return	
*/
HPR_BOOL CTaskDo::Task_Stop( HPR_VOID )
{
	if (m_bRunning == HPR_TRUE)
	{
		m_bRunning = HPR_FALSE;
		HPR_SetEvent(m_hTaskEvent);
	}
		
	for (int i=0;i<MAX_THREAD_NUM;i++)
	{
		if (m_hTaskThread[i] != HPR_INVALID_THREAD)
		{
			HPR_Thread_Wait(m_hTaskThread[i]);
			m_hTaskThread[i] = HPR_INVALID_THREAD;
		}
	}
	return HPR_TRUE;
}

/**	@fn	HPR_BOOL CTaskDo::Task_Add(MSGINFO* pTask)
*	@brief 
*	@param[in] pTask 
*	@return	
*/
HPR_BOOL CTaskDo::Task_Add( TASKINFO* pTask )
{
	if (m_TaskQueue.GetSize() >= DEFAULT_QUEUE_MAX_SIZE)
	{
        return HPR_FALSE;
	}
	m_TaskQueue.Add(pTask);
    HPR_SetEvent(m_hTaskEvent);
	return HPR_TRUE;
}

/**	@fn	HPR_BOOL CTaskDo::Task_Clear(HPR_VOID)
*	@brief 
*	@param[in]  
*	@return	
*/
HPR_BOOL CTaskDo::Task_Clear( HPR_VOID )
{
	m_TaskQueue.RemoveAll();
	return HPR_TRUE;
}

/**	@fn	HPR_VOIDPTR CALLBACK CTaskDo::CommitTask(HPR_VOIDPTR lpParam)
*	@brief 
*	@param[in] lpParam 
*	@return	
*/
HPR_VOIDPTR CALLBACK CTaskDo::CommitTask( HPR_VOIDPTR lpParam )
{
	CTaskDo *pTaskCtrl = (CTaskDo *)lpParam;

	while(HPR_TRUE == pTaskCtrl->m_bRunning)
	{
		HPR_WaitForSingleObject(pTaskCtrl->m_hTaskEvent, HPR_INFINITE);
		while (HPR_TRUE)
		{
			TASKINFO *pTask = pTaskCtrl->m_TaskQueue.GetHead();
			if (pTask != NULL)
			{
				pTaskCtrl->Task_Do(pTask);
			}
			else
			{   
				//队列中的任务处理完毕，则重置事件等待下次触发
				HPR_ResetEvent(pTaskCtrl->m_hTaskEvent);
				break;
			}
		}
	}
	return NULL;
}

/**	@fn	HPR_UINT32 CTaskDo::GetQueueSize()
*	@brief 
*	@return	
*/
HPR_UINT32 CTaskDo::GetQueueSize()
{
	return m_TaskQueue.GetSize();
}

/**	@fn	HPR_BOOL CTaskDo::Task_Do(TASKINFO* OperTask)
*	@brief 
*	@param[in] OperTask 
*	@return	
*/
HPR_BOOL CTaskDo::Task_Do( TASKINFO* OperTask )
{
    FreeTaskInfo(OperTask);
	return HPR_FALSE;
}

/**	@fn	void CTaskDo::FreeTaskInfo(TASKINFO* OperTask)
*	@brief
*	@param[in] pTaskInfo 
*	@return	
*/
void CTaskDo::FreeTaskInfo( TASKINFO* OperTask )
{
    if (NULL == OperTask)
    {
		return;
    }
	delete OperTask;
	OperTask = NULL;
}
