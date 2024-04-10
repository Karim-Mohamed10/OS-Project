#include "headers.h"
void clearResources(int);

struct process
{
    int arrivaltime;
    int priority;
    int runningtime;
    int id;
};
struct process *processes;
int ReadData(struct process **pprocesses)
{
    FILE *pFile;
    pFile = fopen("processes.txt", "r");
    int count = 0, i = 0; // to count the number of non-comment lines
    char data[100];
    if (pFile == NULL)
    {
        printf("error in reading the file\n");
        return -1;
    }
    else
    {
        while (fgets(data, sizeof(data), pFile) != NULL) //this while loop will terminate when we reach end of the file
        {
            if (data[0] == '#')
            {
                continue;
            }
            else
            {
                count++;
            }
        }
        fseek(pFile, 0, SEEK_SET);
        *pprocesses = (struct process *)malloc(count * sizeof(struct process));
        if (*pprocesses == NULL)
        {
            printf("Memory allocation failed\n");
            fclose(pFile);
            return -1;
        }
        while (fgets(data, sizeof(data), pFile) != NULL) //this while loop will terminate when we reach end of the file
        {
            if (data[0] == '#')
            {
                continue;
            }
            else
            {
                if (sscanf(data, "%d\t%d\t%d\t%d", &((*pprocesses)[i].id), &((*pprocesses)[i].arrivaltime), &((*pprocesses)[i].runningtime), &((*pprocesses)[i].priority)) == 4)
                {
                    i++;
                }
                else
                {
                    printf("Error parsing data.\n");
                    free(*pprocesses);
                    fclose(pFile);
                    return -1;
                }
            }
        }
        fclose(pFile);
        return count;
    }
}
int schedulerpid;
void createCLK()
{
    int pid=fork();
    if(pid==0)
    {
        execl("./clk.out", "clk.out", NULL); // fork to run clk.c
    }
  
}
void createScheduler(char *schedulerpath,char*algo,int RRquantum,int processcount)
{
    int pid=fork();
       if(pid==0)    
       {
    execl("./scheduler.out","scheduler.out",algo,RRquantum,processcount,NULL);
    }
    else
    {
        schedulerpid=pid;
    }
}

int main(int argc, char * argv[])
{
    signal(SIGINT, clearResources);
    // TODO Initialization
    // 1. Read the input files.
  
    int count= ReadData (&processes);
    printf("%d\n",count);
    for (size_t i = 0; i < count; i++)
    {
        printf("%d\n",processes[i].id);
    }
    
    // 2. Ask the user for the chosen scheduling algorithm and its parameters, if there are any.

    char algorithm[5];
    int q ;
    printf("choose algorithm HPF(1),SRTN(2),RR(3)\n");
    scanf("%s",algorithm);
    if(algorithm[0]=='3')
    {
        printf("enter the quantum\n");
        scanf("%d", &q);
    }
    // 3. Initiate and create the scheduler and clock processes.
    createCLK();
    createScheduler("./scheduler.out",algorithm,q,count);
    
      



    // 4. Use this function after creating the clock process to initialize clock
    initClk();
    // To get time use this

  while(1)
  {
    int x = getClk();
    printf("current time is %d\n", x);
    
  }


    // TODO Generation Main Loop
    // 5. Create a data structure for processes and provide it with its parameters.
    // 6. Send the information to the scheduler at the appropriate time.
    // 7. Clear clock resources
    //destroyClk(true);
}

void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption
}