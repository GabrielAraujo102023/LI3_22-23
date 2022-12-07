#include "User.h"
#include "Date.h"
#include "Ride.h"
#include <string.h>
#include "ArrayList.h"
#include <glib.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct user
{

    char *username;
    char *name;
    char gender;
    Date *birthdate;
    Date *account_creation;
    char *pay_method;
    bool account_status;
    int age;
    int account_age;
    double avgScore;
} User;

static ArrayList *list = NULL;
static GHashTable *positions = NULL;

void initListUser(int size)
{
    if (!list)
        list = createAL(size - 1, sizeof(User *));
    if (!positions)
        positions = g_hash_table_new(g_str_hash, g_str_equal);
}

void loadUser(char *sp)
{
    User *user = (User *)malloc(sizeof(User));
    char *aux;
    user->username = strdup(strsep(&sp, ";"));
    if(strlen(user->username) == 0)
    {
        free(user->username);
        free(user);
        return;
    }
    user->name = strdup(strsep(&sp, ";"));
    if(strlen(user->name) == 0)
    {
        free(user->username);
        free(user->name);
        free(user);
        return;
    }
    user->gender = strsep(&sp, ";")[0];
    if(user->gender == '\0')
    {
        free(user->username);
        free(user->name);
        free(user);
        return;
    }
    aux = strdup(strsep(&sp, ";"));
    user->birthdate = sToDate(aux, strlen(aux));
    if(!user->birthdate)
    {
        free(user->username);
        free(user->name);
        free(user);
        free(aux);
        return;
    }
    free(aux);
    aux = strdup(strsep(&sp, ";"));
    user->account_creation = sToDate(aux, strlen(aux));
    if(!user->account_creation)
    {
        free(user->username);
        free(user->name);
        free(user->birthdate);
        free(user);
        free(aux);
        return;   
    }
    free(aux);
    user->pay_method = strdup(strsep(&sp, ";"));
    if(strlen(user->pay_method) == 0)
    {
        free(user->username);
        free(user->name);
        free(user->birthdate);
        free(user->account_creation);
        free(user->pay_method);
        free(user);
        return;   
    }
    aux = strdup(strsep(&sp, "\n"));
    for(int i = 0; i < strlen(aux); i++)
    {
        aux[i] = tolower(aux[i]);
    }
    user->account_status = strcmp(aux, "active") == 0 ? true : false;
    user->age = calculateAge(user->birthdate);
    user->account_age = calculateAge(user->account_creation);
    user->avgScore = -1;
    g_hash_table_insert(positions, user->username, user);
    addAL(list, user);
    free(aux);
}

User *findUserByUsername(char *username)
{
    return (User *)g_hash_table_lookup(positions, username);
}

gboolean isUserActive(char *username)
{
    return findUserByUsername(username)->account_status;
}

char *getUserBasicInfo(char *id)
{

    User *user = findUserByUsername(id);
    char aux[10];
    char *r = (char *) malloc (256);
    strcpy(r,user->name);
    strcat(r, ";");
    strncat(r, &user->gender, 1);
    strcat(r, ";");
    sprintf(aux, "%d", user->age);
    strcat(r, aux);
    strcat(r, ";");
    return r;
}

double getUserAvgScore(char *id)
{

    User *user = findUserByUsername(id);

    if (user->avgScore == -1)
    {
        user->avgScore = calculateUserAvgScore(id);
    }

    return user->avgScore;
}

double *getUserAvgScoreAndPay(char *id)
{

    User *user = findUserByUsername(id);

    if (user->avgScore == -1)
    {
        return calculateUserAvgScoreAndPay(id);
    }

    double *values = (double *)malloc(sizeof(double) * 2);
    values[0] = user->avgScore;
    values[1] = calculateTotalPayUser(id);
    return values;
}

void _freeUser(void *u)
{
    User *user = (User *) u;
    free(user->account_creation);
    free(user->birthdate);
    free(user->name);
    free(user->username);
    free(user->pay_method);
    free(user);
}

void freeUser()
{
    freeArrayList(list, _freeUser);
    g_hash_table_destroy(positions);
}

LinkedList *getUserGenderAccAgeName(char *username)
{
    User *user = findUserByUsername(username);
    LinkedList *r = createLL();
    addLL(r, &user->gender);
    addLL(r, &user->account_age);
    addLL(r, user->name);
    return r;
}

int getUserAccAge(char *username)
{
    return findUserByUsername(username)->account_age;
}