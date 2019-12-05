/**
 * Name: Joseph Lord
 * Program: Filesys.cpp
 * Class: CSE 461 Adv Operating Systems
 * Date: 2019-11-18
 * Purpose: Implementation of the Filesys class
**/

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include "Sdisk.h"
#include "Filesys.h"

using namespace std;

Filesys::Filesys(string diskname, int numberofblocks, int blocksize): Sdisk(diskname, numberofblocks, blocksize) {
	rootsize = getblocksize() / 13;
	fatsize = ((getnumberofblocks() * 4) / getblocksize()) + 1;	

	string buffer = "";
	getblock(1, buffer);	//gets the first block of the disk, which is the root directory
	
	if(buffer[1] == '#') {	//no filesystem, create and store
		//create root
		for(int i = 0; i < rootsize; i++) {
			filename.push_back("xxxxxxxx");	//8 x's stand for an open file spot
			firstblock.push_back(0);
		}
		
		//create FAT
		fat.push_back(2 + fatsize);	//initial data block
		fat.push_back(-1);	//unusable blocks

		for(int i = 0; i < fatsize; i++) {
			fat.push_back(-1);
		}

		for(int i = (2 + fatsize); i < getnumberofblocks(); i++) {
			fat.push_back(i + 1);	//adds the next contiguous block
		}

		fat[fat.size() - 1] = 0;	//the last block in the fat goes back to the beginning of the fat

		fssynch();	//to write back to the disk, use fssynch
	} else {	//filesystem exists, read it in
		//root directory is in buffer
		istringstream instream;
		instream.str(buffer);	//reads in the buffer

		for(int i = 0; i < rootsize; i++) {
			string f = "";
			int b = 0;

			instream >> f >> b;

			filename.push_back(f);
			firstblock.push_back(b);
		}

		//read in fat
		istringstream instream2;
		buffer.clear();

		for(int i = 0; i < fatsize; i++) {	//get all the fat blocks
			string b = "";
			getblock(2 + i, b);
			buffer += b;
		}

		instream2.str(buffer);

		for(int i = 0; i < getnumberofblocks(); i++) {	//read all the fat blocks
			int f = 0;
			instream2 >> f;

			fat.push_back(f);	
		}
	}
}

int Filesys::fsclose() {
	fssynch();	//closes the filesystem after writing??
}

int Filesys::fssynch() {
	//write root to disk
	ostringstream outstream2;
	
	for(int i = 0; i < rootsize; i++) {
		outstream2 << filename[i] << " " << firstblock[i] << " ";	
	}
	
	//dont need to use block(), root directory only takes up one block
	string buffer2 = outstream2.str();
	putblock(1, buffer2);
	
	//write fat to disk
	string buffer;
	ostringstream outstream;

	for(int i = 0; i < getblocksize(); i++) {
		outstream<< fat[i] <<" ";
	}
	
	buffer = outstream.str();
	vector<string> blocks = block(buffer, getblocksize());
	
	for(int i = 0;  i < blocks.size(); i++) {
		putblock(2 + i, blocks[i]);	//2 is the first block of the FAT
	}
}

int Filesys::newfile(string file) {	
	for(int i = 0; i < rootsize; i++) {
		if(filename[i] == "xxxxxxxx") {	//there is an available slot
			filename[i] = file;	//change "xxxxxxx" to the name of the file
			firstblock[i] = 0;	//change 0 to the first available block
			
			fssynch();	//write to disk
			return 1;
		}
	}
	
	return -1; //there is not available slot, or the file already exists	
}

int Filesys::rmfile(string file) {
	for(int i = 0; i < rootsize; i++) {
		if(filename[i] == file) {	//checks if file exists
			if(firstblock[i] == 0) {	//checks if file is empty
				filename[i] = "xxxxxxxx";
				
				fssynch();	//write to disk
				return 1;
			}
		}
	}
	
	return -1;	//file does not exist or file is not empty
}

int Filesys::getfirstblock(string file) {
	for(int i = 0; i < rootsize; i++) {
		if(filename[i] == file) {	//checks if file exists
			return firstblock[i];
		}
	}
	
	return -1;	//file does not exist
}

int Filesys::addblock(string file, string buffer) {
	int first = getfirstblock(file);
	
    if (first == -1) {
		//printf("%s does not exist!\n", file);
		cout << file << " does not exist!\n";
        return -1;	//file does not exist
    }
	
    int allocate = fat[0];
	
    if (allocate == 0) {     
		printf("No more space on the disk!\n");
        return -1;	//no more free data blocks
    }
	
    fat[0] = fat[fat[0]];    //Set fat[0] to next available block
    fat[allocate] = 0;
	
    if (first == 0) {    //If file exists, but is empty 
        for (int i = 0; i < rootsize; i++) {
			if(filename[i] == file) {
            	firstblock[i] = allocate;
			}
        }
    } else {          //If file exists and already uses some blocks
        int iblock = first;
		
        while (fat[iblock] != 0) {
            iblock = fat[iblock];
		}
		
        fat[iblock] = allocate;
    }
	
    fssynch();
    putblock(allocate, buffer);
	
    return allocate;
}

int Filesys::delblock(string file, int blocknumber) {
	if(!checkblock(file, blocknumber)) {
		return -1;	//blocknumber is not part of the file
	}

	int deallocate = blocknumber;

	if(deallocate == getfirstblock(file)) {	//blocknumber is first block in file
		for(int i = 0; i < filename.size(); i++) {
			if(file == filename[i]) {
				firstblock[i] = fat[deallocate];
				break;
			}
		}
	} else {
		int iblock = getfirstblock(file);

		while(fat[iblock] != deallocate) {
			iblock = fat[iblock];
		}

		//fat[iblock] == deallocate
		fat[iblock] = fat[deallocate];
	}

	fat[deallocate] = fat[0];
	fat[0] = deallocate;

	fssynch();
}

int Filesys::readblock(string file, int blocknumber, string& buffer) {
	if(checkblock(file, blocknumber)) {
		getblock(blocknumber, buffer);
		return 1;
	} else {
		return -1;	//block is not in that file
	}
}

int Filesys::writeblock(string file, int blocknumber, string buffer) {
	if(checkblock(file, blocknumber)) {
		putblock(blocknumber, buffer);
		return 1;
	} else {
		return -1;	//block is not in that file
	}
}

int Filesys::nextblock(string file, int blocknumber) {
	if(checkblock(file, blocknumber)) {
		return fat[blocknumber];
	} else {
		return -1;	//block is not in that file
	}

}
	   
bool Filesys::checkblock(string file, int blocknumber) {
	int iblock = getfirstblock(file);

	while(iblock >= 0) {
		if(iblock == blocknumber) {
			return true;	//block is in file
		}

		iblock = fat[iblock];	//next block in file
	}

	return false;	//block is not in file
}

vector<string> Filesys::ls() {
	vector<string> flist;
	
	for(int i = 0; i < filename.size(); i++) {
		if(filename[i] != "xxxxxxxx") {
			flist.push_back(filename[i]);
		}
	}
	
	return flist;
}