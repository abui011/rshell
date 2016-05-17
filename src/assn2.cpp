#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <string>
#include <iostream>
#include <vector>
#include <iostream>
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

using namespace std;
using namespace boost;

void CreateArgList(vector <int> &alist, string &s)//makes a list of arguments
{
	string argm;
	string st = s;
	while(st.find("&&") != string::npos || st.find("||") != string::npos
	|| st.find(";") != string::npos) //a while loop to delete arguments
	{
		int i = 5000;
		if (st.find("||") < i)
		{
			i = st.find("||");
			argm = "||";
		}
		if (st.find("&&") < i)
		{
			i = st.find("&&");
			argm = "&&";
		}
		if (st.find(";") < i)
		{
			i = st.find(";");
			argm = ";";
		}
		//everything before this finds the first instance of an argumen
		if(argm == "&&")
		{
			st.erase(st.begin() + i);
			st.erase(st.begin() + i);
			alist.push_back(1);
		}
		if (argm == "||")
		{
			st.erase(st.begin() + i);
			st.erase(st.begin() + i);
			alist.push_back(2);
		}
		if(argm == ";")
		{
			st.erase(st.begin() + i);
			alist.push_back(3);
		}
	}
}
// i push back three numbers which stand for the argument
// 1 stands for &&, 2 stands for || 3 stands for ;
// this logic shows up in future code
bool execute( string ap, string ba)         //exectutes the command ,returns 
                                            //false if fails,true if succeeds
{

	int status;
	char* final [3];
	if(ba == "  ")			//incase of no other arguement
	{				//e.g ls
		final[0] = (char*)ap.c_str();
		final[1] = NULL;
		final[3] = NULL;
	}
	else
	{
		final[0] = (char*)ap.c_str(); //if there is another arguement
		final[1] = (char*)ba.c_str(); //e.g echo hello
		final[2] = NULL;
	}
	pid_t c_pid, pid;
	c_pid = fork();		//making the fork/pid

	if(c_pid == -1)		//failed fork
	{
		cout << "fork failed: please try again" << endl;
		perror("FORK: ");
		return false;
	}
	else if(c_pid == 0)		//child
	{
		int t;
		t = execvp(final[0],final);
		perror("ERROR: ");
		exit(t);
	}
	else
	{
		pid = wait(&status);	//parent
		if(pid){}
		if(status == 0)
			return true;	//if execute succeds, return true
		else
		return false;		//if execute fails, return false
	}
}
void parsethis(string test, vector <string>& real)
// parses through the command, first by breaking up with tokenizer and then
// // pushing back each individual string into a vector to be easily processed
{
	int counter = 0;
	vector<string> place;
	char_separator<char> sepspaces(" ");
		//tokenizer used to split up strings
	tokenizer<char_separator<char> > token_spaces(test, sepspaces);
	BOOST_FOREACH(string t, token_spaces)
	{
		place.push_back(t); //pushback string into a vector
	}
	for(int i = 0; i < place.size(); ++i)
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
			{		//removing the quatation marks
				while(place.at(j).find('\"') < place.at(j).size())
				{
					place.at(j).erase(place.at(j).find('\"'), 1);
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
			if(counter == 2)
			{
				real.push_back("  ");
			}
			counter = 0;
		}
	}
	if (counter == 1)
	{
		real.push_back("  ");
	}
}
void colonoscopy(string &test) //we use this function to format our list guide
{				// this basically jsut add a space before a semi
	vector<int> destinations;
	for(int i = 0; i < test.size(); i++)
	{
		if (test.at(i) == ';')
		{
			destinations.push_back(i);
		}
	}
	for (int i = 0; i < destinations.size(); ++i)
	{
		test.insert(test.begin() + destinations.at(i) + i, ' ');
	}
}
bool letsdoit(vector<int> &aglist, vector<string> &plist) // logic function
{
// we set up the format as two vectors, one for arguments, and one for
// commands and their parameters (vector of ints and one of strings)
// from the tokenizer we set up single ls commands to have a space after
// echos have a string after and etc
	bool status = execute(plist.at(0), plist.at(1)); //execute first command
	for (int i = 0; i < aglist.size(); ++i) //main goal is to go through the
	{					//arguement list
		if(aglist.at(i) == 1) //the && argument
		{
			if(status == false) //check if first command was false
			{
				plist.erase(plist.begin()); //delete the current command
				plist.erase(plist.begin()); // does not update the status
			}				   //and doesn't execute
			else
			{
				plist.erase(plist.begin()); // deletes current command
				plist.erase(plist.begin()); // but executes and updates status
				status = execute(plist.at(0), plist.at(1));
			}
		}
		if(aglist.at(i) == 2) // the || argument
		{
			if(status == true)
			{
				plist.erase(plist.begin());
				plist.erase(plist.begin());
			}
			else
			{
				plist.erase(plist.begin());
				plist.erase(plist.begin());
				status = execute(plist.at(0), plist.at(1));
			}
		}
		if(aglist.at(i) == 3) //the ; argument
		{
			plist.erase(plist.begin());	
			plist.erase(plist.begin());
			status = execute(plist.at(0), plist.at(1));
		}
	}
}

void deletecomments(string &input) //this deletes comments
{
	int i = input.find('#'); // this takes the substring of everything before the
	if (i != 0 && i < input.size()) //"#" character if there's no '"' char before
	{
		if(input.at(i - 1) != 0 && i < input.find('"'))
		{
			input = input.substr(0, i);
		}
	}
	else if(i == 0 && i < input.size())
	{
		input.clear();
	}
}

int main()
{
	char* login = getlogin(); //getting the login
	char hostname[1024];	//hostname
	hostname[1023] = '\0'; //conventions for obtaining hostname
	int hostname_affirm = gethostname(hostname,30);
	string test;
	while(1)
	{
		if(login == NULL || hostname_affirm != 0) //checking if hostname is availiable
		cout << " $ ";	//Displaying the dollar prompt if hostname is availiable
		else
		cout << login << "@" << hostname << "$ "; //display prompt if prompt is availiable
		getline(cin,test);
		if(test == "exit")
			exit(1);
		vector<int> argumentlist;
		vector<string> parsedlist;
		deletecomments(test);
		CreateArgList(argumentlist,test);
		colonoscopy(test);
		parsethis(test, parsedlist);
		double size = parsedlist.size();
		if (argumentlist.size() != 0)
		{
			if (argumentlist.at(argumentlist.size()-1) == 3 &&
			   (argumentlist.size() != size/2 - 1))
			//takes out ; at the end because it doesn't do anything
			{
				argumentlist.pop_back();
			}
		}
		if (argumentlist.size() != size/2 -1) //checks for double args
		{
			cout << "bash; syntax error near unexpected token" << endl;
		}
		else
		{
			letsdoit(argumentlist, parsedlist);
		}
	}
	return 0;
}
	
