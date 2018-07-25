/*
 * sleepingTA.c
 *
 *  Created on: Apr 2, 2018
 *      Author: derek
 */


#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

pthread_mutex_t mutex_lock;

sem_t students_sem;
sem_t ta_sem;

int waiting_students = 0;


#define MAX_SLEEP_TIME 3
#define NUM_OF_STUDENTS 4
#define NUM_OF_HELPS 2
#define NUM_OF_SEATS 2
#define NUM_OF_TA 1

void *TA_Action();
void *Student_Action(void *threadID);

pthread_t *Students;
pthread_t TA;

int main(void)
{
	int id;

	printf("CS149 SleepingTA from Derek Ortega\n");

	sem_init(&ta_sem,0,0 );
	sem_init(&students_sem,0,0);

	pthread_mutex_init(&mutex_lock, NULL);

	Students = (pthread_t*) malloc(sizeof(pthread_t)*NUM_OF_STUDENTS);
	TA = (pthread_t*) malloc(sizeof(pthread_t)*NUM_OF_TA);
	pthread_create(&TA, NULL, TA_Action, NULL);

	for(id = 0; id < NUM_OF_STUDENTS; id++)
	{
		pthread_create(&Students[id], NULL, Student_Action,(void*) (long)id);
	}

	for(id = 0; id< NUM_OF_STUDENTS; id++)
	{
		pthread_join(Students[id], NULL);
	}

	pthread_cancel(TA);

	pthread_join(TA, NULL);
	free(Students);
	free(TA);
	return 1;

}

void *TA_Action()
{



	while(1)
	{
		sem_wait(&ta_sem);
		printf("TA has been awakened by student.\n");


		while(1)
		{

			//the lock for the guy
			pthread_mutex_lock(&mutex_lock);
			if(waiting_students == 0)
			{
				pthread_mutex_unlock(&mutex_lock);
				break;
			}

			pthread_mutex_lock(&mutex_lock);
			waiting_students--;
			pthread_mutex_unlock(&mutex_lock);



			printf("\t Helping the student for 3 seconds, # of waiting students %i. \n", waiting_students);
			sleep(3);
			sem_post(&students_sem);
			usleep(3000);

		}

	}
}


void *Student_Action(void* threadID)
{

	unsigned int seed = time(NULL);
	int programmingTime = 3;
	int helped = 0;

	while(1)
	{
		printf("Student %ld programming for %i seconds.\n", (long)threadID,programmingTime);
		sleep(programmingTime);


		if(waiting_students > NUM_OF_SEATS)
		{

			printf("Student %ld will return at another time. \n", (long)threadID);
			sem_wait(&students_sem);
		}
		else
		{
			pthread_mutex_lock(&mutex_lock);
			waiting_students++;
			printf("Student %ld takes a seat, # of waiting students = %i\n", (long)threadID, waiting_students);

			pthread_mutex_unlock(&mutex_lock);


			if(waiting_students == 1)
			{
				sem_post(&ta_sem);
				helped++;
				printf("Student %ld receiving help\n", (long)threadID);

				pthread_mutex_lock(&mutex_lock);
				waiting_students--;
				pthread_mutex_unlock(&mutex_lock);

				if(helped == NUM_OF_HELPS)
				{
					printf("Student %ld is finished.\n", (long)threadID);
					break;
				}
			}
			else
			{
				pthread_mutex_lock(&mutex_lock);
				waiting_students++;




				pthread_mutex_unlock(&mutex_lock);

				sem_wait(&students_sem);
			}

		}


	}
}
