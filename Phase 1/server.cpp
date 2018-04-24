#include <pthread.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#define NUM_THREADS 1000
#define roomMax 3 //per day

using namespace std;


pthread_mutex_t mutex;

sql::Driver *driver;
sql::Connection *con;
sql::Statement *stmt;
sql::ResultSet *res,*res1;
sql::PreparedStatement *pstmt;

void dbConnect()
{
	try{
    /* Create a connection */
    driver = get_driver_instance();
    //printf("inside connect\n");
    con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
    /* Connect to the MySQL test database */
    con->setSchema("project");
    cout<<"Database Connection established...\n";
	}
	catch (sql::SQLException &e) {
  		cout << "# ERR: SQLException in " << __FILE__;
  		cout << "# ERR: " << e.what();
  		cout << " (MySQL error code: " << e.getErrorCode();
  		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
}
/*
void createRoomsTable()
{
  //pstmt = con->prepareStatement("drop table rooms;");
  pstmt = con->prepareStatement("create table rooms(roomNum integer(4) not null,addharNum varchar(12) not null,name varchar (100) not null, datee date not null, constraint uc_rooms unique (addharNum,name,datee));");
  pstmt->execute();
}
void createUserRegTable();
{
  pstmt = con->prepareStatement("create table users(userName varchar(20) not null,userId varchar(20) primary key,password varchar(20) not null);");
  pstmt->execute();
}*/

bool updateInsert(int roomNum, char* name, char* addharNum,char* dateBook)
{
//  printf("name is%s\n",name );
	try{
	  //cout<<"inside update insert\n";
	  pstmt = con->prepareStatement("INSERT INTO rooms(roomNum,addharNum,name,datee) VALUES(?,?,?,?)  ");
	  pstmt->setInt(1, roomNum);
	  pstmt->setString(2, addharNum);
	  pstmt->setString(3, name);
	  pstmt->setString(4, dateBook);
	  pstmt->execute();
	  cout<<"Insert room...\n";
	  //printf("%d",roomNum);
	  fflush(stdout);
	  return true;
	}
	catch (sql::SQLException &e) {
  		cout << "# ERR: SQLException in " << __FILE__;
  		cout << "# ERR: " << e.what();
  		cout << " (MySQL error code: " << e.getErrorCode();
  		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
  		return false;
	}
}

bool addharCheck(int roomNo, char* addharNo,char* dateBookStr)
{
	try{
	  cout<<"Checking entry...\n";
	  pstmt = con->prepareStatement("SELECT roomNum,addharNum,datee from rooms where roomNum=? and addharNum=? and datee=?");
	  pstmt->setInt(1, roomNo);
	  pstmt->setString(2, addharNo);
	  pstmt->setString(3, dateBookStr);
	  res = pstmt->executeQuery();

	  if(res->next()){
	       return true;
	  }
	  else
	  {
	    return false;
	  }
	}
	catch (sql::SQLException &e) {
  		cout << "# ERR: SQLException in " << __FILE__;
  		cout << "# ERR: " << e.what();
  		cout << " (MySQL error code: " << e.getErrorCode();
  		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
}

void cancelledupdate(int roomNo, char* addharNo,char* dateBook)//deletes the entry from the table 
{
	try{
	  cout<<"Cancel room...\n";
	  pstmt = con->prepareStatement("delete from rooms where roomNum=? and addharNum=? and datee=?");
	  pstmt->setInt(1, roomNo);
	  pstmt->setString(2, addharNo);
	  pstmt->setString(3, dateBook);
	  res = pstmt->executeQuery();
	}
	catch (sql::SQLException &e) {
  		cout << "# ERR: SQLException in " << __FILE__;
  		cout << "# ERR: " << e.what();
  		cout << " (MySQL error code: " << e.getErrorCode();
  		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
}

bool ValidateUser(char* userId, char* Pass)
{
	try{
	  cout<<"Validating user...\n";
	  pstmt = con->prepareStatement("SELECT userId,password from users where userId=? and password=? ");
	  pstmt->setString(1, userId);
	  pstmt->setString(2, Pass);
	  res = pstmt->executeQuery();

	  if(res->next()){
	       return true;
	  }
	  else
	  {
	    return false;
	  }
	}
	catch (sql::SQLException &e) {
  		cout << "# ERR: SQLException in " << __FILE__;
  		cout << "# ERR: " << e.what();
  		cout << " (MySQL error code: " << e.getErrorCode();
  		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
}

bool createUser(char* regName, char* regId, char* regPass)
{
	try{
	  cout<<"creating user...\n";
	  pstmt = con->prepareStatement("insert into users values (?,?,?) ");
	  pstmt->setString(1, regName);
	  pstmt->setString(2, regId);
	  pstmt->setString(3, regPass);
	  res = pstmt->executeQuery();
	  return true;
	}
	catch (sql::SQLException &e) {
  		cout << "# ERR: SQLException in " << __FILE__;
  		cout << "# ERR: " << e.what();
  		cout << " (MySQL error code: " << e.getErrorCode();
  		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
  		return false;
	}
}


int roomsAvailable(char* dateBookStr)
{
  int i=1;
  int countNumRooms;
  int roomNumber;
  cout<<"Checking Rooms available...\n";

  try{
	  pstmt = con->prepareStatement("SELECT count(roomNum) from rooms where datee = ? ");
	  pstmt->setString(1, dateBookStr);
	  res = pstmt->executeQuery();
	   
	  if(res->next())
	  {
	      countNumRooms=res->getInt(1);
	      if(countNumRooms>=roomMax)
	        return (0);
	      else
	      {
	        pstmt = con->prepareStatement("SELECT roomNum from rooms where datee = ? ");
	        pstmt->setString(1, dateBookStr);
	        res1 = pstmt->executeQuery();
	        //cout<<"here";
	        if(countNumRooms==0)
	        {
	           return 1;
	        }
	        while(res1->next())
	        {
	          if(i<roomMax)
	          {
	            //cout<<i;
	            //cout<<"value of i";
	            roomNumber=res1->getInt(1);
	            if(i==roomNumber)
	            {
	              i++;
	              //cout<<"here"<<i<<endl;
	              continue;
	            }
	          }
	          else
	          {
	          	//cout<<"here 1 "<<i<<endl;
	          	return i;
	          }
	            
	        }
	        //cout<<"here 2 "<<i<<endl;
	        return (i);  
	      }
	  }
	  else
	    return (0);

	}
	catch (sql::SQLException &e) {
	  		cout << "# ERR: SQLException in " << __FILE__;
	  		cout << "# ERR: " << e.what();
	  		cout << " (MySQL error code: " << e.getErrorCode();
	  		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		}

}

//thread to book the ticket
void book(int clientsock)
{
  char name[30],addhar[30];
 // char roomInfo[50];//to give user info about room (booked or not)
  int i,room_booked=0;
  char bookStr[10];
  char book[] = "Your room number booked is: ";
  char dateBookStr[30];
  memset(&name,'0',sizeof(name));
  if((read(clientsock,name,sizeof(name)))<=0)
    perror("\n");
  else
  {
    //printf("%s\n",name);
    fflush(stdout);
  }

  memset(&addhar,'0',sizeof(addhar));
  if((read(clientsock,addhar,sizeof(addhar)))<=0)
    perror("\n");
  else
  {
    //printf("%s\n",addhar);
    fflush(stdout);
  }
  //printf("room count = %d\n",room_count );
  

  memset(&dateBookStr,'0',sizeof(dateBookStr));
  if((read(clientsock,dateBookStr,sizeof(dateBookStr)))<=0)
  {
    perror("Date not recieved...\n");
	pthread_exit(NULL);
  }
  else
  { 
    //printf("-----------------------Date----------------------\n");
    //printf("%s\n",dateBookStr);
    fflush(stdout);
  }


  //book the room
  pthread_mutex_lock(&mutex);
  room_booked=roomsAvailable(dateBookStr);
  //cout<<room_booked;
  //cout<<"room booked";
  if(room_booked)//if room available then room number will be returned else 0
  {
    if(updateInsert(room_booked,name,addhar,dateBookStr))
    {
	    sprintf(bookStr, "%d", room_booked);
	    strcat(book, bookStr);
	    //cout<<bookStr;
	    fflush(stdout);
	    write(clientsock, book, 40);
	}
	else
	{
		fflush(stdout);
		write(clientsock,"Room already booked with same credentials...\n",60);
	}
  
  }
  else
  {
    write(clientsock,"No rooms are available...\n",30);

  }
  pthread_mutex_unlock(&mutex);
  pthread_exit(NULL);
    
}

//function to cancel the ticket
void cancel(int clientsock)
{
  char roomNum[30],addhar[30];
  int i,roomNumberInt;
  char dateBookStr[30];
  memset(&roomNum,'0',sizeof(roomNum));
  if((read(clientsock,roomNum,sizeof(roomNum)))<=0)
    perror("\n");
  else
  {
    //printf("%s\n",roomNum);
    fflush(stdout);
    roomNumberInt=atoi(roomNum);
    //write in the file
  }
  //printf("%d\n",room_count );
  memset(&addhar,'0',sizeof(addhar));
  if((read(clientsock,addhar,sizeof(addhar)))<=0)
    perror("\n");
  else
  {
    //printf("%s\n",addhar);
    fflush(stdout);
    //write in the file
  }
  memset(&dateBookStr,'0',sizeof(dateBookStr));
  if((read(clientsock,dateBookStr,sizeof(dateBookStr)))<=0)
  {
    perror("Date not recieved...\n");
	pthread_exit(NULL);
  }
  else
  { 
    //printf("-----------------Cancel date------------------\n");
    //printf("%s\n",dateBookStr);
    fflush(stdout);
  }

  //cancel the room

  pthread_mutex_lock(&mutex); 
  if(addharCheck(roomNumberInt,addhar,dateBookStr))
    {
      //cout<<"hereee";
      cancelledupdate(roomNumberInt,addhar,dateBookStr);
      write(clientsock,"Your room is cancelled...",30);

    }
    else
    {
      write(clientsock,"One of the entry made is wrong...",50);
    }
   
  pthread_mutex_unlock(&mutex);
  pthread_exit(NULL);
}


//to reshedule the room
void reschedule(int clientsock)
{
  char roomNum[30],addhar[30],name[30];
  int i,roomNumberInt;
  char currentDate[30];
  char rescheduleDate[30];
  int checkRoom;
  char bookStr[10];
  char book[] = "Rescheduled. Your room number booked is: ";

  memset(&name,'0',sizeof(name));
  if((read(clientsock,name,sizeof(name)))<=0)
    perror("\n");
  else
  {
    //printf("%s\n",name);
    fflush(stdout);
    //write in the file
  }

  memset(&roomNum,'0',sizeof(roomNum));
  if((read(clientsock,roomNum,sizeof(roomNum)))<=0)
    perror("\n");
  else
  {
    //printf("%s\n",roomNum);
    fflush(stdout);
    roomNumberInt=atoi(roomNum);
    //write in the file
  }
  //printf("%d\n",room_count );
  memset(&addhar,'0',sizeof(addhar));
  if((read(clientsock,addhar,sizeof(addhar)))<=0)
    perror("\n");
  else
  {
    //printf("%s\n",addhar);
    fflush(stdout);
    //write in the file
  }
  memset(&currentDate,'0',sizeof(currentDate));
  if((read(clientsock,currentDate,sizeof(currentDate)))<=0)
  {
    perror("Date not recieved...\n");
	pthread_exit(NULL);
  }
  else
  { 
    //printf("-----------------Current date------------------\n");
    //printf("%s\n",currentDate);
    fflush(stdout);
  }

  memset(&rescheduleDate,'0',sizeof(rescheduleDate));
  if((read(clientsock,rescheduleDate,sizeof(rescheduleDate)))<=0)
  {
    perror("Date not recieved...\n");
	pthread_exit(NULL);
  }
  else
  { 
    //printf("-----------------Reshedule date------------------\n");
    //printf("%s\n",rescheduleDate);
    fflush(stdout);
  }

  // reshedule the room

  pthread_mutex_lock(&mutex); 
  if(addharCheck(roomNumberInt,addhar,currentDate))
    {
      //cout<<"hereee";
      checkRoom=roomsAvailable(rescheduleDate);
      if(checkRoom)
      {
        
        if(updateInsert(checkRoom,name,addhar,rescheduleDate))
        {
        	cancelledupdate(roomNumberInt,addhar,currentDate);

        	sprintf(bookStr, "%d", checkRoom);
	  	    strcat(book, bookStr);
	        //cout<<bookStr;
	        fflush(stdout);
	        write(clientsock, book, 60);
        	//write(clientsock,"Your room is rescheduled...",30);
    	}
    	else
    	{
    		write(clientsock,"Your room is already booked on rescheduled date..\n",60);
    	}
      }
      else
      {
        write(clientsock,"Rooms not available on rescheduled date...\n",60);
      }
    }
    else
    {
      write(clientsock,"One of the entry made is wrong...\n",50);
    }
   
  pthread_mutex_unlock(&mutex);
  pthread_exit(NULL);
}

//thread to give client options
void *ClientOptions(void *clientfd)
{
  long tid; 
  int clientsock = *(int*)clientfd;
  char choice[1],ch;//to read user choice
  memset(&choice,'0',sizeof(choice));
  char userId[30],Pass[30];
  char regName[30],regId[30],regPass[30];
  char login[30];

  //to register or login
  read(clientsock,login,30);

  if(strcmp(login,"login")==0)
  {
	  //retreive userid and password from client
  	  cout<<"Client choose to login...\n";
	  if(read(clientsock,userId,30)<=0)
	  	perror("\n");
	  if(read(clientsock,Pass,30)<=0)
	  	perror("\n");
	  //cout<<userId<<Pass<<"\n";

	  if(ValidateUser(userId,Pass))
	  {
	  	write(clientsock,"Yes",3);

		  if((ch=read(clientsock,choice,sizeof(choice)))<=0)
		    perror("\n");
		  else
		  {
		    if(choice[0]=='b')
		    {
		      printf("Client choose to book...\n");
		      book(clientsock);
		    }
		    else if(choice[0]=='c')
		    {
		      printf("Client choose to cancel...\n");
		      cancel(clientsock);
		    }
		    else if(choice[0]=='r') 
		    {
		      printf("Client choose to reschedule...\n");
          reschedule(clientsock);
		    }
        else
        {
          //invalid;
        }
		  }
		  pthread_exit(NULL);
		  //switch()
	   }
	   else
	   {
	   	write(clientsock,"No",30);
	   	//cout<<"Invalid Details entered by client";
	   	pthread_exit(NULL);
	   }
	}
	else
	{
	 cout<<"Client choose to register...\n";

	 if(read(clientsock,regName,30)<=0)
	 	perror("\n");
	 if(read(clientsock,regId,30)<=0)
	 	perror("\n");
	 if(read(clientsock,regPass,30)<=0)
	 	perror("\n");

	 if(createUser(regName,regId,regPass))
	 {
		write(clientsock,"User Created...\n",30);
	 	pthread_exit(NULL);
	 }
	 else
	 {
	 	write(clientsock,"User Not Created...Choose different User ID...\n",60);
	 	pthread_exit(NULL);
	 }

	}
}

int main (int argc, char *argv[])
{
    //parameters for threading
  pthread_t threads[NUM_THREADS];
  int rc,i;
  long t=0;

  dbConnect();

  pthread_mutex_init(&mutex, NULL);

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
}
