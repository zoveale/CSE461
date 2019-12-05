/**
 * Name: Joseph Lord
 * Program: Filesys.h
 * Class: CSE 461 Adv Operating Systems
 * Date: 2019-10-22
 * Purpose: Class the creates and manages a filesystem
**/

#ifndef TABLE_H
#define TABLE_H

#include "Sdisk.h"
#include "Filesys.h"

using namespace std;

class Table: public FileSys 
{
	public :
		Table(string diskname, int blocksize, int numberofblocks, string flatfile, string indexfile);
		int buildtable(string inputfile);
		int search(string value);
	private :
		string flatfile;
		string indexfile;
		int numberofrecords;
		FileSys filesystem;
		
		int indexsearch(string value);
};

#endif