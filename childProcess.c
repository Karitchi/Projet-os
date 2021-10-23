#include "random.c"
#include "childProcess.h"

void initializeLapRelativeData(sharedMemory *sharedMemory, int childId)
{
    sharedMemory->car[childId].isPitStop = 0;
    sharedMemory->car[childId].lapTime = 0;
}

void generateSectorsTimes(sharedMemory *sharedMemory, int childId)
{
    for (int i = 0; i < 3; i++)
    {
        sharedMemory->car[childId].sector[i] = generateRandomNumber(sharedMemory, 9, 40);
    }
}

void calculateLapTime(sharedMemory *sharedMemory, int childId)
{
    for (int i = 0; i < 3; i++)
    {
        sharedMemory->car[childId].lapTime += sharedMemory->car[childId].sector[i];
    }
}

void calculateTotalTime(sharedMemory *sharedMemory, int childId)
{
    for (int i = 0; i < 3; i++)
    {
        sharedMemory->car[childId].totalTime += sharedMemory->car[childId].sector[i];
    }
}

void generatePitStops(sharedMemory *sharedMemory, int childId)
{
    float random = generateRandomNumber(sharedMemory, 1, 0);

    if (random > 0.9)
    {
        sharedMemory->car[childId].isPitStop = 1;
        sharedMemory->car[childId].lapTime += generateRandomNumber(sharedMemory, 9, 5);
    }
}

void generateOut(sharedMemory *sharedMemory, int childId, int pId)
{
    float random = generateRandomNumber(sharedMemory, 1, 0);

    if (random > 0.998)
    {
        sharedMemory->car[childId].isOut = 1;
    }
}

void findBestLap(sharedMemory *sharedMemory, int childId)
{
    if (sharedMemory->car[childId].lapTime < sharedMemory->car[childId].bestLap)
    {
        sharedMemory->car[childId].bestLap = sharedMemory->car[childId].lapTime;
    }
}
