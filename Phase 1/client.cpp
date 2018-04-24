#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h> 
#include <ctime>
#include <iostream>
using namespace std;

bool validateDate(char* date)
    {
    	    //cuurrent date
     time_t t = time(0);   // get time now
     struct tm * now = localtime( & t );
     int year = now->tm_year + 1900 ;
     int month = now->tm_mon + 1;
     int day = now->tm_mday;

     try{
     string s(date);
     int yr = atoi(s.substr(0,4).c_str());
     int mn = atoi(s.substr(5,2).c_str());
     int dy = atoi(s.substr(8,2).c_str());

     if(mn > 12)
     {
     	return false;
     }
     if(dy > 31)
     {
     	return false;
     }

     if(yr < year)
     {
     	return false;
     }
     else{
     	if(yr == year && mn < month)
     	{
     		return false;
     	}
     	else{
     		if(mn == month && dy < day)
     		{
     			return false;
     		}
     	}
     }
     return true;
 	}
 	catch(exception &e)
 	{
 		return false;
 	}
 }


int main(int argc, char *argv[])
{
    struct sockaddr_in serv_addr;
    int sockfd =0 ;
    int k,j;//to take user request
    int i;
    char book_info[50],roomNumber[10];
    //parameters needed to book the room book
    char name[30],addhar[30];
    char userId[30], pass[30];
    char cond[3];
    char regName[30],regId[30],regPass[30];
    char create_info[50];
    int room_booked;
    int client_id; 
    char date[20];
    char resdate[20];


    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)//open socket at client's client side
    {
      		printf("\n Error : Could not create socket \n");
       		return 1;
   	} 
	memset(&serv_addr, '0', sizeof(serv_addr)); 
	serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));//Central server port number
	printf("Server address used is: %s\n", argv[1]);//Central Servel ip address
  if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
  	{
        	printf("\n inet_pton error occured\n");
        	return 1;
    } 
	if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)//connection made to Central Server
    	{
		perror("\n Error : Connect Failed \n");
		return 1;
    }
    else
    {
    	printf("connected to Server...\n");

      printf("Please choose :\n 1. to login as a existing user \n 2. to register as a new user \n");
      scanf("%d",&j);

      switch(j)
      {
       case 1 : write(sockfd,"login",5);
                printf("Please enter your user ID: ");
                scanf("%s",userId);
                write(sockfd,userId,sizeof(userId));

                printf("Please enter your Password: ");
                scanf("%s",pass);
                write(sockfd,pass,sizeof(pass));

                memset(&cond,'0',sizeof(cond));
                if((read(sockfd,cond,sizeof(cond)))<=0)
                    perror("\n");
                else
                {
                    //printf("%s\n",cond);
                    fflush(stdout);
                }

                for(i=0;i<3;i++)
                    cond[i]=cond[i];
                cond[3]='\0';
                //printf("%s",cond);

                if(strcmp(cond,"Yes")==0)
                {  
                  printf("Welcome User...\n");
                  printf("Please choose :\n 1. to book a room \n 2. to cancel a room \n 3. to reschedule a room \n ");
                  scanf("%d",&k);
                  switch(k)
                  {
                    case 1 : printf("You choose to book a room...\n");
                             write(sockfd,"b",1);
                             printf("Enter your name:\n");
                             scanf("%s",name);
                             write(sockfd,name,sizeof(name));
                             printf("Enter your addhar number:\n");
                             scanf("%s",addhar);
                             write(sockfd,addhar,sizeof(addhar));
                             printf("Enter a date with format  yyyy-mm-dd: \n");
                             scanf("%s",date);
                             if(validateDate(date))
                             {
                                write(sockfd,date,sizeof(date));
                                memset(&book_info,'0',sizeof(book_info));
                                if((read(sockfd,book_info,sizeof(book_info)))<=0)
                                    perror("\n");
                                else printf("%s\n",book_info);            
                                    fflush(stdout);
                             }
                             else
                             {
                                printf("Invalid date entered...\n");
                             }                  
                             break;
                   
                    case 2 : printf("You choose to cancel a room...\n");
                             write(sockfd,"c",1);
                             printf("Enter your room Number:\n");
                             scanf("%s",roomNumber);
                             write(sockfd,roomNumber,sizeof(roomNumber));
                             printf("Enter your addhar number:\n");
                             scanf("%s",addhar);
                             write(sockfd,addhar,sizeof(addhar));
                             printf("Enter a cancel date with format  yyyy-mm-dd:  \n");
                             scanf("%s",date);
                             if(validateDate(date))
                             {
	                             write(sockfd,date,sizeof(date));
	                             memset(&book_info,'0',sizeof(book_info));
	                             if((read(sockfd,book_info,sizeof(book_info)))<=0)
	                              perror("\n");
	                             else printf("%s\n",book_info);
	                             fflush(stdout);      
	                         } 
	                         else
                             {
                                printf("Invalid date entered...\n");
                             }          
                             break;

                    case 3 : printf("you choose to reschedule...\n");
                             write(sockfd,"r",1);
                             printf("Enter your name:\n");
                             scanf("%s",name);
                             write(sockfd,name,sizeof(name));
                             printf("Enter your room Number:\n");
                             scanf("%s",roomNumber);
                             write(sockfd,roomNumber,sizeof(roomNumber));
                             printf("Enter your addhar number:\n");
                             scanf("%s",addhar);
                             write(sockfd,addhar,sizeof(addhar));
                             printf("Enter current booking date with format  yyyy-mm-dd:\n");
                             scanf("%s",date);
                             if(validateDate(date))
                             {
                             write(sockfd,date,sizeof(date));
                         	 }
                         	 else
                             {
                                printf("Invalid date entered...\n");
                                break;
                             } 
                             printf("Enter reschedule date with format  yyyy-mm-dd:\n");
                             scanf("%s",resdate);
                             if(validateDate(resdate))
                             {
                             write(sockfd,resdate,sizeof(resdate));
                             memset(&book_info,'0',sizeof(book_info));
                             if((read(sockfd,book_info,sizeof(book_info)))<=0)
                              perror("\n");
                             else printf("%s\n",book_info);
                             fflush(stdout);
                             }   
                             else
                             {
                                printf("Invalid date entered...\n");
                             }              
                             break;
                   
                   default : printf("invalid selection...\n");
                  }
                }
                else
                {
                  printf("Invalid credentials...\n");
                }
                break;

        case 2 : write(sockfd,"register",8);
                 printf("Please enter your name:\n");
                 scanf("%s",regName);
                 write(sockfd,regName,sizeof(regName));
                 printf("Please select a User ID:\n");
                 scanf("%s",regId);
                 write(sockfd,regId,sizeof(regId));
                 printf("Please select a Password:\n");
                 scanf("%s",regPass);
                 write(sockfd,regPass,sizeof(regPass));
                 memset(&create_info,'0',sizeof(create_info));
                 if((read(sockfd,create_info,sizeof(create_info)))<=0)
                  perror("\n");
                 else printf("%s\n",create_info);            
                 fflush(stdout);                 
                 break;

        default : printf("Invalid selection...\n");        
     }

    }
} 