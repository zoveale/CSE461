/**
 * Name: Joseph Lord
 * Program: Sdisk.cpp
 * Class: CSE 461 Adv Operating Systems
 * Date: 2019-10-04
 * Purpose: Implementation of the Sdisk class
**/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "Sdisk.h"

using namespace std;

Sdisk::Sdisk(string diskname, int numberofblocks, int blocksize) {
	//initialize all variables
	this->diskname = diskname;
	this->numberofblocks = numberofblocks; 
	this->blocksize = blocksize;
	
	//open the vdisk
	fstream iofile;
	iofile.open(diskname.c_str());
	
	//format the vdisk if it is empty
	if(!iofile.good()) {
		iofile.open(diskname.c_str(), ios::out);
		
		for(int i = 0; i < (blocksize * numberofblocks); i++) {
			iofile.seekp(i, ios::beg);
			iofile.put('#'); 
		}
	}
	
	//close the vdisk
	iofile.close();
}

int Sdisk::getblock(int blocknumber, string & buffer) {
	//open the vdisk
	fstream iofile;
	iofile.open(diskname.c_str(), ios::in | ios::out);
	
	//initialize variables
	int pos = blocknumber * blocksize;	//start of the block
	char c = ' ';	//buffer for the buffer
	
	//get the block from the file and put it into the buffer
	iofile.seekg(pos, ios::beg);
	
	for(int i = 0; i < blocksize; i++) {
		iofile.get(c);
		buffer += c;
	}
	
	//close the vdisk
	iofile.close();
	return 1;
}

int Sdisk::putblock(int blocknumber, string buffer) {
	if (blocknumber > numberofblocks) {	//checks if blocknumber is valid
    	cout << "Blocknumber is invalid.\n";
        return -1;
    }	
	
	//open the vdisk
	fstream iofile;
	iofile.open(diskname.c_str(), ios::in | ios::out);
	
	//initialize variables
	int pos = blocknumber * blocksize;	//start of the block
	
	//put the buffer onto the vdisk
	iofile.seekp(pos, ios::beg);
	
	for(int i = 0; i < buffer.length() && i < blocksize; i++, pos++) {	
		iofile.put(buffer[i]);
	}
	
	//close the vdisk
	iofile.close();
	return 1;
	
}

int Sdisk::getnumberofblocks() {
	return numberofblocks;
}

int Sdisk::getblocksize() {
	return blocksize;
}

vector<string> Sdisk::block(string buffer, int b) {
	//initialize variables
	vector<string> blocks;
	int numofblocks = 0;
	string tempblock;

	//figure out the number of blocks the buffer will need
	if (buffer.length() % b == 0) { 
		numofblocks = buffer.length() / b;
	} else { 
		numofblocks = (buffer.length() / b) + 1;
	}

	//break the buffer into blocks, stops before the last block
	for (int i = 0; i < numofblocks; i++) { 
		tempblock = buffer.substr(b * i,b);
		  blocks.push_back(tempblock);
	}
	
	//finds the last block
	int lastblock = blocks.size() - 1;

	//puts the last of the buffer into the last block
	for (int i = blocks[lastblock].length(); i < b; i++) { 
		blocks[lastblock] += "#";
	}

	//return the buffer in a vector of blocks
	return blocks; 
}
