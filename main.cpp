#include <iostream>
#include <string>
#include "Sdisk.h"
#include "Filesys.h"
#include "Shell.h"

using namespace std;

int main()
{
 //
 //This main program inputs commands to the shell.
 //It inputs commands as : command op1 op2
 //You should modify it to work for your implementation.
 //
	string s;
	string command="go";
	string op1,op2;
	
	//Sdisk disk1("disk1",256,128);
  	//Filesys fsys("disk1",256,128);
	Shell cmdline("disk1",256,128);

	while (command != "quit") {
       command.clear();
       op1.clear();
       op2.clear();
       
		cout << "student@labOS$ ";
       getline(cin,s);
       
		int firstblank=s.find(' ');
       	if (firstblank < s.length()) s[firstblank]='#';
       
		int secondblank=s.find(' ');
       	command=s.substr(0,firstblank);
		
       if (firstblank < s.length())
         op1=s.substr(firstblank+1,secondblank-firstblank-1);
		
       if (secondblank < s.length())
         op2=s.substr(secondblank+1);
		
       if (command=="dir") {
            cmdline.dir();
       }
       if (command=="add") {
            cmdline.add(op1);
       }
       if (command=="del") {
            cmdline.del(op1);
       }
       if (command=="type") {
            cmdline.type(op1);
       }
       if (command=="copy") {
            cmdline.copy(op1, op2);
       }
       
      }

	return 0;
}
