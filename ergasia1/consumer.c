#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "structs.h"
#define PERMS 0666

int main (int argc,char* argv[]){
  int my_id=getpid();
  int shm,semaph_id,arithmos_paidiou,semaphoros_counting,Semaph_id_counting,Semaph_id,shmid; //shm(id thw shared memory)  & semaph_id(id tou kathe semaphore)
  integer *shmemory;
  int i,semaphoros,l,M,N;
  long double  xronos,kathisterisi,average;
  long double sum=0.000000000000000000;
  M=atoi(argv[5]);
  N=atoi(argv[6]);
  int Copied_Table[M];
  FILE *f=fopen("apotelesmata.txt","w");
  if (f==NULL){
    perror("Error opening file\n");
    exit(1);
  }

    //////functions of semaphore//////
    int up(int*semaphore,int x)
    {
      struct sembuf up[1];
      up->sem_num=x;
      up->sem_op=1;
      up->sem_flg=SEM_UNDO;
      semop(*semaphore,up,1);
      return 0;
    }

    int down(int*semaphore,int x)
    {
      struct sembuf down[1];
      down->sem_num=x;
      down->sem_op=-1;
      down->sem_flg=SEM_UNDO;
      semop(*semaphore,down,1);
        return 0;
    }
    ///////////////////////////////////////////////////
    int up_semaphore_counting(int *semaphore)
    {
      struct sembuf up[1];
      up->sem_num=0;
      up->sem_op=1;
      up->sem_flg=SEM_UNDO;
      semop(*semaphore,up,1);
    }



  ///critical section////
  shm=atoi(argv[1]);// pairnw to key ths shared memory
  arithmos_paidiou=atoi(argv[2]);
  int count=1;
  shmid=shmget(shm,count*sizeof(integer),IPC_EXCL|PERMS);
  shmemory = (integer*)shmat(shmid,NULL,0); //attachment
  if (shmid<0)
  {
    printf("shmget failed fjekgndnvdvdjved!\n"); //if shmget fail ,end the programm
    return(-1);
  }


  semaphoros=atoi(argv[3]);//semaphoros einai to key twn semaphorwn tou pinaka
  Semaph_id=semget(semaphoros,N,IPC_EXCL);//id semaphorwn tou pinaka
  if(Semaph_id==-1)
  {
    perror("semget failed twn paidiwn\n");
    return(-1);
  }


  semaphoros_counting=atoi(argv[4]);//semaphoros_counting einai to key tou counting semaphore
  Semaph_id_counting=semget(semaphoros_counting,1,IPC_EXCL);
  if(Semaph_id_counting==-1)
  {
    perror("semget failed tou counting\n");
    return(-1);
  }


////critical section///////
  for(l=0;l<M;l++)
  {
    down(&Semaph_id,arithmos_paidiou);
    Copied_Table[l]=shmemory->number;
    kathisterisi=time(NULL)-(shmemory->xronosfragida);
    sum=sum+kathisterisi;
    up_semaphore_counting(&Semaph_id_counting);

  }
///////////////////////////
  average=(sum)/(double)M;
  printf("\nconsumer noumero %d me id: %d kai exw average :%Lf \n",arithmos_paidiou,my_id,average);
  fprintf(f,"consumer noumero %d me id: %d kai exw average :%Lf \n",arithmos_paidiou,my_id,average);
  for(l=0;l<M;l++){
    fprintf(f,"%d, ",Copied_Table[l]);
  }
  return 0;
}
