/**
 * Name: Joseph Lord
 * Program: Filesys.h
 * Class: CSE 461 Adv Operating Systems
 * Date: 2019-10-22
 * Purpose: Class the creates and manages a filesystem
**/

#ifndef FILESYS_H
#define FILESYS_H

#include <string>
#include <vector>
#include "Sdisk.h"

using namespace std;

class Filesys: public Sdisk {
	public:
		Filesys(string diskname, int numberofblocks, int blocksize);
		int fsclose();		//writes FAT and ROOT to the sdisk, closing the sdisk
		int newfile(string file);	//adds an entry for the string file in ROOT
		int rmfile(string file);	//removes the entry file from ROOT if the file is empty
		int getfirstblock(string file);	//returns the block number of the first block in file
		int addblock(string file, string buffer);	//adds a block of data stored in the string buffer to the end of file
		int delblock(string file, int blocknumber);	//removes block numbered blocknumber from file
		int readblock(string file, int blocknumber, string& buffer);	//gets blocknumber from file and stores the data
		int writeblock(string file, int blocknumber, string buffer);	//writes the buffer to the blocknumber in file
		int nextblock(string file, int blocknumber);	//returns the number of the block that follows blocknumber in file
		vector<string> ls();	//returns a vector with all the active file names
	private:
		int rootsize;           	//maximum number of entries in ROOT
		int fatsize;            	//number of blocks occupied by FAT
		vector<string> filename;   	//filenames in ROOT
		vector<int> firstblock;		//firstblocks in ROOT
		vector<int> fat;			//file allocation table
		int fssynch();				//writes FAT and ROOT to the sdisk
		bool checkblock(string file, int blocknumber);	//checks if the block is part of the file
};

#endif