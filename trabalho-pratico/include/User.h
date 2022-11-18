#ifndef USER_H_
#define USER_H_

#include <stdbool.h>
#include "Date.h"
#include <stdio.h>
#include "LinkedList.h"

typedef struct user User;
void loadUser(char *);
int getElementSizeUser();
User *findUserByUsername(char *);
gboolean isUserActive(char *);
void initListUser(int);
char *getUserBasicInfo(char *);
double getUserAvgScore(char *);
double *getUserAvgScoreAndPay(char *);
void freeUser();
char *userGenderAndAccountAge(char *, char, int);
int getUserAccAge(char *);
LinkedList *getUserGenderAccAgeName(char *);
#endif