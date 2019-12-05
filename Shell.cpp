#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include "Sdisk.h"
#include "Filesys.h"
#include "Shell.h"

using namespace std;

Shell::Shell(string diskname, int numberofblocks, int blocksize): Filesys(diskname, numberofblocks, blocksize) {
	
}

int Shell::dir() {
	vector<string> flist = ls();
	
	for(int i = 0; i < flist.size(); i++) {
		//printf("%s\n", flist);
		cout << flist[i] << endl;
	}
	
	return 0;
}

int Shell::add(string file) {
	int code = newfile(file);
	
	if(code == -1) {
		//printf("%s already exists!\n", file);
		cout << file << " already exists!\n";
		return 0;
	}
	
	string b;
	
	//printf("%s: \n", file);
	cout << file << ": " << endl;
	getline(cin, b);
	
	vector<string> blocks = block(b, getblocksize());
	
	for(int i = 0; i < blocks.size(); i++) {
		int code2 = addblock(file, blocks[i]);
		
		if(code2 == -1) {
			return 0;
		}
	}
}

int Shell::del(string file) {
	int code = clobber(file);
	
	if(code != 0) {
		rmfile(file);
		return 1;
	}
	
	return 0;
}

int Shell::type(string file) {
	int code = getfirstblock(file);
	
	if(code == -1) {
		//printf("No such file '%s' exists!\n", file);
		cout << "No such file exists!\n";
		return 0;
	}
	
	int iblock = code;	//the first block of the source file
	string buffer = "";
	
	while(iblock != 0) {
		string b;
		getblock(iblock, b);
		
		//compile the entire file
		buffer += b;
		
		iblock = nextblock(file, iblock);
	}
	
	//printf("%s: \n%s\n", file, buffer);	//print the contents of the file
	cout << file << ": \n" << buffer << endl;
	return 1;	//all is good
}

int Shell::copy(string file1, string file2) {
	int code = getfirstblock(file1);
	
	if(code == -1) {
		//printf("No such file '%s' exists!\n", file1);
		cout << "No such file exists!\n";
		return 0;
	}
	
	int code2 = getfirstblock(file2);
	
	if(code2 != -1) {
		//printf("%s already exists!\n", file2);
		cout << file2 << " already exists!\n";
		return 0;
	}
	
	int code3 = newfile(file2);
	
	if(code3 == -1) {
		printf("No space left in the root directory!\n");
		return 0;
	}
	
	int iblock = code;	//the first block of the source file
	
	while(iblock != 0) {
		string b;
		getblock(iblock, b);
		
		int code4 = addblock(file2, b);
		
		if(code4 == -1) {
			printf("No space left on disk!");
			del(file2);
			
			return -1;
		}
		
		iblock = nextblock(file1, iblock);
	}
	
	return 1;	//all is good
}

int Shell::clobber(string file) {
	int iblock = getfirstblock(file);
	
	if(iblock == -1) {
		cout << "File '" << file << "' already exists!\n";
		return 0;
	}
	
	if(iblock != 0) {
		delblock(file, iblock);
		clobber(file);	//recursive
	}
	
	return 1;
}