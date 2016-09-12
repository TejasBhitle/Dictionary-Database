#include <stdio.h>
#include <stdlib.h>/*to use exit function */
#include<malloc.h>/*to use malloc function*/
#include<string.h>/*to use %s and strcmp*/

int search_flag,Wordnum=0;/*counts total no. of words*/
/*flag is 0 when search function is called from main
flag is 1 when search function is called from sorter
flag is 2 when search function is called from delete*/

struct node
{
  char name[20],mean[50];
  struct node *link;  /*this means that the pointer created will point to a datatype
  struct of the node structure.*/
}*start;
/*global pointer "start" that will point to the
first node in the linked list*/

void Restore_Data()//restores data at the begin
{
    char data1[30],data2[50],temp[50],c;
    int count=0,i=0;
    printf("Data restoring\nPLEASE WAIT......\n");

    FILE *fp;
    fp=fopen("database","r");//only reads data
    printf("file opened\n");

    do{//counts no of words existing in database
        c=fscanf(fp," %[^\n]s",temp);
        count++;
    }while(c != EOF);//EOF =end of file

    rewind(fp);//take cursor to the beginning of the file

    for(i=0;i<(count-1)/2;i++)//adds data from database to linked list
    {
        fscanf(fp," %[^\n]s\n",data1);
        fscanf(fp," %[^\n]s\n",data2);
        Adder(data1,data2,i);
    }
    printf("Data restored Successfully\n");
    fclose(fp);//closes file
}

void save_data()//saves data
{
 system("cls");
 FILE *fp;
 fp=fopen("database","w");//overwrites;any previous data gets deleted
 printf("Data backup started....\n");
 struct node *ptr;
 ptr=start;
 while(ptr->link!=NULL)//writes data from linked list to file
 {
     fprintf(fp," %s\n",ptr->name);
     fprintf(fp," %s\n",ptr->mean);
     ptr=ptr->link;
 }
 fprintf(fp," %s\n",ptr->name);
 fprintf(fp," %s\n",ptr->mean);//last word gets written
 fclose(fp);

 printf("Data Saved\n");
}

void getdata()/*Takes word from user*/
{
    char data1[20],data2[50];
     printf("Enter only in lower case please\n\n");
     printf("Enter Word name:- ");
     scanf(" %[^\n]s",&data1);
     strlwr(data1);/*in case of upper case*/
     Sorter(data1);
}

int Sorter(char *data1)/*decides position of entered word & takes the meaning*/
{
 int count=0;
 /*count is no of nodes before the loc of the word to be added*/
 int y,x,i;
 char data2[50];
 struct node *ptr1;
     if(start==NULL) /*if its the first creation*/
    {
         count=0;/*if 1st then count = */
         printf("Enter meaning of %s:- ",data1);
         scanf(" %[^\n]s",&data2);
         Adder(data1,data2,count);
     }
     else /*its not the first creation*/
     {
        search_flag=1;
        /*to indicate that search is called by Sorter */
        y=search(data1);/*returns 1 if word exists*/
        if(y==1)
        {
            system("cls");
            printf("word %s already exists\n",data1);
            return 0;
        }
        else
        {
            ptr1=start;
            printf("Enter meaning of %s:- ",data1);
            scanf(" %[^\n]s",&data2);
            while(1)
            {
                x=strcmp(ptr1->name,data1);
                /*returns 1 if data1 is before ptr->name
                  returns -1 if data1 is after ptr->name*/
                if(x==1)
                    break;
                else if(ptr1->link==NULL)
                {
                    count=-1;
                    break;
                }
                else/*checking next name in the list*/
                {
                    count++;
                    ptr1=ptr1->link;/*moving to next node*/
                }
            }
            Adder(data1,data2,count);
            return 0;
        }
     }
}

int search(char *data1)
{
  struct node *ptr;
  int x,count=0;
  ptr=start;
  while(1)
  {
      x=strcmp(ptr->name,data1);
      if(x==0)
      {
          printf("word found\n");
          if(search_flag==1)/*called from sorter*/
            return 1;
          else if(search_flag==0)/*called from main*/
          {
             display(ptr->name,ptr->mean);
             return 0;
          }
          else if(search_flag==2)
          {
              return count;
          }
      }
      else if(ptr->link==NULL && search_flag==1)/*called from sorter*/
        break;

      else if(ptr->link==NULL && ((search_flag==0)||(search_flag==2)))//called by main or delete
      {
          printf("Word not found\n");
          return -1;
      }
      ptr=ptr->link;/*continue searching*/
      count++;
  }
  return 0;
}

void Adder (char *data1[],char *data2[],int count)//creates structure and adds
{
      int i;
      //char data2[50];
      struct node *tempnode,*ptrA;
      //Allocate memory to structure
      tempnode=(struct node*)malloc(sizeof(struct node));
      //printf("Enter meaning of %s:- ",data1);
      //scanf(" %[^\n]s",&data2);

      //copying contents into structure
      strcpy(tempnode->name,data1);
      strcpy(tempnode->mean,data2);

      ptrA=start;
      if(start==NULL)//if 1st creation
      {
       start=tempnode;
       tempnode->link=NULL;
      }
      else//not 1st creation
      {
       switch(count)
      {
       case 0:/*case0 means add node at beginning*/
             tempnode->link=start;
             start=tempnode;
             break;

       case -1:/*case -1 means add node at end*/
               while(1)/*Traverse to the last node*/
              {
                if(ptrA->link !=NULL)
                ptrA=ptrA->link;
               else
                break;
             }
             tempnode->link=NULL;
             ptrA->link=tempnode;
             break;

     default:/*case default means adding somewhere in middle*/
        for(i=0;i<count-1;i++)//traversing
        {
            ptrA=ptrA->link;
        }
        tempnode->link=ptrA->link;
        ptrA->link=tempnode;
        system("cls");
      }
     }
 Wordnum++;//increment total no. of words
 system("cls");
 printf("Word added\n");
}

void display(char *name[],char *mean[])//displays data
{
    int x = strlen(name);
    printf("\n");
  if(x<=7)
    printf("%s\t\t\t\t\t%s\n",name,mean);
   else
    printf("%s\t\t\t\t%s\n",name,mean);
}

int Delete()
{
  char word[20],choice;
  int position,i,j;
  struct node *ptrA ,*ptrB;
  ptrA=start;
  ptrB=start;
  printf("Enter the word you want to delete:\n");
  scanf(" %[^\n]s",&word);
  strlwr(word);
  printf("Are you sure you want to delete %s and its meaning\nChoose y/n ? \n",word);
  scanf(" %c",&choice);
  if(choice=='y')
  {
   position=search(word);

   if(position==-1)
      printf("unable to delete\n");
   else
   {
      if(position==0)//to delete first word
      {
          ptrA=ptrA->link;
          start=ptrA;
      }
      else
      {
       for(i=0;i<position-1;i++)//traversing
         ptrA=ptrA->link;
       for(j=0;j<position;j++)
         ptrB=ptrB->link;

       if(ptrB->link==NULL)//if last node
         ptrA->link=NULL;
       else
         ptrA->link=ptrB->link;
      }
     free(ptrB);//free memory
     Wordnum--;
     printf("word deleted\n");
   }
  }
  else
    printf("Delete aborted\n");
  return;
}

void About()
{
    printf("DICTIONARY USING LINKED LIST v5.0\n");
    printf("Designed by\n");
    printf("Computer Student of SARDAR PATEL INSTITUTE OF TECHNOLOGY\n\n");
    printf("Tejas Bhitle\t\t7\n");
}

void main()
{
  int choice,i;
  char word[20],option;
  struct node *ptr;
  start=NULL;
  Restore_Data();

while(1)
 {
  for(i=0;i<65;i++)
   printf("-");
  printf("\n****PROJECT DICTIONARY****\n\n");
  printf("\nTotal number of words:-%d\n\n\n",Wordnum);
  printf("1.Add word\n");
  printf("2.Search Word\n");
  printf("3.Display complete dictionary\n");
  printf("4.Delete Word\n");
  printf("5.Save\n");
  printf("6.About\n");
  printf("7.Exit\n\n");
  printf("Enter a choice:-\n");
  scanf("%d",&choice);

  switch(choice)
  {
   case 1:system("cls");
          getdata();
          break;
   case 2:
       system("cls");
       search_flag=0;//calling search from main
       if(start==NULL)/*if no word added*/
       {
           printf("Search is invalid\n");
           printf("Dictionary is empty\nAdd words first\n\n");
           break;
       }
       else
        printf("Enter word to search:-\n");
        scanf(" %[^\n]s",&word);
        strlwr(word);//to lower case
        search(word);
         break;
   case 3:
       system("cls");
       if(start==NULL)/*if no word added*/
       {
           printf("Dictionary is empty\nAdd words first\n\n");
           break;
       }
       else
       {
           ptr=start;
           printf("NAME\t\t\t\t\tMEANING\n");
           for(i=0;i<65;i++)
              printf("-");
           printf("\n");
           while(1)
           {
                display(ptr->name,ptr->mean);
               if(ptr->link !=NULL)
                ptr=ptr->link;
               else
                break;
           }
       }
       break;
   case 4:
       system("cls");
       search_flag=2;
       if(start==NULL)
       {
           printf("delete is invalid\nAdd words first\n\n");
           break;
       }
       else
        Delete();
        break;
   case 5:
       save_data();
       break;
   case 6:
      system("cls");
      About();
      break;
  case 7:
       system("cls");
       printf("All your data will be saved\n");
       printf("Are you sure you want to exit\ny/n ?\n");
       scanf(" %c",&option);
       if(option=='y')
       {
         save_data();
         printf("THANK YOU FOR USING DICTIONARY\n");
         exit(0);
       }
       system("cls");
       break;
  default:
       system("cls");
       printf("choice invalid\n");
  }
 }
}
