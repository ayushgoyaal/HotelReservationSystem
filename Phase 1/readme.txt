Software required:-

1. Boost
Procedure to install:
* From the given link download boost package
https://sourceforge.net/projects/boost/?source=typ_redirect
* To install boost,go into the folder and execute
./bootstrap
./b2 install

2. Mysql-connector-cpp
        Follow this link : 
https://dev.mysql.com/doc/connector-cpp/en/connector-cpp-installation-source-distribution.html
OR
Procedure to install:
* Execute in shell
git clone https://github.com/mysql/mysql-connector-cpp
* Go Inside mysql-connector-cpp folder and execute following in shell
   * cmake -DBOOST_ROOT=/(----Relative path_of_Boost folder ----)/boost_1_61_0/. 
   * make clean
   * make -j4
   * make install
   * sudo apt install libmysqlcppconn-dev

3. CPP

---------------------------------------------------------------------------------------------------------
                
Process to run code:-

* to compile the server code 
   * Execute makefile 
      * make server

* to compile the client code
   * Execute makefile 
      * make client

* To run server code
   * Execute 
      * ./server PORT_NUMBER                        

* To run client code
   * Execute
      * .client IP_OF_SERVER PORT_NUMBER_OF_SERVER


---------------------------------------------------------------------------------------------------------

Process to test the code:-

Test for following request made by clients - 

1. Register:
      Enter username, user id and password. The user id must be unique for all user. If you choose already used user id the user will not be registered.

2. Login
   1. Enter registered user id and password else you will not be able to login.

3. Book
   1. Enter name, Aadhaar number ,and a valid Date.
   2. Returns the booking status
      1. If room is booked, room number is returned.
      2. If room is not booked, reason for not booking is returned.

4. Cancel
   1. Enter valid Room number, Aadhaar number and Date
      1. If details entered are correct, room is cancelled
      2. If details entered are not correct, error message is shown. 

5. Reschedule
   1. Enter valid name, Room number ,Aadhaar number, Current Date and Rescheduled Date of booking 
      1. If details entered are correct,room is rescheduled and new room number is returned.
      2. If details entered are not correct, reason for not rescheduling is returned.

Some​ ​ critical​ ​ scenarios:

1.​ ​ If​ ​ two​ ​ clients​ ​ try​ ​ to​ ​ book​ ​ a ​ ​ room​ ​ for​ ​ same​ ​ date​ ​ at​ ​ the​ ​ same​ ​ time​ ​ then​ ​ they​ ​ should​ ​ not
be​ ​ allocated​ ​ same​ ​ room​ ​ number.

2.​ ​ If​ ​ only​ ​ one​ ​ room​ ​ is​ ​ available​ ​ on​ ​ a ​ ​ given​ ​ date​ ​ and​ ​ two​ ​ clients​ ​ try​ ​ to​ ​ book​ ​ that​ ​ room​ ​ at
the​ ​ same​ ​ time​ ​ then​ ​ the​ ​ room​ ​ will​ ​ be​ ​ allocated​ ​ to​ ​ only​ ​ one​ ​ user.
