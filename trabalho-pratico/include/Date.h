#ifndef DATE_H_
#define DATE_H_
#include <stdbool.h>

typedef struct date Date;

#define BASEDAY 9
#define BASEMONTH 10
#define BASEYEAR 2022

Date *sToDate(char *, int);
Date *sToDateSimple(char *);
int calculateAge(Date *);
int isDateBigger(Date*, Date *);
void printDate(Date *);
char *dateToS(Date *);
bool isDateValid(Date *);

#endif