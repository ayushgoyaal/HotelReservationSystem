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
#define NUM_THREADS 1000000
#define roomMax 10000000 //per day

using namespace std;

int room_booked=0;
pthread_mutex_t mutex;


void dbConnect()
{

	sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	sql::ResultSet *res,*res1;
	sql::PreparedStatement *pstmt;
	try{
    /* Create a connection */
    driver = get_driver_instance();
    //printf("inside connect\n");
    con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
    /* Connect to the MySQL test database */
    con->setSchema("project");
    //cout<<"Database Connection established...\n";
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
  pstmt = con->prepareStatement("create table rooms(roomNum integer(10) not null,addharNum varchar(12) not null,name varchar (100) not null, datee date not null, constraint uc_rooms unique (addharNum,name,datee));");
  pstmt->execute();
}
void createUserRegTable();
{
  pstmt = con->prepareStatement("create table users(userName varchar(20) not null,userId varchar(20) primary key,password varchar(20) not null);");
  pstmt->execute();
}*/

bool updateInsert(int roomNum, char* name, char* addharNum,char* dateBook)
{
	sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	sql::ResultSet *res,*res1;
	sql::PreparedStatement *pstmt;
	try{
    /* Create a connection */
    driver = get_driver_instance();
    //printf("inside connect\n");
    con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
    /* Connect to the MySQL test database */
    con->setSchema("project");
    //cout<<"Database Connection established...\n";
	}
	catch (sql::SQLException &e) {
  		cout << "# ERR: SQLException in " << __FILE__;
  		cout << "# ERR: " << e.what();
  		cout << " (MySQL error code: " << e.getErrorCode();
  		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
//  printf("name is%s\n",name );
	try{
	  //cout<<"inside update insert\n";
	  pstmt = con->prepareStatement("INSERT INTO rooms(roomNum,addharNum,name,datee) VALUES(?,?,?,?)  ");
	  pstmt->setInt(1, roomNum);
	  pstmt->setString(2, addharNum);
	  pstmt->setString(3, name);
	  pstmt->setString(4, dateBook);
	  pstmt->execute();
	  //cout<<"Insert room...\n";
	  //printf("%d",roomNum);
	  fflush(stdout);
		delete con;
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

int addharCheck(char* name, char* addharNo,char* dateBookStr)
{
	int room=0;
	sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	sql::ResultSet *res,*res1;
	sql::PreparedStatement *pstmt;
	try{
    /* Create a connection */
    driver = get_driver_instance();
    //printf("inside connect\n");
    con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
    /* Connect to the MySQL test database */
    con->setSchema("project");
    //cout<<"Database Connection established...\n";
	}
	catch (sql::SQLException &e) {
  		cout << "# ERR: SQLException in " << __FILE__;
  		cout << "# ERR: " << e.what();
  		cout << " (MySQL error code: " << e.getErrorCode();
  		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}

	try{
	  //cout<<"Checking entry...\n";
	  pstmt = con->prepareStatement("SELECT roomNum from rooms where name=? and addharNum=? and datee=?");
	  pstmt->setString(1, name);
	  pstmt->setString(2, addharNo);
	  pstmt->setString(3, dateBookStr);
	  res = pstmt->executeQuery();

	  if(res->next()){
			  room=res->getInt(1);
				delete con;
	       return room;
	  }
	  else
	  {
	    delete con;
			return 0;
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
		sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	sql::ResultSet *res,*res1;
	sql::PreparedStatement *pstmt;
	try{
    /* Create a connection */
    driver = get_driver_instance();
    //printf("inside connect\n");
    con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
    /* Connect to the MySQL test database */
    con->setSchema("project");
    //cout<<"Database Connection established...\n";
	}
	catch (sql::SQLException &e) {
  		cout << "# ERR: SQLException in " << __FILE__;
  		cout << "# ERR: " << e.what();
  		cout << " (MySQL error code: " << e.getErrorCode();
  		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
	try{
	  //cout<<"Cancel room...\n";
	  pstmt = con->prepareStatement("delete from rooms where roomNum=? and addharNum=? and datee=?");
	  pstmt->setInt(1, roomNo);
	  pstmt->setString(2, addharNo);
	  pstmt->setString(3, dateBook);
	  res = pstmt->executeQuery();
		delete con;
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
		sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	sql::ResultSet *res,*res1;
	sql::PreparedStatement *pstmt;
	try{
    /* Create a connection */
    driver = get_driver_instance();
    //printf("inside connect\n");
    con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
    /* Connect to the MySQL test database */
    con->setSchema("project");
    //cout<<"Database Connection established...\n";
	}
	catch (sql::SQLException &e) {
  		cout << "# ERR: SQLException in " << __FILE__;
  		cout << "# ERR: " << e.what();
  		cout << " (MySQL error code: " << e.getErrorCode();
  		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
	try{
	  cout<<"Validating user...\n";
	  pstmt = con->prepareStatement("SELECT userId,password from users where userId=? and password=? ");
	  pstmt->setString(1, userId);
	  pstmt->setString(2, Pass);
	  res = pstmt->executeQuery();

	  if(res->next()){
	  		//cout<<"In valid usr";
				delete con;
				 return true;
	  }
	  else
	  {
			delete con;
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
		sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	sql::ResultSet *res,*res1;
	sql::PreparedStatement *pstmt;
	try{
    /* Create a connection */
    driver = get_driver_instance();
    //printf("inside connect\n");
    con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
    /* Connect to the MySQL test database */
    con->setSchema("project");
    //cout<<"Database Connection established...\n";
	}
	catch (sql::SQLException &e) {
  		cout << "# ERR: SQLException in " << __FILE__;
  		cout << "# ERR: " << e.what();
  		cout << " (MySQL error code: " << e.getErrorCode();
  		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
	try{
	  cout<<"creating user...\n";
	  pstmt = con->prepareStatement("insert into users values (?,?,?) ");
	  pstmt->setString(1, regName);
	  pstmt->setString(2, regId);
	  pstmt->setString(3, regPass);
	  res = pstmt->executeQuery();
		delete con;
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

bool cancelRoom()//deletes the entry from the table
{
	sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	sql::ResultSet *res,*res1;
	sql::PreparedStatement *pstmt;
	try{
    /* Create a connection */
    driver = get_driver_instance();
    //printf("inside connect\n");
    con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
    /* Connect to the MySQL test database */
    con->setSchema("project");
    //cout<<"Database Connection established...\n";
	}
	catch (sql::SQLException &e) {
  		cout << "# ERR: SQLException in " << __FILE__;
  		cout << "# ERR: " << e.what();
  		cout << " (MySQL error code: " << e.getErrorCode();
  		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
	try{
	  //cout<<"Cancel room...\n";
	  stmt = con->createStatement();
  	  stmt->execute("Delete from rooms limit 1");
  	  delete stmt;
  	  delete con;
  	  return true;
  	  
	}
	catch (sql::SQLException &e) {
  		/*cout << "# ERR: SQLException in " << __FILE__;
  		cout << "# ERR: " << e.what();
  		cout << " (MySQL error code: " << e.getErrorCode();
  		cout << ", SQLState: " << e.getSQLState() << " )" << endl;*/
  		return false;
  		
	}
}


int roomsAvailable(char* dateBookStr)
{
	sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	sql::ResultSet *res,*res1;
	sql::PreparedStatement *pstmt;
	try{
    /* Create a connection */
    driver = get_driver_instance();
    //printf("inside connect\n");
    con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
    /* Connect to the MySQL test database */
    con->setSchema("project");
    //cout<<"Database Connection established...\n";
	}
	catch (sql::SQLException &e) {
  		cout << "# ERR: SQLException in " << __FILE__;
  		cout << "# ERR: " << e.what();
  		cout << " (MySQL error code: " << e.getErrorCode();
  		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
			cout<<"insiderooms available\n";
	}
  int i=1;
  int countNumRooms;
  int roomNumber;
  //cout<<"Checking Rooms available...\n";
	//cout<<dateBookStr<<"\n";

  try{
		//cout<<"asda";
	  pstmt = con->prepareStatement("SELECT count(roomNum) from rooms where datee = ? ");
		pstmt->setString(1, dateBookStr);
		//cout<<"asda";
		res = pstmt->executeQuery();
		//cout<<"herwwe";
	  if(res->next())
	  {
	      countNumRooms=res->getInt(1);
				//printf("counted number of rooms is equal to %d\n",countNumRooms );
	      if(countNumRooms>=roomMax)
	        {
						delete con;
						return (0);
					}
	      else
	      {
	        pstmt = con->prepareStatement("SELECT roomNum from rooms where datee = ? order by roomNum ");
	        pstmt->setString(1, dateBookStr);
	        res1 = pstmt->executeQuery();
	        //cout<<"here";
	        if(countNumRooms==0)
	        {
				delete con;
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
							//printf("room no returned is %d\n",i );
							delete con;
							return i;
	          }

	        }
	        //cout<<"here 2 "<<i<<endl;
					//printf("room no returned is %d\n",i );
					delete con;
					return (i);
	      }
	  }
	  else{
			delete con;
			return (0);
		}


	}
	catch (sql::SQLException &e) {
	  		cout << "# ERR: SQLException in " << __FILE__;
	  		cout << "# ERR: " << e.what();
	  		cout << " (MySQL error code: " << e.getErrorCode();
	  		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		}

}

//thread to book the ticket
void book(int clientsock,int k)
{
	//cout<<"Innn Book";
  char name[] = "name";
  char addhar[] = "addhar";
 // char roomInfo[50];//to give user info about room (booked or not)
  int i;
	//int room_booked=0;
  char bookStr[10];
  char nameStr[10];
	char numStr[10];
  char addharStr[10];
  char book[] = "Your room number booked is: ";
  char dateBookStr[]="2017-12-12";
//  memset(&name,'0',5);
/*  if((read(clientsock,name,5))<=0)
    perror("\n");
  else
  {4 Exited

    //printf("%s\n",name);
    fflush(stdout);
  }
*/
//  memset(&addhar,'0',7);
/*  if((read(clientsock,addhar,7))<=0)
    perror("\n");
  else
  {
    //printf("%s\n",addhar);
    fflush(stdout);
  }
  */
  //printf("room count = %d\n",room_count );
  //printf("here i book\n");

  /*memset(&dateBookStr,'0',4);////////////////////////////////hereeeeeeeeeeeeeeee
  if((read(clientsock,dateBookStr,sizeof(dateBookStr)))<=0)
  {int k=0;
    perror("Date not recieved...\n");
	pthread_exit(NULL);
  }
  else
  {
    //printf("-----------------------Date----------------------\n");
    //printf("%s\n",dateBookStr);
    fflush(stdout);
  }
	*/
	sprintf(nameStr, "%d", clientsock);
	strcat(nameStr,"a");

  	sprintf(numStr, "%d", k);
  	strcat(name, nameStr);
	strcat(name,numStr);

    sprintf(addharStr, "%d", k);
	strcat(addhar, nameStr);

  	strcat(addhar, addharStr);

	//printf("\n%s\n%s\n%s\n",name,addhar,dateBookStr );

  //book the room


  //pthread_mutex_lock(&mutex);

  //room_booked=roomsAvailable(dateBookStr);

	pthread_mutex_lock(&mutex);
	room_booked++;
	pthread_mutex_unlock(&mutex);


  //cout<<"room booked";
	//cout<<room_booked;
  if(room_booked)//if room available then room number will be returned else 0
  {
    if(updateInsert(room_booked,name,addhar,dateBookStr))
    {
	    sprintf(bookStr, "%d", room_booked);
	    strcat(book, bookStr);
	    //cout<<bookStr;
	    fflush(stdout);
	    write(clientsock, book, 50);
	}
	else
	{
		fflush(stdout);
		//write(clientsock,"Room already booked with same credentials...\n",60);
	}

  }
  else
  {
   // write(clientsock,"No rooms are available...\n",30);

  }
  //pthread_mutex_unlock(&mutex);
  //pthread_exit(NULL);

}

//function to cancel the ticket
void cancel(int clientsock,int k)
{
  char name[] = "name";
  char addhar[] = "addhar";
  char roomNum[30];
  int i,roomNumberInt;
  char dateBookStr[]="2017-12-12";
  char bookStr[10];
  char nameStr[10];
  char numStr[10];
  char addharStr[10];
  /*
  sprintf(nameStr, "%d", clientsock);
  strcat(nameStr,"a");
  sprintf(numStr, "%d", k);
  strcat(name, nameStr);
  strcat(name,numStr);
  sprintf(addharStr, "%d", k);
  strcat(addhar, nameStr);
  strcat(addhar, addharStr);*/

  //cancel the room
  if(cancelRoom())
  {
  	write(clientsock,"Your room is cancelled...",30);
  }
  else
  {
  	write(clientsock,"Room not cancelled...",50);
  }

  //pthread_mutex_lock(&mutex);
 /* roomNumberInt=addharCheck(name,addhar,dateBookStr);
  if(roomNumberInt)
    {
      //cout<<"hereee";
      cancelledupdate(roomNumberInt,addhar,dateBookStr);
      write(clientsock,"Your room is cancelled...",30);

    }
    else
    {
      write(clientsock,"One of the entry made is wrong...",50);
    }*/

  //pthread_mutex_unlock(&mutex);
  //pthread_exit(NULL);
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

  // reshedule the roomint k=0;

  pthread_mutex_lock(&mutex);
  if(addharCheck(name,addhar,currentDate))
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
  //pthread_exit(NULL);
}

//thread to give client options
void *ClientOptions(void *clientfd)
{
  long tid;
  int clientsock = *(int*)clientfd;
  char choice[1],ch;//to read user choice
  //memset(&choice,'0',sizeof(choice));
  char userId[]="a",Pass[]="a";
  char regName[30],regId[30],regPass[30];
  char login[30];
  int k,valUser;
	int bookNum=0;
  //to register or login
  //read(clientsock,login,5);
  //printf("%s\n",login);
  //k=strcmp(login,"login");+
  //printf("%d\n",k);
  k=0;
	//dbConnect();
  if(k==0)
  {
	  //retreive userid and password from client
  	  //cout<<"Client choose to login...\n";
	  /*if(read(clientsock,userId,1)<=0)
	  	perror("\n");
	  if(read(clientsock,Pass,1)<=0)
	  	perror("\n");
	  //cout<<userId<<Pass<<"\n";
          */
					/*
          userId[0]='a';
          userId[1]='\0';
          Pass[0]='a';
					Pass[1]='\0';
*/
		pthread_mutex_lock(&mutex);
		valUser=ValidateUser(userId,Pass);
		pthread_mutex_unlock(&mutex);
	  if(valUser)
	  {
	  	  //write(clientsock,"Yes",3);
                  //printf("%s  \n %s\n",userId,Pass);
                  //printf("book read %s",ch);
                  //ch=read(clientsock,choice,1);
			while (1)
			{
				//printf("-----------book----------\n");
				if((ch=read(clientsock,choice,1))<=0)
		    	{//perror("\n");
		    		}
					else
				  {
				  	bookNum++;
				    if(choice[0]=='b')
				    {
				      //printf("................book...............\n");
				      book(clientsock,bookNum);
				    }
				    else if(choice[0]=='c')
				    {
				      //printf("Client choose to cancel...\n");
				      cancel(clientsock,bookNum);
							//bookNum--;
				    }
				    else if(choice[0]=='r')
				    {
				      printf("Client choose to reschedule...\n");
		          reschedule(clientsock);
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
	   else
	   {
	   	write(clientsock,"No",30);
	   	//cout<<"Invalid Details entered by client";
			close(clientsock);
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
		close(clientsock);
	 	pthread_exit(NULL);
	 }
	 else
	 {
	 	write(clientsock,"User Not Created...Choose different User ID...\n",60);
		close(clientsock);
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

  //dbConnect();

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
