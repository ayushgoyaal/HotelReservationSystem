#include <cpp_redis/cpp_redis>
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
using namespace std;

#ifdef _WIN32
#include <Winsock2.h>
#endif /* _WIN32 */
#define NUM_THREADS 1000000
#define roomMax 10000000 //per day


long roomBookedNum=0;
pthread_mutex_t mutex1;

cpp_redis::client client;

int insertRoom(char* dateBook,long roomNum,char* name,char* aadhaar)
{

  char roomNumStr[50];

  sprintf(roomNumStr,"%ld",roomNum);

  auto set1 = client.hset(roomNumStr, "name", name);
  auto set2 = client.hset(roomNumStr, "addhar", aadhaar);
  auto set3 = client.hset(roomNumStr, "date", dateBook);
  auto set4 = client.hset(roomNumStr, "roomno", roomNumStr);

  client.sync_commit();

  //cout << roomNumStr << endl;

  return 1;

}

int cancelRoom(long roomNum)//deletes the entry from the table
{
  //fflush(stdout);
  //cout<<"Check1";

  char roomNumStr[50];

  sprintf(roomNumStr,"%ld",roomNum);
  //cout<<"Roomno:"<<roomNumStr<<endl;

  vector<string> vec;
  vec.push_back(roomNumStr);

  //cout<<vec[0]<<endl;

  client.del(vec);

  client.sync_commit();

  //cout<<"Deleted" <<endl;
  
  return 1;

}

//thread to book the ticket
void book(int clientsock,int k)
{
  char name[] = "name";
  char aadhaar[]="aadhaar";
  char book[] = "Your room number booked is: ";
  char dateBookStr[]="2017-12-12,";
  char bookStr[50];
  char nameStr[50];
  char numStr[50];
  char aadhaarStr[50];

  sprintf(nameStr, "%d", clientsock);
	strcat(nameStr,"a");
  sprintf(numStr, "%d", k);
  strcat(name, nameStr);
	strcat(name,numStr);
  sprintf(aadhaarStr, "%d", k);
	strcat(aadhaar, nameStr);
  strcat(aadhaar, aadhaarStr);


	pthread_mutex_lock(&mutex1);
	roomBookedNum++;
	pthread_mutex_unlock(&mutex1);


  if(roomBookedNum)
  {
    if(insertRoom(dateBookStr,roomBookedNum,name,aadhaar))
    {
	    sprintf(bookStr, "%ld", roomBookedNum);
	    strcat(book, bookStr);
	    //cout<<bookStr;
	    fflush(stdout);
	    write(clientsock, book, 50);
	  }
  }
}


//function to cancel the ticket
void cancel(int clientsock,int k)
{
  char name[] = "name";
  char addhar[] = "addhar";
  char roomNum[50];
  char dateBookStr[]="2017-12-12";
  char bookStr[50];
  char nameStr[50];
  char numStr[50];
  char addharStr[50];
  
  sprintf(nameStr, "%d", clientsock);
  strcat(nameStr,"a");
  sprintf(numStr, "%d", k);
  strcat(name, nameStr);
  strcat(name,numStr);
  sprintf(addharStr, "%d", k);
  strcat(addhar, nameStr);
  strcat(addhar, addharStr);


  pthread_mutex_lock(&mutex1);
	roomBookedNum--;
	pthread_mutex_unlock(&mutex1);
  //cout<<"Values: "<<dateBookStr<<endl<<roomBookedNum<<endl<<name<<endl<<addhar<<endl;

  //cancel the room
  if(cancelRoom(roomBookedNum))
  {
  	write(clientsock,"Your room is cancelled...",30);
  }
  else
  {
  	write(clientsock,"Room not cancelled...",50);
  }

}


//thread to give client options
void *ClientOptions(void *clientfd)
{
  int clientsock = *(int*)clientfd;
  char choice[1],ch;//to read user choice
	long bookNum=0;
  while (1)
	 {
				if((ch=read(clientsock,choice,1))<=0)
		    	{
            //perror("\n");
            break;
		    	}
				else
				  {
				  	bookNum++;
				    if(choice[0]=='b')
				    {
				      book(clientsock,bookNum);
				    }
				    else if(choice[0]=='c')
				    {
              //cout<<"Check cancel"<<endl;
              //cout<<bookNum<<endl;
				      cancel(clientsock,bookNum);
				    }
		        else
		        {
		          printf("invalid\n" );
							break;
		        }
				  }
    }
		close(clientsock);
    pthread_exit(NULL);
}


int main (int argc, char *argv[])
{
  #ifdef _WIN32
    //! Windows netword DLL init
    WORD version = MAKEWORD(2, 2);
    WSADATA data;

    if (WSAStartup(version, &data) != 0)
    {
      std::cerr << "WSAStartup() failure" << std::endl;
      return -1;
    }
  #endif

  //! Enable logging
  cpp_redis::active_logger = std::unique_ptr<cpp_redis::logger>(new cpp_redis::logger);

  

  client.connect("127.0.0.1", 6379, [](const std::string& host, std::size_t port, cpp_redis::client::connect_state status) {
    if (status == cpp_redis::client::connect_state::dropped) {
      std::cout << "client disconnected from " << host << ":" << port << std::endl;
    }
  },
    0, -1, 5000);


  //parameters for threading
  pthread_t threads[NUM_THREADS];
  int rc,i;
  long t=0;

  //dbConnect();

  pthread_mutex_init(&mutex1, NULL);

    //parameters for socket coonection
  int listenfd = 0;//to allow number of clients
  struct sockaddr_in serv_addr,client_addr;
  socklen_t addr_size;
  int connfd = 0;//socket discriptor


  listenfd = socket(AF_INET, SOCK_STREAM, 0);//welcome socket
  memset(&serv_addr, '0', sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;//server family
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(atoi(argv[1]));//host to network short(Big endian)---port number of central server
  bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));//binding port with the socket
  listen(listenfd, 10);
  printf("Welcome to server...\nReady to accept connections...\n");


  while(1)//infinite loop to accept multiple connections
  {
      addr_size = sizeof(client_addr); //since accept field take 3 arg as socklen_t type
      connfd = accept(listenfd,(struct sockaddr *)&client_addr,&addr_size);
      if(connfd<0)
      {
          perror("Accept error...\n");
          exit(1);
      }
      printf("Connection accepted...\n");
      fflush(stdout);
      rc = pthread_create(&threads[t], NULL, ClientOptions, (void *)&connfd);
      //printf("%d\n",rc );
      t++;
       if (rc){
          //printf("ERROR; return code from pthread_create() is %d\n", rc);
          //exit(-1);
       }
  }

    
  
  //std::this_thread::sleep_for(std::chrono::milliseconds(1));
  client.disconnect();
  
  #ifdef _WIN32
    WSACleanup();
  #endif
  return 0;
}
