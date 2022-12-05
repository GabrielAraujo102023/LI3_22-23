#include "Driver.h"
#include <string.h>
#include <stdlib.h>
#include "ArrayList.h"
#include "Ride.h"

typedef struct driver
{
    char *id;
    char *name;
    Date *birth_day;
    char gender;
    char *car_class;
    char *license_plate;
    char *city;
    Date *account_creation;
    bool account_status;
    int age;
    int account_age;
    double avgScore;
} Driver;

static ArrayList *list = NULL;

void initListDriver(int size)
{
    if (!list)
        list = createAL(size - 1, sizeof(Driver *));
}

void loadDriver(char *sp)
{
    Driver *driver = (Driver *)malloc(sizeof(Driver));
    driver->id = strdup(strsep(&sp, ";"));
    driver->name = strdup(strsep(&sp, ";"));
    driver->birth_day = sToDate(strsep(&sp, ";"));
    driver->gender = strsep(&sp, ";")[0];
    driver->car_class = strdup(strsep(&sp, ";"));
    driver->license_plate = strdup(strsep(&sp, ";"));
    driver->city = strdup(strsep(&sp, ";"));
    driver->account_creation = sToDate(strsep(&sp, ";"));
    driver->account_status = strcmp(strsep(&sp, "\n"), "active") == 0 ? true : false;
    driver->age = calculateAge(driver->birth_day);
    driver->account_age = calculateAge(driver->account_creation);
    driver->avgScore = -1;
    addAL(list, driver);
}

Driver *findDriverByID(char *id)
{
    return (Driver *)getByIndex(list, atoi(id) - 1);
}

bool isDriverActive(char *id)
{
    return findDriverByID(id)->account_status;
}

char *getDriverBasicInfo(char *id)
{
    Driver *driver = findDriverByID(id);
    char aux[100];
    char *r = driver->name;
    strcat(r, ";");
    strncat(r, &driver->gender, 1);
    strcat(r, ";");
    sprintf(aux, "%d", driver->age);
    strcat(r, aux);
    strcat(r, ";");
    return r;
}

double getDriverAvgScore(char *id)
{
    Driver *driver = findDriverByID(id);
    if (driver->avgScore == -1)
    {
        driver->avgScore = calculateDriverAvgScore(id);
    }
    return driver->avgScore;
}

double *getDriverAvgScoreAndPay(char *id)
{
    Driver *driver = findDriverByID(id);
    if (driver->avgScore == -1)
        return calculateDriverAvgScoreAndPay(id);

    double *values = (double *)malloc(sizeof(double) * 2);
    values[0] = driver->avgScore;
    values[1] = calculateTotalPayDriver(id);
    return values;
}

char *getCarClass(char *id)
{
    return findDriverByID(id)->car_class;
}

void _freeDriver(void *d)
{
    Driver *driver = (Driver *)d;
    free(driver->birth_day);
    free(driver->account_creation);
    free(driver->id);
    free(driver->car_class);
    free(driver->city);
    free(driver->license_plate);
    free(driver->name);
    free(driver);
}

void freeDriver()
{
    freeArrayList(list, _freeDriver);
}

int getDriverAccAge(char *id)
{
    return findDriverByID(id)->account_age;
}

char *getDriverName(char *id)
{
    return findDriverByID(id)->name;
}

int compareDriversByScore(const void *A, const void *B)
{
    Driver *a = *(Driver **)A;
    Driver *b = *(Driver **)B;
    
    if(a->avgScore == -1)
        a->avgScore = calculateDriverAvgScore(a->id);
    if (b->avgScore == -1)
        b->avgScore = calculateDriverAvgScore(b->id);

    if (a->avgScore <= b->avgScore)
        return 1;
    if (a->avgScore == b->avgScore)
        return mostRecentRide(a->id, b->id);
    return -1;
}

char **topNdrivers(int n)
{
    ArrayList* temp = copyAL(list, sizeof(Driver));
    quickSortArrayList(temp, sizeof(Driver *), compareDriversByScore);
    char **r = malloc(sizeof(char *) * n);
    char aux[10];
    Driver *driver;
    for (int i = 0; i < n; i++)
    {
        driver = (Driver *)getByIndex(temp, i);
        r[i] = (char *)malloc(strlen(driver->id) + strlen(driver->name) + 15);
        sprintf(aux, "%.3lf", driver->avgScore);
        strcpy(r[i], driver->id);
        strcat(r[i], ";");
        strcat(r[i], driver->name);
        strcat(r[i], ";");
        strcat(r[i], aux);
    }
    freeArrayListSimple(temp);
    return r;
}