#include <iostream>
#include <string>
#include <windows.h>
#include <mysql.h>
#include <string.h>
#include <sstream>
#include <dos.h>
#include <cstdio>

using namespace std;


void user_signup(MYSQL* conn,MYSQL_ROW row,MYSQL_RES *res);
void buy_ticket(MYSQL* conn,MYSQL_ROW row,MYSQL_RES *res,string movie_id);
void delete_movie_list(MYSQL* conn,MYSQL_ROW row,MYSQL_RES *res,string id);
void update_movie_list(MYSQL* conn,MYSQL_ROW row,MYSQL_RES *res,string movie_id);
void add_movie_list(MYSQL* conn,MYSQL_ROW row,MYSQL_RES *res);
void show_movie_list(MYSQL* conn,MYSQL_ROW row,MYSQL_RES *res);
void user_login(MYSQL* conn,MYSQL_ROW row,MYSQL_RES *res);
void Admin(MYSQL* conn,MYSQL_ROW row,MYSQL_RES *res);
void User(MYSQL* conn,MYSQL_ROW row,MYSQL_RES *res);
int welcome();

void user_signup(MYSQL* conn,MYSQL_ROW row,MYSQL_RES *res)
{
    string username;

    cout<<"Enter username :";   cin>>username;

    int qstate;

    qstate = mysql_query(conn,"select * from user");

    if(!qstate)
    {
        //int record_found = 0;
        res = mysql_store_result(conn);
        while((row=mysql_fetch_row(res)))
        {
            if(username==row[3])
            {
                cout<<"Already registered."<<endl;
                user_signup(conn,row,res);
            }
        }
    }

        string firstName,lastName,password;

        cout<<"Enter first name :"; cin>>firstName;
        cout<<"Enter last name :";  cin>>lastName;
        cout<<"Enter password :";   cin>>password;

        string status = "active";

        string query="insert into user(firstName,lastName,username,password,status) values('"+firstName+"','"+lastName+"','"+username+"','"+password+"','"+status+"')";


        const char* q = query.c_str();

        //cout<<"query is: "<<q<<endl;
        qstate = mysql_query(conn,q);

        if(!qstate)
        {
            cout<<"record inserted successfully..."<<endl;
            User(conn,row,res);
        }
        else
            cout<<"query problem: "<<mysql_error(conn)<<endl;


}

void buy_ticket(MYSQL* conn,MYSQL_ROW row,MYSQL_RES *res,string movie_id)
{
    int qstate;

    qstate = mysql_query(conn,"select * from movielist");

    if(!qstate)
    {
        res = mysql_store_result(conn);
        while((row=mysql_fetch_row(res)))
        {
            if(movie_id==row[0])
            {
                string currentTicket = row[5];

                //string to int

                stringstream degree(currentTicket);

                int currentTicketCount = 0;
                degree>>currentTicketCount;


                if(currentTicketCount>0)
                {
                    currentTicketCount = currentTicketCount-1;

                    //int to string

                    ostringstream str1;
                    str1<<currentTicketCount;
                    string ticketCount = str1.str();

                     string query = "update movielist set ticketCount = '"+ticketCount+"' where id = '"+movie_id+"' ";

                     const char* q = query.c_str();

                    mysql_query(conn,q);

                    cout<<"Ticket bought succesfull.\n"<<endl;
                }
                else
                    cout<<"No ticket available..."<<endl;
            }
        }
    }
}

void delete_movie_list(MYSQL* conn,MYSQL_ROW row,MYSQL_RES *res,string id)
{
    string query = "delete from movielist where id = '"+id+"' ";

    const char* q = query.c_str();

    mysql_query(conn,q);

    cout<<"Movie deleted"<<endl;
}

void update_movie_list(MYSQL* conn,MYSQL_ROW row,MYSQL_RES *res,string movie_id)
{
    cout<<"1.Movie Name\t2.Actor\t3.Actress\n"<<endl;
    int option;
    cout<<"What you want to modify :";  cin>>option;

    char newdata[100];
    cout<<"New data :"; gets(newdata);

    string data(newdata);

    switch(option)
    {
    case 1 :
        {
            string query = " update movielist set movieName = ' "+data+" ' where id = ' "+movie_id+" ' ";

            const char* q = query.c_str();

            mysql_query(conn,q);

            cout<<"Movie name updated..."<<endl;

            break;
        }
        case 2 :
        {
            string query = " update movielist set actor = ' "+data+" ' where id = ' "+movie_id+" ' ";

            const char* q = query.c_str();

            mysql_query(conn,q);

            cout<<"Actor name updated..."<<endl;

            break;
        }
        case 3 :
        {
            string query = " update movielist set actress = ' "+data+" ' where id = ' "+movie_id+" ' ";

            const char* q = query.c_str();

            mysql_query(conn,q);

            cout<<"Actress name updated..."<<endl;

            break;
        }
        default:
            cout<<"Invalid option...."<<endl;
    }
}

void add_movie_list(MYSQL* conn,MYSQL_ROW row,MYSQL_RES *res)
{
    char name[100],get_movieActor[100],get_movieActress[100];
    string ticketPrice,ticketCount;

    cout<<"Enter movie name :";
    fflush(stdin);
    gets(name);
    cout<<"Enter movie actor :";
    gets(get_movieActor);
    cout<<"Enter movie actress :";
    gets(get_movieActress);
    cout<<"Enter ticket price :";
    cin>>ticketPrice;
    cout<<"Enter total number of ticket :";
    cin>>ticketCount;

    string movieName(name);
    string movieActor(get_movieActor);
    string movieActress(get_movieActress);

    string query = " INSERT INTO movielist (movieName,actor,actress,ticketPrice,ticketCount) VALUES ( ' " +movieName+ " ',' " +movieActor+ " ',' " +movieActress+ " ',' " +ticketPrice+ " ',' " +ticketCount+ " ' )";

    const char* q = query.c_str();

    mysql_query(conn,q);

    cout<<"Movie Added..."<<endl;
}

void show_movie_list(MYSQL* conn,MYSQL_ROW row,MYSQL_RES *res)
{
    int qstate = mysql_query(conn,"select * from movielist");

        if(!qstate)
        {
            res = mysql_store_result(conn);
            while((row=mysql_fetch_row(res)))
            {
                cout<<"ID : "<<row[0]<< " "
                    <<"Movie name : "<<row[1]<< " "
                    <<"Actor : "<<row[2]<<" "
                    <<"Actress : "<<row[3]<<" "
                    <<"Ticket price : "<<row[4]<<" "
                    <<"Ticket count : "<<row[5]<<
                    endl;
            }
        }
}

void user_login(MYSQL* conn,MYSQL_ROW row,MYSQL_RES *res)
{
    string usernameInput,passwordInput;

    cout<<"Enter username :";   cin>>usernameInput;
    cout<<"Enter password :";   cin>>passwordInput;

    int qstate;

    qstate = mysql_query(conn,"select * from 	user");

    //cout<<"qstate = "<<qstate;

    int record_found =0;

    if(!qstate)
    {

        string status = "active";
        res = mysql_store_result(conn);

        while((row=mysql_fetch_row(res)))
        {
            if(((usernameInput==row[3])&&(passwordInput==row[4])&&(status==row[5])))
            {
                string firstNameFromDB = row[1];
                string lastNameFromDb = row[2];

                record_found++;

                cout<<"Welcome,"<<firstNameFromDB<<endl;

                show_movie_list(conn,row,res);

                string movie_id;

                cout<<"Enter id of movie to buy ticket :";  cin>>movie_id;

                buy_ticket(conn,row,res,movie_id);

                    }

                }
            }
            if(record_found==0)
                cout<<"Invalid user!!"<<endl;
}



void Admin(MYSQL* conn,MYSQL_ROW row,MYSQL_RES *res)
{
    //system("cls");

    cout<<"\t\t\tWelcome to admin section\n"<<endl;

        string usernameInput,passwordInput;

        cout<<"Enter username :";   cin>>usernameInput;
        cout<<"Enter password :";   cin>>passwordInput;

        int qstate;

        qstate = mysql_query(conn,"select * from admin");

        //cout<<"qstate = "<<qstate;

        if(!qstate)
        {
            int record_found =0;
            string status = "active";
            res = mysql_store_result(conn);
            while((row=mysql_fetch_row(res)))
            {
                if((usernameInput==row[3])&&(passwordInput==row[4])&&(status==row[5]))
                {
                    string firstNameFromDB = row[1];
                    string lastNameFromDb = row[2];

                    record_found++;

                    //system("cls");

                    cout<<"Welcome,"<<firstNameFromDB<<endl;

                    cout<<"1.Show Movie List\t2.Add Movie List\t3.Update Movie List\t4.Delete Movie List\n";

                    int admin_activity;
                    cin>>admin_activity;

                    switch(admin_activity)
                    {
                    case 1 :
                        cout<<"Movie List..."<<endl;
                        show_movie_list(conn,row,res);
                        break;

                    case 2 :
                        add_movie_list(conn,row,res);
                        break;

                    case 3 :{
                        show_movie_list(conn,row,res);
                        string movie_id;
                        cout<<"Enter id of movie :";    cin>>movie_id;
                        update_movie_list(conn,row,res,movie_id);
                        break;
                    }

                    case 4 :{
                        show_movie_list(conn,row,res);
                        string movie_id;
                        cout<<"Enter id of movie :";    cin>>movie_id;
                        delete_movie_list(conn,row,res,movie_id);
                        break;
                    }

                    default:
                        cout<<"Invalid option!!"<<endl;
                    }

                }
            }
            if(record_found==0)
                cout<<"Invalid admin!!"<<endl;
        }
        else
        {
            cout<<"query error: "<<mysql_error(conn)<<endl;
        }
}

void User(MYSQL* conn,MYSQL_ROW row,MYSQL_RES *res)
{
    //system("cls");

    cout<<"\t\t\tWelcome to user section\n"<<endl;

    cout<<"1.Login\t2.Signup\n"<<endl;

    int option_for_user;

    cout<<"Option :";   cin>>option_for_user;

    switch(option_for_user)
    {
    case 1 :
        user_login(conn,row,res);
        break;

    case 2 :
        user_signup(conn,row,res);
        cout<<"User Login :"<<endl;
        user_login(conn,row,res);
        break;

    default:
        cout<<"incorrect option!!"<<endl;
    }

}

int welcome()
{
    //system("cls");

    cout<<"Welcome to my Ticket Booking System"<<endl;
    cout<<"1.Admin\t2.User\t3.Exit\n"<<endl;
    int option;
    cin>>option;
    return option;
}

int main()
{
    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES *res;

    conn = mysql_init(0);

/*
    if(conn)
        cout<<"connection object ok, conn="<<conn<<endl;
    else
        cout<<"conn object problem: "<<mysql_error(conn);
        */
    while(1){
    int login_type = welcome();


    conn = mysql_real_connect(conn,"localhost","root","","ticketbookingmanagement",0,NULL,0);
/*
    if(conn)
        cout<<"connected to database ticketbookingmanagement"<<endl;
    else
        cout<<"Not connected";
*/

    switch(login_type)
    {
    case 1:
        Admin(conn,row,res);
        break;
    case 2 :
        User(conn,row,res);
        break;
    case 3 :
        exit(0);
    default:
        cout<<"Invalid option!!"<<endl;
    }

    }
    mysql_close(conn);

    return 0;
}
