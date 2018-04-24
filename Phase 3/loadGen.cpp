#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
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

#include <pthread.h>


using namespace std;
char *portNum;
char *ipAddress;
int count=0;
int timee;
time_t start, end;
double elapsed;
long double resp=0;

long double ReqCount=0;
long double BookReqCount=0;
long double CancelReqCount=0;

pthread_mutex_t mutex;


void *bookReq(void *threadId)
{

    struct sockaddr_in serv_addr;
    int sockfd =0,i;
    char *userId, *pass;
    char name[]="name",addhar[]="addhar",clientidStr[30];
    long clientid;
    clientid = (long)threadId;
    char date[]="2017-12-12",book_info[50];
    char cond[3];

    struct timeval st,ed;
    long double start1;
    long double end1;
    long double elapsed1;


    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)//open socket at client's client side
    {
      		printf("\n Error : Could not create socket \n");
    }
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(portNum));//Central server port number
    //printf("Server address used is: %s\n", ipAddress);//Central Servel ip address
    if(inet_pton(AF_INET, ipAddress, &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
    }
    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)//connection made to Central Server
    {
        perror("\n Error : Connect Failed \n");
    }
    else
    {
        printf("connected to Server...\n");
        count++;
        printf("-------%d----------\n",count);
        while(1)
        {

            end = time(NULL);
            elapsed = difftime(end, start);
            if (elapsed >= timee /* seconds */)
            {
            	printf("No: %Lf Book Requests \n",ReqCount);
            	printf("Throughput: %Lf \n",(long double)ReqCount/timee);
            	printf("Avg Resp Time: %Lf \n",(long double)resp/ReqCount);
                //cout<<"No: "<< ReqCount<<" Book Exited\n";
                //cout<<"Resp Time of all req: "<< resp << "\n";
                exit(2);
            }
            //printf("book ");
            //printf("%ld\n",clientid);
            write(sockfd,"b",1);
            gettimeofday(&st,NULL);
            start1 = 1000000 * st.tv_sec + st.tv_usec;

            memset(&book_info,'0',sizeof(book_info));
            if((read(sockfd,book_info,sizeof(book_info)))<=0)
                perror("\n");
            else //printf("%s\n",book_info);
              {
              	gettimeofday(&ed,NULL);
              	end1 = 1000000 * ed.tv_sec + ed.tv_usec;
              	elapsed1 = end1 - start1;
              	//cout<<"Time: "<<elapsed1;
              	pthread_mutex_lock(&mutex);
                resp = resp + elapsed1;
                ReqCount++;
                pthread_mutex_unlock(&mutex);

              }

          }
      }
    close(sockfd);
    pthread_exit(NULL);
}


void *bookCancelReq(void *threadId)
{

    struct sockaddr_in serv_addr;
    int sockfd =0,i;
    char *userId, *pass;
    char name[]="name",addhar[]="addhar",clientidStr[30];
    long clientid;
    clientid = (long)threadId;
    char date[]="2017-12-12",book_info[50];
    char cond[3];

    struct timeval st,ed;
    long double start1;
    long double end1;
    long double elapsed1;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)//open socket at client's client side
    {
      		printf("\n Error : Could not create socket \n");
    }
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(portNum));//Central server port number
    //printf("Server address used is: %s\n", ipAddress);//Central Servel ip address
    if(inet_pton(AF_INET, ipAddress, &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
    }
    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)//connection made to Central Server
    {
        perror("\n Error : Connect Failed \n");
    }
    else
    {
        printf("connected to Server...\n");
        count++;
        printf("-------%d----------\n",count);
        while(1)
        {

            end = time(NULL);
            elapsed = difftime(end, start);
            if (elapsed >= timee /* seconds */)
            {
            	printf("No: %Lf Book Requests \n No: %Lf Cancel Requests\n",BookReqCount,CancelReqCount);
		printf("Total No: %Lf \n",(BookReqCount+CancelReqCount));
            	printf("Throughput: %Lf \n ",(long double)(BookReqCount + CancelReqCount)/timee);
            	printf("Avg resp time: %Lf \n",(long double)resp/(BookReqCount + CancelReqCount));
                //cout<<"No:"<<BookReqCount<<" Book\n "<<"No: "<<CancelReqCount<<" Cancel Exited\n";
                //cout<<"Resp Time of all req: "<< resp << "\n";
                exit(2);
            }
            //printf("book ");
            //printf("%ld\n",clientid);
            if(elapsed <= (timee * 0.70))
            {
            write(sockfd,"b",1);
            gettimeofday(&st,NULL);
            start1 = 1000000 * st.tv_sec + st.tv_usec;

            memset(&book_info,'0',sizeof(book_info));
            if((read(sockfd,book_info,sizeof(book_info)))<=0)
                perror("\n");
            else //printf("%s\n",book_info);
              {
              	gettimeofday(&ed,NULL);
              	end1 = 1000000 * ed.tv_sec + ed.tv_usec;
              	elapsed1 = end1 - start1;
                pthread_mutex_lock(&mutex);
                resp = resp + elapsed1;
                BookReqCount++;
                pthread_mutex_unlock(&mutex);

              }
          	}
          	else
          	{
            write(sockfd,"c",1);
            gettimeofday(&st,NULL);
            start1 = 1000000 * st.tv_sec + st.tv_usec;

            memset(&book_info,'0',sizeof(book_info));
            if((read(sockfd,book_info,sizeof(book_info)))<=0)
              perror("\n");
            else
            {
              gettimeofday(&ed,NULL);
              end1 = 1000000 * ed.tv_sec + ed.tv_usec;
              elapsed1 = end1 - start1;
              pthread_mutex_lock(&mutex);
              resp = resp + elapsed1;
              CancelReqCount++;
              pthread_mutex_unlock(&mutex);

            }
       		}


          }
      }
    close(sockfd);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    struct sockaddr_in serv_addr;
    int sockfd =0 ;
    long thread;
    int rc;
    int j;
    void *status;
    int NUM_THREADS =atoi(argv[3]);

    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;


    /* Initialize and set thread detached attribute */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);


    ipAddress = argv[1];
    portNum = argv[2];
    timee = atof(argv[4]);
    //printf("%s",ipAddress);

    printf("Please choose :\n 1. to load test book request \n 2. to load test book and cancel requests\n");
    scanf("%d",&j);

    start = time(NULL);

    switch(j)
    {


	case 1: for(thread=0;thread<NUM_THREADS;thread++)
		    {
		        //usleep(100);
		        printf("%ld\n",thread);
		        rc=pthread_create(&threads[thread], NULL,bookReq, (void *)thread);
		        if(rc)
		        {
		          printf("ERROR; return code from pthread_create() is %d\n", rc);
		          exit(-1);
		        }
		        else
		        {
		            printf("success\n");
		        }

		    }
		    //printf("Outside thread creation\n" );
		    /* Free attribute and wait for the other threads */
		    pthread_attr_destroy(&attr);
		    for(thread=0; thread<NUM_THREADS; thread++) {
		       rc = pthread_join(threads[thread], &status);
		       if (rc) {
		          //printf("ERROR; return code from pthread_join() is %d\n", rc);
		          exit(-1);
		          }
		       //printf("Main: completed join with thread %ld having a status of %ld\n",thread,(long)status);
		       }

		     //printf("Main: program completed. Exiting.\n");
		     pthread_exit(NULL);

	case 2: for(thread=0;thread<NUM_THREADS;thread++)
		    {
		        //usleep(100);
		        printf("%ld\n",thread);
		        rc=pthread_create(&threads[thread], NULL,bookCancelReq, (void *)thread);
		        if(rc)
		        {
		          printf("ERROR; return code from pthread_create() is %d\n", rc);
		          exit(-1);
		        }
		        else
		        {
		            printf("success\n");
		        }

		    }
		    //printf("Outside thread creation\n" );
		    /* Free attribute and wait for the other threads */
		    pthread_attr_destroy(&attr);
		    for(thread=0; thread<NUM_THREADS; thread++) {
		       rc = pthread_join(threads[thread], &status);
		       if (rc) {
		          //printf("ERROR; return code from pthread_join() is %d\n", rc);
		          exit(-1);
		          }
		       //printf("Main: completed join with thread %ld having a status of %ld\n",thread,(long)status);
		       }

		     //printf("Main: program completed. Exiting.\n");
		     pthread_exit(NULL);

	default: cout<<"Invalid Entry\n";

 	}
    return 1;

}
