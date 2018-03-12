#pragma once
#include <string>
#include <queue>

using namespace std;


class string_generator {
public:
	~string_generator();
	string_generator();
	string* getNext();
	string_generator::string_generator(const char* str, const char* delimiter);
	bool is_end();
	int size();
private:
	queue< string* > items;
};

