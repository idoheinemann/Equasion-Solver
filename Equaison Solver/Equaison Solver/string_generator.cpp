#include <string>
#include <iostream>
#include <queue>
#include "string_generator.h"
using namespace std;


string_generator::~string_generator() {

}

string_generator::string_generator() {

}

string_generator::string_generator(const char* str, const char* delimiter) {
	string res("");
	register int index = 0;
	while ((int)*(str + index) != 0) {
		if (*(str + index) == *delimiter) {
			register int ind = 0;
			bool split = true;
			while ((int)*(delimiter + ind) != 0) {
				if (*(delimiter + ind) != *(str + index + ind)) {
					split = false;
					break;
				}
				ind++;
			}
			if (split) {
				items.push(new string(res));
				index += ind;
				res = "";
			}
		}
		res = res + string({ *(str + index) });
		index++;
	}
	items.push(new string(res));
}

bool string_generator::is_end() {
	return items.empty();
}

int string_generator::size() {
	return items.size();
}

string* string_generator::getNext() {
	if (is_end()) {
		delete this;
		return new string("");
	}
	string* temp = items.front();
	items.pop();
	return temp;
}