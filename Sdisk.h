/**
 * Name: Joseph Lord
 * Program: Sdisk.h
 * Class: CSE 461 Adv Operating Systems
 * Date: 2019-10-01
 * Purpose: Class the creates and manages a virtual storage disk
**/

#ifndef SDISK_H
#define SDISK_H

#include <string>
#include <vector>

using namespace std;

class Sdisk {
	public :
		Sdisk(string diskname, int numberofblocks, int blocksize);
		int getblock(int blocknumber, string& buffer);	//gets a block from the disk
		int putblock(int blocknumber, string buffer);	//puts a string into the disk
		int getnumberofblocks(); 						// accessor function
		int getblocksize(); 							// accessor function
		vector<string> block(string buffer, int b);		//blocks the buffer into a list of blocks of size b
	private :
		string diskname;        // file name of software-disk
		int numberofblocks;     // number of blocks on disk
		int blocksize;          // block size in bytes
};

#endif