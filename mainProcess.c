#include "mainProcess.h"

void *createSharedMemory(sharedMemory *sharedMemory, int shmId, int key)
{
    shmId = shmget(key, sizeof(*sharedMemory), IPC_CREAT | 0666);
    return shmat(shmId, NULL, 0);
}

void initializeGPRelativeData(sharedMemory *sharedMemory, int *carNumbers)
{
    for (int i = 0; i < 20; i++)
    {
        sharedMemory->car[i].isEliminated = 0;
        sharedMemory->car[i].carNumber = carNumbers[i];
    }
}

void choseRace(int *chosenRace, int *timeOfRace)
{
    system("clear");
    printf("Essai 1 : 1\n");
    printf("Essai 2 : 2\n");
    printf("Essai 3 : 3\n");
    printf("\n");
    printf("Qualification 1 : 4\n");
    printf("Qualification 2 : 5\n");
    printf("Qualification 3 : 6\n");
    printf("\n");
    printf("Course : 7\n");
    printf("\n");
    printf("Quitter : 0\n");
    printf("\n");
    printf("Choix : ");

    scanf("%d", chosenRace);

    switch (*chosenRace)
    {
    case 0:
        exit(0);
        break;
    case 1:
    case 2:
        *timeOfRace = 5400;
        break;

    case 3:
        *timeOfRace = 3600;
        break;
    case 4:
        *timeOfRace = 1080;
        break;
    case 5:
        *timeOfRace = 900;
        break;
    default:
        *timeOfRace = 720;
    }
}

void initializeRaceRelativeData(sharedMemory *sharedMemory)
{
    for (int i = 0; i < 20; i++)
    {
        sharedMemory->numberOfCarsFinished = 0;
        sharedMemory->car[i].totalTime = 0;
        sharedMemory->car[i].isOut = 0;
        sharedMemory->car[i].bestLap = INFINITY;
    }
}

void generateChilds(int *pId, int *childId)
{
    for (int i = 0; i < 20; i++)
    {
        *pId = fork();

        if (!*pId)
        {
            *childId = i;
            break;
        }
    }
}

void initializeBestSectors(sharedMemory *sharedMemory)
{
    for (int i = 0; i < 3; i++)
    {
        sharedMemory->bestSectorTimes[i] = INFINITY;
    }
}

void findBestSectors(sharedMemory *sharedMemory)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            if (sharedMemory->car[j].sector[i] < sharedMemory->bestSectorTimes[i])
            {
                sharedMemory->bestSectorTimes[i] = sharedMemory->car[j].sector[i];
                sharedMemory->bestSectorCars[i] = sharedMemory->car[j].carNumber;
            }
        }
    }
}

void sortCarsByBestLap(sharedMemory *sharedMemory)
{
    int i, j;
    struct car temp;
    for (i = 0; i < (20 - 1); ++i)
    {
        for (j = 0; j < (20 - 1 - i); ++j)
        {
            if (sharedMemory->car[j].bestLap > sharedMemory->car[j + 1].bestLap)
            {
                temp = sharedMemory->car[j + 1];
                sharedMemory->car[j + 1] = sharedMemory->car[j];
                sharedMemory->car[j] = temp;
            }
        }
    }
}

int display(sharedMemory *sharedMemory)
{
    system("clear");
    printf("|------------------------------------------------------------------------------------------------------|\n");
    printf("| car  | sector 1 | sector 2 | sector 3 | lap time  | best lap time  | total time  | pit stop(s) | out |\n");
    for (int i = 0; i < 20; i++)
    {
        if (sharedMemory->car[i].isEliminated)
        {
            printf("|------------------------------------------------------------------------------------------------------|\n");
            printf(RED "|  %2d  |  %3.3f  |  %3.3f  |  %3.3f  |  %3.3f  |    %3.3f     |  %8.3f   |     %2d      |  %d  |\n" RESET,
                   sharedMemory->car[i].carNumber,
                   sharedMemory->car[i].sector[0],
                   sharedMemory->car[i].sector[1],
                   sharedMemory->car[i].sector[2],
                   sharedMemory->car[i].lapTime,
                   sharedMemory->car[i].bestLap,
                   sharedMemory->car[i].totalTime,
                   sharedMemory->car[i].isPitStop,
                   sharedMemory->car[i].isOut);
        }
        else if (sharedMemory->car[i].isOut)
        {
            printf("|------------------------------------------------------------------------------------------------------|\n");
            printf(YEL "|  %2d  |  %3.3f  |  %3.3f  |  %3.3f  |  %3.3f  |    %3.3f     |  %8.3f   |     %2d      |  %d  |\n" RESET,
                   sharedMemory->car[i].carNumber,
                   sharedMemory->car[i].sector[0],
                   sharedMemory->car[i].sector[1],
                   sharedMemory->car[i].sector[2],
                   sharedMemory->car[i].lapTime,
                   sharedMemory->car[i].bestLap,
                   sharedMemory->car[i].totalTime,
                   sharedMemory->car[i].isPitStop,
                   sharedMemory->car[i].isOut);
        }
        else if (sharedMemory->car[i].isPitStop)
        {
            printf("|------------------------------------------------------------------------------------------------------|\n");
            printf(BLU "|  %2d  |  %3.3f  |  %3.3f  |  %3.3f  |  %3.3f  |    %3.3f     |  %8.3f   |     %2d      |  %d  |\n" RESET,
                   sharedMemory->car[i].carNumber,
                   sharedMemory->car[i].sector[0],
                   sharedMemory->car[i].sector[1],
                   sharedMemory->car[i].sector[2],
                   sharedMemory->car[i].lapTime,
                   sharedMemory->car[i].bestLap,
                   sharedMemory->car[i].totalTime,
                   sharedMemory->car[i].isPitStop,
                   sharedMemory->car[i].isOut);
        }
        else if (i == 0)
        {
            printf("|------------------------------------------------------------------------------------------------------|\n");
            printf(GRN "|  %2d  |  %3.3f  |  %3.3f  |  %3.3f  |  %3.3f  |    %3.3f     |  %8.3f   |     %2d      |  %d  |\n" RESET,
                   sharedMemory->car[i].carNumber,
                   sharedMemory->car[i].sector[0],
                   sharedMemory->car[i].sector[1],
                   sharedMemory->car[i].sector[2],
                   sharedMemory->car[i].lapTime,
                   sharedMemory->car[i].bestLap,
                   sharedMemory->car[i].totalTime,
                   sharedMemory->car[i].isPitStop,
                   sharedMemory->car[i].isOut);
        }
        else
        {
            printf("|------------------------------------------------------------------------------------------------------|\n");
            printf("|  %2d  |  %3.3f  |  %3.3f  |  %3.3f  |  %3.3f  |    %3.3f     |  %8.3f   |     %2d      |  %d  |\n",
                   sharedMemory->car[i].carNumber,
                   sharedMemory->car[i].sector[0],
                   sharedMemory->car[i].sector[1],
                   sharedMemory->car[i].sector[2],
                   sharedMemory->car[i].lapTime,
                   sharedMemory->car[i].bestLap,
                   sharedMemory->car[i].totalTime,
                   sharedMemory->car[i].isPitStop,
                   sharedMemory->car[i].isOut);
        }
    }
    printf("|------------------------------------------------------------------------------------------------------|\n\n");
    printf("|--------------------------------------------------------|\n");
    printf("| best sector 1    | best sector 2    | best sector 3    |\n");
    printf("|--------------------------------------------------------|\n");
    for (int i = 0; i < 3; i++)
    {
        printf("|  %2d : %.3f     ",
               sharedMemory->bestSectorCars[i],
               sharedMemory->bestSectorTimes[i]);
    }
    printf("|\n|--------------------------------------------------------|\n");
}

void eliminate5LastCars(sharedMemory *sharedMemory)
{
    for (int i = 20; i > 14; i--)
    {
        sharedMemory->car[i].isEliminated = 1;
    }
}