/*
 * main.c
 *
 * Created: 03.11.2019 22:34:44
 *  Author: ThePetrovich
 */
#include <math.h>
#include <avr/io.h>
#include <systemd/systemd.h>
#include <initd/initd.h>
#include <kernel/kernel.h>
#include <kernel/types.h>

char exampleBuffer[32]; //Declare a buffer to hold FIFO data
kFifo_t exampleFifo; //Declare the FIFO itself
kMutex_t exampleMutex; //Mutex declaration - not required, but recommended

kTask simpleTask1(void* args)
{
	debug_logMessage(PGM_ON, L_INFO, PSTR("task1: Task 1 starts\r\n"));

	while (1) {
		char receiveBuffer[32] = "";
		uint8_t receiveBufferIndex = 0;

		debug_logMessage(PGM_ON, L_INFO, PSTR("task1: Hello from task 1!\r\n"));

		threads_mutexLock(&exampleMutex);
		/*
		 * threads_fifoAvailable - check if there is data in FIFO
		 * exampleFifo - pointer (handle) to the FIFO
		 * Returns: 1 if FIFO is not empty, otherwise 0
		 */
		while (threads_fifoAvailable(&exampleFifo)) {
			/*
			 * threads_fifoRead - read a byte from FIFO
			 * exampleFifo - pointer (handle) to the FIFO
			 * Returns: byte from FIFO if read was successful, otherwise 0
			 */
			receiveBuffer[receiveBufferIndex] = threads_fifoRead(&exampleFifo);
			receiveBufferIndex++;
		}
		debug_logMessage(PGM_ON, L_INFO, PSTR("task1: Fifo contents: %s\r\n"), receiveBuffer);
		threads_mutexUnlock(&exampleMutex);

		kernel_yield(200);
	}
}

kTask simpleTask2(void* args)
{
	debug_logMessage(PGM_ON, L_INFO, PSTR("task2: Task 2 starts\r\n"));

	while (1) {
		char receiveBuffer[32] = "";
		uint8_t receiveBufferIndex = 0;

		debug_logMessage(PGM_ON, L_INFO, PSTR("task2: Hello from task 2!\r\n"));

		threads_mutexLock(&exampleMutex);
		/*
		 * threads_fifoAvailable - check if there is data in FIFO
		 * exampleFifo - pointer (handle) to the FIFO
		 * Returns: 1 if FIFO is not empty, otherwise 0
		 */
		while (threads_fifoAvailable(&exampleFifo)) {
			/*
			 * threads_fifoRead - read a byte from FIFO
			 * exampleFifo - pointer (handle) to the FIFO
			 * Returns: byte from FIFO if read was successful, otherwise 0
			 */
			receiveBuffer[receiveBufferIndex] = threads_fifoRead(&exampleFifo);
			receiveBufferIndex++;
		}
		debug_logMessage(PGM_ON, L_INFO, PSTR("task2: Fifo contents: %s\r\n"), receiveBuffer);
		threads_mutexUnlock(&exampleMutex);

		kernel_yield(200);
	}
}

kTask simpleTask3(void* args)
{
	debug_logMessage(PGM_ON, L_INFO, PSTR("task1: Task 3 starts\r\n"));

	while (1) {
		debug_logMessage(PGM_ON, L_INFO, PSTR("task3: Hello from task 3! My favorite food is %s\r\n"), (char*)args);
		debug_logMessage(PGM_ON, L_INFO, PSTR("task3: Telling this to everyone\r\n"), (char*)args);

		threads_mutexLock(&exampleMutex);
		for (int i = 0; i < strlen((char*)args); i++) {
			/*
			 * threads_fifoWrite - write a byte to FIFO
			 * exampleFifo - pointer (handle) to the FIFO
			 * (char*)args[i] - character to send
			 * Returns: 1 if write was successful, otherwise 0
			 */
			if (!threads_fifoWrite(&exampleFifo, ((char*)args)[i])) {
				debug_logMessage(PGM_ON, L_INFO, PSTR("task3: Fifo write error\r\n"));
			}
		}
		threads_mutexUnlock(&exampleMutex);

		kernel_yield(200);
	}
}

/*
 * Init stages
 * Behaves like Arduino's void init(), but if you need to do a lot of stuff safely and in right order,
 * initd provides you with 3 init stages - user_preinit, user_init, user_postinit. Do whatever you want with them.
 */

void user_preinit()
{
	/*
	 * threads_fifoInit - Initialize FIFO (pipe)
	 * exampleBuffer - buffer to store FIFO data
	 * 32 - size of that buffer
	 * Returns: kFifo_t (a structure with all required fields set)
	 */
	exampleFifo = threads_fifoInit(exampleBuffer, 32);
	exampleMutex = threads_mutexInit(); //Initializes mutex
	return;
}

void user_init()
{

	/*
	 * kernel_createTask - Create a task
	 * simpleTask1 - task body
	 * NULL - pointer to parameter array (this task takes no parameters)
	 * 250 - amount of allocated memory in bytes
	 * 1 - task priority (where 1 is the lowest and 255 is the highest)
	 * KTASK_USER - task type, may be either KTASK_USER or KTASK_SYSTEM, defines memory protection rules
	 * "task1" - task name
	 */
	kernel_createTask(simpleTask1, NULL, 250, 1, KTASK_USER, "task1");

	/*
	 * kernel_createTask - Create a task
	 * simpleTask2 - task body
	 * NULL - pointer to parameter array (this task takes no parameters)
	 * 250 - amount of allocated memory in bytes
	 * 2 - task priority (where 1 is the lowest and 255 is the highest)
	 * KTASK_USER - task type, may be either KTASK_USER or KTASK_SYSTEM, defines memory protection rules
	 * "task2" - task name
	 */
	kernel_createTask(simpleTask2, NULL, 250, 2, KTASK_USER, "task2");

	/*
	 * kernel_createTask - Create a task
	 * simpleTask2 - task body
	 * (void*)exampleParameter - pointer to parameter array (this task takes string exampleParameter as a parameter)
	 * 250 - amount of allocated memory in bytes
	 * 3 - task priority (where 1 is the lowest and 255 is the highest)
	 * KTASK_USER - task type, may be either KTASK_USER or KTASK_SYSTEM, defines memory protection rules
	 * "task3" - task name
	 */
	static char exampleParameter[] = "Spaghetti and meatballs";
	kernel_createTask(simpleTask3, (void*)exampleParameter, 250, 3, KTASK_USER, "task3");
	return;
}

void user_postinit()
{
	//Nothing to do in post-init
	return;
}

int main()
{
	initd_startup(); //Call initd_startup to run the kernel
	while (1);
}
