#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <sys/stat.h>

using namespace std;
using namespace boost;

// ls; suppose to work
//echo with quation marks suppose to ignore " "
//# COMMENTS

void CreateArgList(vector <int> &alist, string &s) // makes a list of arguments
{
 
    string argm;  
    string st = s;
    while(st.find("&&") != string::npos || st.find("||") != string::npos
    || st.find(";") != string::npos) //a while loop to delete arguments
    {
        int i = 5000;
        int f = 0;
        int e = 0;
        
        if (st.find("||") < i )
        {
            i = st.find("||");
            argm = "||";
        }
        if (st.find("&&") < i )
        {
            i = st.find("&&");
            argm = "&&";
        }
        if (st.find(";") < i )
        {
            i = st.find(";");
            argm = ";";
        }
        
        if (st.find('(') < st.size())
        {
            f = st.find('(');
        }
        
        if (st.find(')') < st.size())
        {
            e = st.find(')');
        }
        
        // everything before this finds the first instance of an argument
        if (argm == "&&")
        {
            if (i > f && i < e)
            {
                st.erase(st.begin()+i);
                st.erase(st.begin()+i);
                f = st.find('(');
                st.erase(st.begin()+f);
                e = st.find(')');
                st.erase(st.begin()+f);
            }
            else
            {
                st.erase(st.begin()+i);
                st.erase(st.begin()+i);
                alist.push_back(1);
            }
        }
        if (argm == "||")
        {
            if (i > f && i < e)
            {
                st.erase(st.begin()+i);
                st.erase(st.begin()+i);
                f = st.find('(');
                st.erase(st.begin()+f);
                e = st.find(')');
                st.erase(st.begin()+f);
            }
            else
            {
                st.erase(st.begin()+i);
                st.erase(st.begin()+i);
                alist.push_back(2);
            }
        }
        if (argm == ";")
        {
            if (i > f && i < e)
            {
                st.erase(st.begin()+i);
                f = st.find('(');
                st.erase(st.begin()+f);
                e = st.find(')');
                st.erase(st.begin()+f);
            }
            else
            {
                st.erase(st.begin()+i);
                alist.push_back(3);
            }
        }
        // i push back three numbers which stand for the argument
        // 1 stands for &&, 2 stands for || 3 stands for ;
        // this logic shows up in future code
    }
}

int test_real(string a) //helper function
{
//reg = 2
//dir = 1
//DNE = 0
    struct stat sb;
    
    if (stat(a.c_str(), &sb) == -1) 
    {
        return 0;
    }
    switch (sb.st_mode & S_IFMT) {
    case S_IFDIR:  return 1;                            break;
    case S_IFREG:  return 2;                            break;
    default:       printf("unknown?\n");                break;
    }
}
bool test_main(string b, string flag)  // pass string and flag
{                                      // return true or false if the test is
    int test_num = test_real(b);       // true or false
    if(flag == "-e" && test_num != 0)
        return true;
    else if(flag == "-f" && test_num == 2)
        return true;
    else if(flag == "-d" && test_num == 1)
        return true;
    else 
        return false;
}

void parsethis(string test, vector <string>& real)
// parses through the command, first by breaking up with tokenizer and then
// pushing back each individual string into a vector to be easily processed
{
    int counter = 0;
    vector<string> place;
    char_separator<char> sepspaces(" ");
                     //tokenizer used to split up string into smaller strings
    tokenizer<char_separator<char> > token_spaces(test, sepspaces); 
    BOOST_FOREACH(string t, token_spaces)           
    {
        place.push_back(t);         //pushback strings into a vector
    }
    // for (int i =0; i < place.size(); ++i)
    // {
    //     cout << place.at(i) << endl;
    // }
    for(int i = 0; i < place.size(); ++i)
    {
        if(place.at(i) == "par")
        {
            real.push_back("par");
            string ex;
            int j = i + 1;
            while((j < place.size()) && (place.at(j) != "par2"))
            {
                ex += place.at(j) + " "; //creating the string for echo
                ++j;
                ++i;
            }
            ++i;
            real.push_back(ex);
            counter = 0;
        }
        else
        {
            counter++;
            if(place.at(i) != "&&" && place.at(i) != "echo" && place.at(i) != 
                    "||" && place.at(i) != ";")
            {
                real.push_back(place.at(i));
            }
            else if(place.at(i) == "echo")
            {
                real.push_back("echo");
                string ex;
                int j = i + 1;
                while((j < place.size()) && (place.at(j) != "&&") && (place.at(j) 
                            != "||") && (place.at(j) != ";"))
                {
                                            //removing the quation marks
                    while(place.at(j).find('\"') < place.at(j).size())
                    {
                        place.at(j).erase(place.at(j).find('\"'),1);
                    }
                    ex += place.at(j) + " "; //creating the string for echo
                    ++j;
                    ++i;
                }
                real.push_back(ex);
                counter = 0;
            }
            else if (place.at(i) == "&&" || place.at(i) == "||" || 
                        place.at(i) == ";")
            {
                if (counter == 2)
                {
                    real.push_back("  ");
                }
                counter = 0;
            }
            
        }
        if (counter == 1 && place.at(i) != "test")
        {
            real.push_back("  ");
        }
    }
    
    for (int i =0; i < real.size(); ++i)
    {
        if(real.at(i) == "par2")
        {
            real.erase(real.begin() + i);
        }
    }
}
void colonoscopy(string &test) //we use this function to format our list guide
{                              //this basically just add a space before a semi
    vector<int> destinations;
    for (int i = 0; i < test.size(); i++)
    {
        if (test.at(i) == ';')
        {
            destinations.push_back(i);
        }
    }
    for (int i = 0; i < destinations.size(); i++)
    {
        test.insert(test.begin()+destinations.at(i) + i, ' ');
    }
}

bool execute( string ap, string ba, string flag ); //to mention it previously

bool letsdoit(vector<int> &aglist, vector<string> &plist, vector <string> &flist) // logic function
{
    // we set up the format as two vectors, one for arguments, and one for
    // commands and their parameters (vector of ints and one of strings)
    // from the tokenizer we set up single ls commands to have a space after
    // echos have a string after and etc
    string flag = flist.at(0);
    bool status = execute(plist.at(0),plist.at(1),flag); //executes first command
    if(plist.at(0) == "test")
    {
        flag.erase(flag.begin());
    }
    for (int i = 0; i < aglist.size(); i++) // main goal is to go through the 
    {                                       // argument list
        flag = flist.at(0);
        if (aglist.at(i) == 1) //the && argument
        {
            if(plist.at(0) == "test")
            {
                flag.erase(flag.begin());
            }
            if (status == false) //checks if first command was false
            {
                plist.erase(plist.begin());  //deletes the current command
                plist.erase(plist.begin());  // does not update status
            }                                // and doesnt execute
            else
            {
                plist.erase(plist.begin()); // deletes current command
                plist.erase(plist.begin()); // but executes and updates status
                status = execute(plist.at(0),plist.at(1), flag);
            }
        }
        if (aglist.at(i) == 2) // the || argument
        {
            if(plist.at(0) == "test")
            {
                flag.erase(flag.begin());
            }
            if (status == true)
            {
                plist.erase(plist.begin());
                plist.erase(plist.begin());
        
            }
            else
            {
                plist.erase(plist.begin());
                plist.erase(plist.begin());
                status = execute(plist.at(0),plist.at(1), flag);
            }
        }
        if (aglist.at(i) == 3) // the ; argument
        {
            if(plist.at(0) == "test")
            {
                flag.erase(flag.begin());
            }   
            plist.erase(plist.begin());
            plist.erase(plist.begin());
            status = execute(plist.at(0),plist.at(1), flag);
        }
    }
    return status;
}

void deletecomments(string &input) // this deletes comments 
{
    int i = input.find('#'); // this takes the substring of everything before the
    if (i != 0 && i < input.size()) //"#" character if there's no '"' char before
    {   
        if (input.at(i-1) != 0 && i < input.find('"'))
        {
            input = input.substr(0, i);
        }
    }
    else if (i == 0 && i < input.size())
    {
        input.clear();
    }
}

void flaggetter(string &input, vector <string> &flaglist) //gets the flags and puts them into a vector
{
    string argm; 

    while(input.find("-e") != string::npos || input.find("-f") != string::npos
    || input.find("-d") != string::npos) //a while loop to delete arguments
    {
        int i = 5000;
        
        if (input.find("-e") < i )
        {
            i = input.find("-e");
            argm = "-e";
        }
        if (input.find("-f") < i )
        {
            i = input.find("-f");
            argm = "-f";
        }
        if (input.find("-d") < i )
        {
            i = input.find("-d");
            argm = "-d";
        }
        // everything before this finds the first instance of an argument
        if (argm == "-e")
        {
            input.erase(input.begin()+i);
            input.erase(input.begin()+i);
            flaglist.push_back("-e");
        }
        if (argm == "-f")
        {
            input.erase(input.begin()+i);
            input.erase(input.begin()+i);
            flaglist.push_back("-f");
        }
        if (argm == "-d")
        {
            input.erase(input.begin()+i);
            input.erase(input.begin()+i);
            flaglist.push_back("-d");
        }
        // i push back three numbers which stand for the argument
        // 1 stands for &&, 2 stands for || 3 stands for ;
        // this logic shows up in future code
    }
    if (flaglist.size() < 1)
    {
        flaglist.push_back("-e");
    }
}
bool brackets(string &p) //replace brackets with test
{
    int i = 0;
    int j = 0;
    if(p.find('[') < p.size() && p.find(']') < p.size())
    {
        if(p.at(p.find('[') + 1) != ' ' || p.at(p.find(']') - 1) != ' ')
        {
            cout << "bash: command not found";
            exit(0);
        }
    }
    char endp = ']';
    while(p.find('[') < p.size() || p.find(endp) < p.size())
    {
        bool check = false;
        i = p.find('[');
        j = p.find(']');
        if (j < i)
        {
            cout << "bash: syntax error near unexpected token" << endl;
            return false;
        }
        for (int z = i+1; z < j; z++)
        {
            if(p.at(z) != ' ')
            {
                check = true;
                break;
            }
        }
        if (check == false)
        {
            cout << "bash: syntax error near unexpected token" << endl;
            return false;
        }
        if (j > p.size())
        {
            cout << "bash: syntax error near unexpected token" << endl;
            return false;
        }
        p.erase(p.begin()+i);
        p.erase(p.begin()+j-1);
        p.insert(p.begin()+i, ' ');
        p.insert(p.begin()+i, 't');
        p.insert(p.begin()+i, 's');
        p.insert(p.begin()+i, 'e');
        p.insert(p.begin()+i, 't');
    }
    return true;
}
bool parentheses(string &p) //finds first instances of '(' and ')'
{
    int i = 0;
    int j = 0;
    char endp = ')';
    while(p.find('(') < p.size() || p.find(endp) < p.size())
    {
        bool check = false;
        i = p.find('(');
        j = p.find(')');
        if (j < i)
        {
            cout << "this one";
            cout << "bash: syntax error near unexpected token" << endl;
            return false;
        }
        for (int z = i+1; z < j; z++)
        {
            if(p.at(z) != ' ')
            {
                check = true;
                break;
            }
        }
        if (check == false)
        {
            cout << "no this ne;";
            cout << "bash: syntax error near unexpected token" << endl;
            return false;
        }
        if (j > p.size())
        {
            cout << "actually this";
            cout << "bash: syntax error near unexpected token" << endl;
            return false;
        }
        p.erase(p.begin()+i);
        p.insert(p.begin()+i, ' ');
        p.insert(p.begin()+i, 'r');
        p.insert(p.begin()+i, 'a');
        p.insert(p.begin()+i, 'p');
        
        j = p.find(')');
        if (j != p.size() - 1)
        { 
            p.erase(p.begin()+j);
            p.insert(p.begin()+j+1, ' ');
            p.insert(p.begin()+j+1, '2');
            p.insert(p.begin()+j+1, 'r');
            p.insert(p.begin()+j+1, 'a');
            p.insert(p.begin()+j+1, 'p');
            p.insert(p.begin()+j+1, ' ');
        }
        else
        {
            p.push_back(' ');
            p.push_back('p');
            p.push_back('a');
            p.push_back('r');
            p.push_back('2');
            p.push_back(' ');
        }
        
    }
    return true;
}
//returns true if all guidelines are followed
//returns false if there are errors

void flag_error(string check) //checks for multiple flag arguments
{
    vector<string> place;
    char_separator<char> sepspaces(" ");
                     //tokenizer used to split up string into smaller strings
    tokenizer<char_separator<char> > token_spaces(check, sepspaces); 
    BOOST_FOREACH(string t, token_spaces)           
    {
        place.push_back(t);         //pushback strings into a vector
    }
    for(int i = 0; i < place.size(); ++i)
    {
        if(place.at(i) == "-e" || place.at(i) == "-d" || place.at(i) == "-f" && i != place.size())
        {
            if(place.at(i + 1).at(0) == '-')
            {
                cout << "bash: test: too many arguments" << endl;
                exit(0);
            }
        }
    }
}


// a mini version of our int main
void mini(string &substr, vector <int> &argumentlist, vector <string> 
          &parsedlist, vector <string> &flist)
{
    // if (parentheses(substr) == false)
    // {
    //     exit(0);
    // }
    flag_error(substr);
    deletecomments(substr);
    flaggetter(substr, flist);
    CreateArgList(argumentlist,substr);
    colonoscopy(substr);
    parsethis(substr, parsedlist);
    double size = parsedlist.size();
    if (argumentlist.size() != 0)
    {
        if (argumentlist.at(argumentlist.size()-1) == 3 && 
            argumentlist.size() != size/2 - 1 )
             //takes out ; at the end because it doesn't do anything
        {                       
            argumentlist.pop_back();
        }
    }
    if (argumentlist.size() != size/2 - 1) // checks for double args
    {
        cout << "that one";
        cout << "bash: syntax error near unexpected token" << endl;
    }
}


bool execute( string ap, string ba, string flag )     
                                        //exectutes the command ,returns 
{                                      //false if fails,true if succeeds
    
    if(ap != "test" && ap != "par")
    {
        int status;
        char* final [3];
        if(ba == "  ")              //incase of no other arguement
        {                           //e.g ls
            final[0] = (char*)ap.c_str();
            final[1] = NULL;
            final[2] = NULL;
        }
        else
        {
            final[0] = (char*)ap.c_str(); //if there is another arguement
            final[1] = (char*)ba.c_str(); //e.g echo hello
            final[2] = NULL;
        }
        pid_t c_pid, pid;
        c_pid=fork();           //making the fork/pid
        
        if(c_pid==-1)                       //failed fork
        {
            cout<<"fork failed: please try again"<<endl;
            perror("FORK:");
            return false;
        }
        else if(c_pid==0)                    //child
        {
            int t;
            t=execvp(final[0],final);
            perror("ERROR: ");
            exit(t);
        }    
        else
        {
            pid = wait(&status);            //parent
            if(pid){}
            if(status==0)
                return true;        //if execute succeds, return true
            else 
                return false;       //if execute fails, return false
        }
    }
    else if (ap == "par") //changed it here, if we see a parentheses, recursively call letsdoit
    {
        vector <int> aglist;
        vector <string> plist;
        vector <string> flist;
        mini(ba, aglist, plist, flist);
        return letsdoit(aglist,plist,flist);
        
    }
    else
    {
        if (ba == "test")
        {
            return true;
        }
        
        if (test_main(ba, flag) == true)
        {
            cout << "(True)\n";
            return true;
        }
        else
        {
            cout << "(False)\n";
            return false;
        }
    }
}

int main()
{
    char* login=getlogin(); //getting the login
    char hostname[1024];    //hostname
    hostname[1023]='\0';    //CONVENTIONS FOR OBTAINING HOSTNAME
    int hostname_affirm=gethostname(hostname,30);
    string test;
    while (1)
    {
        
        if(login==NULL || hostname_affirm!=0)   //checking if hostname is available
        cout<<"$ ";         //DISPLAYING THE DOLLAR PROMPT IF hostname is not available (like in cloud9)
        else
        cout<<login<<"@"<<hostname<<"$ ";   //display prompt if prompt is available
        getline(cin,test);
        if(test=="exit")
            exit(1);
        vector <int> argumentlist;
        flag_error(test);
        CreateArgList(argumentlist,test);
        if (parentheses(test) == false)
            exit(0);
        if (brackets(test) == false)
            exit(0);
        vector <string> parsedlist;
        vector <string> flist;
        deletecomments(test);
        flaggetter(test, flist);
        colonoscopy(test);
        parsethis(test, parsedlist);
        
        // for (int i = 0; i < argumentlist.size(); i++)
        // {
        //     cout << argumentlist.at(i) << endl;
        // }
        // for (int i = 0; i < parsedlist.size(); i++)
        // {
        //     cout << parsedlist.at(i) << endl;
        // }
        double size = parsedlist.size();
        if (argumentlist.size() != 0)
        {
            if (argumentlist.at(argumentlist.size()-1) == 3 && 
                argumentlist.size() != size/2 - 1 )
                 //takes out ; at the end because it doesn't do anything
            {                       
                argumentlist.pop_back();
            }
        }
        if (argumentlist.size() != size/2 - 1) // checks for double args
        {
            cout << "bash: syntax error near unexpected token" << endl;
        }
        else
        {
            letsdoit(argumentlist, parsedlist, flist);
        }
    }
    return 0;
}
