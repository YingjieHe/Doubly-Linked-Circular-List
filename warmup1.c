#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <locale.h>
#include <monetary.h>
#include<sys/stat.h>

#include "cs402.h"

#include "my402list.h"

typedef struct tagMy402ListObj {
	char Transcation_type;
	int Transcation_time;
	double Transcation_amount;
	char *Transcation_description;
} My402ListObj;

/* ----------------------- Print() ----------------------- */

/*
static
void PrintList1(My402List *pList)
{
	My402ListElem *elem=NULL;
	My402ListObj *temp = NULL;
    for (elem=My402ListFirst(pList); elem != NULL; elem=My402ListNext(pList, elem))
    {
    	temp = (My402ListObj *)elem->obj;
    	fprintf(stdout, "%d ", temp->Transcation_time);
    	fprintf(stdout, "%s ", temp->Transcation_description);
    	fprintf(stdout, "%.2f \n", temp->Transcation_amount);
    }
    fprintf(stdout, "\n");
}
*/


static
void PrintList(My402List *pList)
{
    My402ListElem *elem=NULL;
    My402ListObj *temp = NULL;
    double balance = 0.0, amount = 0.0;
    char *Balance = (char*)malloc(sizeof(char)*20), *Amount = (char*)malloc(sizeof(char)*20);
    char *space1 = (char*)malloc(sizeof(char)*20), *space2 = (char*)malloc(sizeof(char)*20);
    time_t time;
	char *time_trans = NULL;
	char *date = (char*)malloc(sizeof(char)*17);
	char *description = (char*)malloc(sizeof(char)*25);

    fprintf(stdout, "+-----------------+--------------------------+----------------+----------------+\n");
    fprintf(stdout, "|       Date      | Description              |         Amount |        Balance |\n");
    fprintf(stdout, "+-----------------+--------------------------+----------------+----------------+\n");
    for (elem=My402ListFirst(pList); elem != NULL; elem=My402ListNext(pList, elem))
    {
    	temp = (My402ListObj *)elem->obj;

    	amount = temp->Transcation_amount;
    	setlocale(LC_NUMERIC, "en_US");
    	memset(Balance, '\0', sizeof(Balance));
       	memset(Amount, '\0', sizeof(Amount));
       	memset(space1, '\0', sizeof(space1));
       	memset(space2, '\0', sizeof(space2));
       	sprintf(Amount,"%'.2f",amount);
    	if(temp->Transcation_type == '-')
    	{
    		balance -= amount;
    		amount = -amount;
    	}
    	else
    	{
    		balance += amount;
    	}
    	if(balance < 0.0)
    	{
    		sprintf(Balance,"%'.2f",-balance);
    	}
    	else
    	{
    		sprintf(Balance,"%'.2f",balance);
    	}

    	memset(date, '\0', sizeof(date));
    	time = temp->Transcation_time;
    	time_trans = ctime(&time);
    	char *token = NULL;
    	int i = 0;
    	token = strtok(time_trans, " ");
    	while(token != NULL)
    	{
    		if(i == 2 && strlen(token) < 2)
    		{
       			strcat(date, " ");
    		}
    		if(strlen(token) <= 5)
    		{
				strncat(date, token, 4);
				strcat(date, " ");
    		}
    		token = strtok(NULL, " ");
    		i++;
    	}

    	memset(description, '\0', sizeof(description));
    	strncpy(description, temp->Transcation_description, 24);
    	if(strlen(description) < 24)
    	{
    		strncat(description, "                        ", 24 - strlen(description));
    	}


    	if(balance >= 1000000.00 || balance <= -1000000.00)
    	{
    		strcpy(Balance, "?,???,???.??");
    	}
    	if(balance > 0.0)
    	{
   			strncat(space2, "              ", 13 - strlen(Balance));
    	}
    	else if(balance < 0.0)
    	{
   			strncat(space2, "              ", 12 - strlen(Balance));
    	}

    	if(amount >= 1000000.00 || amount <= -1000000.00)
    	{
    		strcpy(Amount, "?,???,???.??");
    	}
    	if(amount > 0.0)
    	{
   			strncat(space1, "              ", 13 - strlen(Amount));
    	}
    	else if(amount < 0.0)
    	{
   			strncat(space1, "              ", 12 - strlen(Amount));
    	}

    	if(temp->Transcation_type == '-' && balance < 0)
    	{
    		fprintf(stdout, "| %s| %s | (%s%s) | (%s%s) |\n", date, description, space1, Amount, space2, Balance);
    	}
    	else if(temp->Transcation_type == '-' && balance > 0)
    	{
    	    fprintf(stdout, "| %s| %s | (%s%s) | %s%s  |\n", date, description, space1, Amount, space2, Balance);
    	}
    	else if(temp->Transcation_type == '+' && balance > 0)
    	{
        	fprintf(stdout, "| %s| %s | %s%s  | %s%s  |\n", date, description, space1, Amount, space2, Balance);
    	}
    	else
    	{
    		fprintf(stdout, "| %s| %s | %s%s  | (%s%s) |\n", date, description, space1, Amount, space2, Balance);
    	}
    }
    fprintf(stdout, "+-----------------+--------------------------+----------------+----------------+\n");
    //fprintf(stdout, "\n");
}

/* ----------------------- Sort() ----------------------- */
/*
 * Used from listtest.c provided by instructor
 */

static
void BubbleForward(My402List *pList, My402ListElem **pp_elem1, My402ListElem **pp_elem2)
    /* (*pp_elem1) must be closer to First() than (*pp_elem2) */
{
    My402ListElem *elem1=(*pp_elem1), *elem2=(*pp_elem2);
    void *obj1=elem1->obj, *obj2=elem2->obj;
    My402ListElem *elem1prev=My402ListPrev(pList, elem1);
/*  My402ListElem *elem1next=My402ListNext(pList, elem1); */
/*  My402ListElem *elem2prev=My402ListPrev(pList, elem2); */
    My402ListElem *elem2next=My402ListNext(pList, elem2);

    My402ListUnlink(pList, elem1);
    My402ListUnlink(pList, elem2);
    if (elem1prev == NULL) {
        (void)My402ListPrepend(pList, obj2);
        *pp_elem1 = My402ListFirst(pList);
    } else {
        (void)My402ListInsertAfter(pList, obj2, elem1prev);
        *pp_elem1 = My402ListNext(pList, elem1prev);
    }
    if (elem2next == NULL) {
        (void)My402ListAppend(pList, obj1);
        *pp_elem2 = My402ListLast(pList);
    } else {
        (void)My402ListInsertBefore(pList, obj1, elem2next);
        *pp_elem2 = My402ListPrev(pList, elem2next);
    }
}

static
void BubbleSortForwardList(My402List *pList)
{
    My402ListElem *elem=NULL;
    My402ListObj *temp1 = NULL;
    My402ListObj *temp2 = NULL;
    int i=0;
    int num_items = My402ListLength(pList);
    for (i=0; i < num_items; i++) {
        int j=0, something_swapped=FALSE;
        My402ListElem *next_elem=NULL;

        for (elem=My402ListFirst(pList), j=0; j < num_items-i-1; elem=next_elem, j++) {
        	temp1 = (My402ListObj *)elem->obj;
        	int cur_val=temp1->Transcation_time, next_val=0;

            next_elem=My402ListNext(pList, elem);
            temp2 = (My402ListObj *)next_elem->obj;
            next_val = temp2->Transcation_time;
            if(cur_val == next_val)
            {
            	fprintf(stderr, "Error: Duplicate time.\n");
            	exit(0);
            }
            else if (cur_val > next_val) {
                BubbleForward(pList, &elem, &next_elem);
                something_swapped = TRUE;
            }
        }
        if (!something_swapped) break;
    }
}

/* ----------------------- Process() ----------------------- */


static
double atod(char *buf)
{
	if(buf == NULL || strlen(buf) == 0) return 0.0;
	char *token = NULL;
	token = strtok(buf, ".");
	int i = 0;
	double amount = 0.0;
	while(token != NULL)
	{
		if(i > 1)
		{
			fprintf(stderr, "Error: Invalid amount.\n");
			exit(0);
		}
		if(i == 0)
		{
			if(strlen(token) > 7)
			{
				fprintf(stderr, "Error: Invalid amount, amount is greater than 1000,0000.\n");
				exit(0);
			}
			amount += atoi(token);
		}
		if(i == 1)
		{
			if(strlen(token) != 2)
			{
				fprintf(stderr, "Error: Invalid amount, more than two digits after decimal point.\n");
				exit(0);
			}
			amount += atoi(token) / 100.0;
		}
		token = strtok(NULL, ".");
		i++;
	}
	return amount;
}


static
char *trimspace(char * buf)
{
	while(isspace(*buf)) buf++;
	char *end = buf + strlen(buf) - 1;
	while(end >= buf && isspace(*end)) end--;
	*(end + 1) = 0;
	return buf;
}


static
void Process(FILE *file, My402List *list)
{
	char buffer[1026];
	while(fgets(buffer, 1026, file) != NULL)
	{
		if(strlen(buffer) > 1024)
		{
			fprintf(stderr, "Error: Invalid file, one line is longer than 1024.\n");
			exit(0);
		}
		My402ListObj *obj = (My402ListObj *)malloc(sizeof(My402ListObj));
		char *token = NULL, *temp = NULL;
		const char *split = "\t";
		int i = 0;
		token = strtok(buffer, split);
		while(token != NULL)
		{
			if(i == 0)
			{
				if(*token != '-' && *token != '+')
				{
					fprintf(stderr, "Error: Invalid transcation type.\n");
					exit(0);
				}
				obj->Transcation_type = *token;
			}
			else if(i == 1)
			{
				if(strlen(token) >= 11)
				{
					fprintf(stderr, "Error: Invalid transcation time.\n");
					exit(0);
				}
				else
				{
					int trans_time = atoi(token);
					if(trans_time > time(NULL))
					{
						fprintf(stderr, "Error: Invalid transcation time, it is greater than current time.\n");
						exit(0);
					}
					obj->Transcation_time = trans_time;
				}
			}
			else if(i == 2)
			{
				temp = token;
			}
			else if(i == 3)
			{
				token = trimspace(token);
				if(strlen(token) == 0)
				{
					fprintf(stderr, "Error: Invalid transcation description.\n");
					exit(0);
				}
				obj->Transcation_description = (char*)malloc(strlen(token)+1);
				strcpy(obj->Transcation_description, token);
			}
            else if(i >= 4)
            {
                fprintf(stderr, "Error: Invalid line-more item.\n");
                exit(0);
            }
			token = strtok(NULL, split);
			i++;
		}
        if(i != 4)
        {
            fprintf(stderr, "Error: Invalid line-less item.\n");
            exit(0);
        }
		obj->Transcation_amount = atod(temp);
		My402ListAppend(list, (void*)obj);
		free(token);
	}
	BubbleSortForwardList(list);
	PrintList(list);
}

/* ----------------------- main() ----------------------- */

int main(int argc, char *argv[])
{
    FILE *file = NULL;
    My402List list;
    struct stat sstat;
    if(argc == 2 && strcmp(argv[1], "sort") == 0)
    {
        file = stdin;
        Process(file, &list);
    }
    else if(argc == 3 && strcmp(argv[1], "sort") == 0)
    {
        file = fopen(argv[2], "r");
        if(stat(argv[2], &sstat)==0 && sstat.st_mode & S_IFDIR)
        {
            fprintf(stderr, "Error: It is directory.\n");
            return 0;
        }
        if(file == NULL)
        {
            fprintf(stderr, "Error: Cannot open the file.\n");
            return 0;
        }
        Process(file, &list);
        fclose(file);
    }
    else
    {
    	fprintf(stderr, "Error: Invalid command.\n");
    }
    return 0;
}
