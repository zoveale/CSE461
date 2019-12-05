/**
 * Name: Joseph Lord
 * Program: Table.cpp
 * Class: CSE 461 Adv Operating Systems
 * Date: 2019-10-22
 * Purpose: Implementation of the Table class
**/

#include "Sdisk.h"
#include "Filesys.h"
#include "Table.h"

using namespace std;

Table::Table(string diskname, int numberofblocks, int blocksize, string flatfile, string indexfile): Filesys(diskname, numberofblocks, blocksize) {
	newfile(flatfile);
	newfile(indexfile);
	
	numberofrecords = 0;
}

int Table::buildtable(string inputfile) {
	ifstream infile;
	infile.open(inputfile.c_str());

	string record;
	int count = 0;

	infile.getline() >> record;

	while(infile.good()) {
		
	}
}

int Table::search(string value) {
	
}
	
int Table::indexsearch(string value) {
	
}