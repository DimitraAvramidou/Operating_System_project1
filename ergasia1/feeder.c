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

#define SHMKEY (key_t)8246
#define SEMKEY (key_t)85462
#define SEMKEY_COUNTING (key_t)44863
#define PERMS 0666

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

int down_sem_counting(int*semaphore,int y)
{
  struct sembuf down[1];
  down->sem_num=0;
  down->sem_op=-y;
  down->sem_flg=SEM_UNDO;
  semop(*semaphore,down,1);
    return 0;
}
///////////////////////////////////////////////////

int main(int argc,char* argv[]){
  int M=atoi(argv[1]);
  int N=atoi(argv[2]);
  if(M<=2999)
  {
   printf("dwse arithmo megalutero tou 3000\n");
   return 0;
  }
   srand(time(NULL));
   integer *shmemory;
   printf("to plithos tou pinaka einai : %d\n",M);
   printf("to plithos twn paidiwn pou tha dimiourgithoun einai : %d\n",N);
   pid_t pid;
   int i,j;
   int wait_table[N]; //ta pid twn paidiwn pou tha perimenei
   int P[M]; //pinakas me tous tuxaious arithous(tou feeder)
   int Semaph_id; ///pinakas semaphorwn
   int Semaph_id_counting; // counting semaphore id

   for(i=0;i<M;i++){ //dimiourgia pinaka FEEDER
     P[i] =rand();

   }
   ////////////////////////////////////////////////////////////
   /////Shared memory-->creation,initialization,attachment/////
   int shmid,k;
   int count=1;
   /////create the shared memory////////
   shmid=shmget(SHMKEY,count*sizeof(integer),IPC_CREAT|PERMS);
   if (shmid<0)
   {
     perror("shmget failed!\n"); //if shmget fail ,end the programm
     return(-1);
   }
   shmemory = (integer*)shmat(shmid,NULL,0);//attachment
   ////create a semapfore table (megethos=paidia) and initialization////
   union semun{
     int val;
     struct semid_ds *buf;
     ushort *array;
   }arg;
   arg.val=0;
      //////////create semaphore table/////////////////////////////////////////
     Semaph_id=semget(SEMKEY,N,IPC_CREAT|PERMS);
     if(Semaph_id==-1)
     {
       perror("semget failed edw\n");
       return(-1);
     }
     for(i=0;i<N;i++)
     {
       int u=semctl(Semaph_id,i,SETVAL,arg.val);//arxikopoiw ton kathe ena sto 0
       if(u==-1)
       {
         printf("semctl failed \n");
         return(-1);
       }
     }
     ///////////////create counting semaphore/////////////////////////////////
     arg.val=N;
     Semaph_id_counting=semget(SEMKEY_COUNTING,1,IPC_CREAT|PERMS);
     if(Semaph_id_counting==-1)
     {
       printf("semget failed\n");
       return(-1);
     }
     int u=semctl(Semaph_id_counting,0,SETVAL,arg.val); //ton arxikopoiw se N=osa kai ta paidia
     if(u==-1)
     {
       printf("semctl f2ailed\n");
       return(-1);
     }
     //////////////////////////////////////////////////////////////////////////
   ///metatrepw to KEY ths shared memory se string gia na to perasw san orisma sthn exec/////
    int s=SHMKEY;
    char *y;
    y=malloc(sizeof(char)*100);
    sprintf(y,"%d",s);
    /////////////////////////////////////////////////////////////////

    ///metatrepw  se string to plithos twn paidiwn gia na to perasw san orisma sthn exec/////
     int t=N;
     char *plithos_paidiwn;
     plithos_paidiwn=malloc(sizeof(char)*100);
     sprintf(plithos_paidiwn,"%d",t);
     /////////////////////////////////////////////////////////////////

     ///metatrepw se string to megethos tou pinaka gia na to perasw san orisma sthn exec/////
      int p=M;
      char *megethos_pinaka;
      megethos_pinaka=malloc(sizeof(char)*100);
      sprintf(megethos_pinaka,"%d",p);
      /////////////////////////////////////////////////////////////////

  //////metatrepw se string ton arithmo tou paidiou gia na to perasw san orisma sthn exec/////////
  char *arithmos_paidiou;
  arithmos_paidiou=malloc(sizeof(char)*12);
  ////////////////////////////////////////////////////////////////////////////////////////////////

/////////////metatrepw to KEY tou counting semaphore se string gia na to perasw sthn exec//////////
int sem_id_counting=SEMKEY_COUNTING;
char *semaphoros_counting;
semaphoros_counting=malloc(sizeof(char)*100);
sprintf(semaphoros_counting,"%d",sem_id_counting);
////////////////////////////////////////////////////////////////////////

//////metatrepw se string to KEY tou semaphore twn paidiwn gia na to perasw san orisma sthn exec////
int sem_id=SEMKEY;
char *semaphoros;
semaphoros=malloc(sizeof(char)*100);
sprintf(semaphoros,"%d",sem_id);
////////////////////////////////////////////////////////////////////////////////////////////////////

   for(i=0; i<N; i++){
     pid=fork();
     if (pid == 0)
     {
      int temp=i;
      sprintf(arithmos_paidiou,"%d",temp);//pernaw ton arithmo tou paidiou

      ////////////////////////////////////////////////////////////////////////////////////////////////
       char* as[]={"./consumer",y,arithmos_paidiou,semaphoros,semaphoros_counting,megethos_pinaka,plithos_paidiwn,NULL};
       execv("./consumer",as);
       perror("fail to exec\n");
     }
     else if (pid<0)
     {
       perror("fail to fork\n");
     }
     else
     {
        wait_table[i]=pid;
     }
   }
//feeder
for(i=0;i<M;i++)
{
  down_sem_counting(&Semaph_id_counting,N);
  shmemory->number=P[i];
  shmemory->xronosfragida=time(NULL);
  //printf("P[i] %d\n",P[i]);
  for(j=0;j<N;j++)
  {
      up(&Semaph_id,j);
  }

}
shmdt(shmemory); //detach
for(i=0;i<N;i++) //remove the table of semaphores
{
  semctl(Semaph_id,0,IPC_RMID);
}
semctl(Semaph_id_counting,0,IPC_RMID); //remove the counting semaphore

  for (i=0;i<N;i++)//wait children
  {

     waitpid(wait_table[i],NULL,0);
   }
   return 0;
}
