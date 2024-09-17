#include "main.h"

/*
class CStrings {
	public:
		CStrings();
		~CStrings();
		void AddString(const char*);
		char* GetString(unsigned int);
		char* operator[](unsigned int);
		unsigned int GetSize();
		void ClearList();

	private:
		char** list;
		unsigned int listsize;
};
*/

CStrings::CStrings() {
	this->list = NULL;
	this->listsize = 0;
}

CStrings::~CStrings() {
	//delete each string in array
	unsigned int i = 0;
	while (i < this->listsize) {
		delete [] (this->list)[i];
		++i;
	}
	//if an array existed, delete it too
	if (this->list) delete [] this->list;

	this->list = NULL;
	this->listsize = 0;
}

void CStrings::AddString(const char* newstr) {
	//get new array size
	int newsize = this->listsize + 1;
	//make new array one larger than current
	char** temp = new char*[newsize];

	//copy over old strings into new array
	unsigned int i = 0;
	while(i < this->listsize) {
		//set new string to the old one
		temp[i] = (this->list)[i];new char[strlen((this->list)[i]) + 1];
		++i;
	}
	//intialize newest string
	temp[i] = new char[strlen(newstr) + 1];
	//copy
	strcpy(temp[i], newstr);
	//null terminator
	temp[i][strlen(newstr)] = 0;

	//delete current strings and array
	if (this->list) delete [] (this->list);
	//assign this list to the new one
	this->list = temp;
	//increment internal size counter
	++(this->listsize);
}

const char* CStrings::GetString(unsigned int index) {
	if (index < 0 || index >= this->listsize) return NULL;
	return (this->list)[index];
}

const char* CStrings::operator[](unsigned int index) {
	return (this->GetString(index));
}

unsigned int CStrings::GetSize() {
	return this->listsize;
}
