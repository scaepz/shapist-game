#ifndef FILEREADER_H
#define FILEREADER_H
#include <string>
using std::string;
class CFileReader
{
public:
	CFileReader();
	virtual ~CFileReader();
	string GetRow(const string & file, int row); //Digs through string and returns a string containing the row specified in int row
};

#endif