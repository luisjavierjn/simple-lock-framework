#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include "CriticalSectionEx.h"

#define THREADCOUNT 12
BaseLock *pLock;
DWORD WINAPI ThreadProc( LPVOID );

int _tmain(int argc, _TCHAR **argv)
{
	_tprintf(_T("Example"));

	 HANDLE aThread[THREADCOUNT];
	 DWORD ThreadID;
	 int i;

	
	pLock=new CriticalSectionEx();; 
	
	// Create worker threads

	for( i=0; i < THREADCOUNT; i++ )
	{
		aThread[i] = CreateThread( 
			NULL,       // default security attributes
			0,          // default stack size
			(LPTHREAD_START_ROUTINE) ThreadProc, 
			NULL,       // no thread function arguments
			0,          // default creation flags
			&ThreadID); // receive thread identifier

		if( aThread[i] == NULL )
		{
			printf("CreateThread error: %d\n", GetLastError());
			return 1;
		}
	}

	// Wait for all threads to terminate

	WaitForMultipleObjects(THREADCOUNT, aThread, TRUE, INFINITE);

	// Close thread and semaphore handles

	for( i=0; i < THREADCOUNT; i++ )
		CloseHandle(aThread[i]);

	delete pLock;

	while(1){}
	return 0;
}

DWORD WINAPI ThreadProc( LPVOID lpParam )
{

	// lpParam not used in this example
	UNREFERENCED_PARAMETER(lpParam);
	BOOL bContinue=TRUE;

	while(bContinue)
	{
		LockObj lock(pLock);
		printf("Thread %d: wait succeeded\n", GetCurrentThreadId());
		bContinue=FALSE;            
		break; 
	}
	return TRUE;
}
