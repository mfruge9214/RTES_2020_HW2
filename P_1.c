/* C Standard Libraries */
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>

/* POSIX Libraries */

#include <pthread.h>
#include <semaphore.h>


int main()
{
	/* Delclare required structures to wait for Sem */
	pid_t pChild;
	sem_t* xSem;


#ifdef SHM

	int shmID;
	/* Create shared memory location with all permissions */
	shmID = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 666);

	/* Attach the memory segment to the parent address space, which the child inherits*/

	xSem = (sem_t*) shmat(shmID, NULL, 0);
#endif

	/* Create semaphore */
	int shared = 1;
	int count = 0;

	xSem = sem_open("mysemaphore", O_CREAT, S_IRWXO, count);

	pChild = fork();

	if(pChild)
	{
		printf(" Process %d: Awaiting semaphore Post....\n", getpid());
		if(!sem_wait(xSem))
		{
			printf("Process %d: Received Semaphore \n", getpid());
		}
		else{
			printf("Process %d: Failed to get Semaphore \n", getpid());
		}
		

	} 

	if(!pChild)
	{
		printf("Process %d: Ready to Post Semaphore... \n", getpid());
		sleep(2);
		if(! sem_post(xSem))
		{
			printf("Process %d: Posted Semaphore \n", getpid());
		}
		else{

			printf("Process %d: Failed to post semaphore \n", getpid());
		}
	}

	printf("Process %d: Returning\n", getpid());
	return 0;

}