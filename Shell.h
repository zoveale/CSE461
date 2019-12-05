/**
 * Name: Joseph Lord
 * Program: Filesys.h
 * Class: CSE 461 Adv Operating Systems
 * Date: 2019-11-18
 * Purpose: Class the takes user commands and runs them in the filesystem.
**/

#ifndef SHELL_H
#define SHELL_H

#include <string>
#include <vector>
#include "Sdisk.h"
#include "Filesys.h"

using namespace std;

class Shell: public Filesys {
	public:
		Shell(string diskname, int numberofblocks, int blocksize);
		int dir();		//lists all files
		int add(string file);	//add a new file using input from the keyboard
		int del(string file);	//deletes the file
		int type(string file);	//lists the contents of file
		int copy(string file1, string file2);	//copies file1 to file2
		int clobber(string file);	//removes all blocks from a file, leaving an empty file
};

#endif