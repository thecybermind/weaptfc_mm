#ifndef CM_CSTRINGS_H
#define CM_CSTRINGS_H

class CStrings {
	public:
		CStrings();
		~CStrings();
		void AddString(const char*);
		const char* GetString(unsigned int);
		const char* operator[](unsigned int);
		unsigned int GetSize();
		void ClearList();

	private:
		char** list;
		unsigned int listsize;
};

#endif //CM_CSTRINGS_H
