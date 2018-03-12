#include <iostream>
#include <string>
#include <regex>
#include "string_generator.h"

using namespace std;

const string DIGIT = "-?\\d+(\\.\\d+)?(E-?\\d+)?";

class matrix {
public:
	matrix(double* raw_mat[],int rows,int columns) {
		this->rows = rows;
		this->columns = columns;
		mat = raw_mat;
	}
	double get_element(int row,int column) {
		return *((*(mat + row)) + column);
	}
	double* get_row(int row) {
		return *(mat + row);
	}
	void add_to_row(int row,double* origrow, double inc) {
		for (register int i = 0; i != columns; i++) {
			*((*(mat + row)) + i) += *(origrow + i) * inc;
		}
	}
	void add_to_row(int row, int addrow, double inc) {
		for (register int i = 0; i != columns; i++) {
			*((*(mat + row)) + i) += *((*(mat + addrow))+ i) * inc;
		}
	}
	void mul_row(int row,double inc) {
		for (register int i = 0; i != columns; i++) {
			*((*(mat + row)) + i) *= inc;
		}
	}
	int rows;
	int columns;
private:
	double** mat;
};

string prepare(string s) {
	string ret = regex_replace(s, regex("-"), "+-");
	ret = regex_replace(ret, regex("\\s"), "");
	
	return ret;
}

double get_inc(string& s, string op) {
	smatch match;
	double i = 0;
	while (regex_search(s, match, regex(DIGIT + op))) {
		for (string x : match) {
			if (!x.length()) {
				break;
			}
			i += stod(regex_replace(x, regex(op), ""));
		}
		s = match.suffix().str();
	}
	return i;
}
double* parse_row(string s,string* vars,int size) {
	double* result = new double[size+1];
	string_generator gen(s.c_str(),"=");
	string first = *gen.getNext();
	string second = *gen.getNext();
	for (register int i = 0; i != size; i++) {
		result[i] = get_inc(first, vars[i]) - get_inc(second,vars[i]);
	}
	result[size] = get_inc(second, string("")) - get_inc(first, string(""));
	return result;
}
void fix_column(matrix& mat,int column) {
	if (!mat.get_element(column,column)) {
		int ind = -1;
		for (int i = 0; i != mat.rows; i++) {
			if (mat.get_element(i, column)) {
				ind = i;
				break;
			}
		}
		if (ind == -1) {
			throw "Mismatch argument, equaison is not a triple equaison";
			return;
		}
		mat.add_to_row(column, ind, 1);
	}
	mat.mul_row(column,1/mat.get_element(column,column));
	for (int i = 0; i != mat.rows; i++) {
		if (i == column) {
			continue;
		}
		mat.add_to_row(i,column,-1*mat.get_element(i,column));
	}
}

int main() {
	string buff;
	cout << "Enter the name of all the variables seperated by a ','" << endl;
	cin >> buff;
	buff = prepare(buff);
	string_generator all_vars(buff.c_str(), ",");
	int size = all_vars.size();
	string* variables = new string[size]();
	int i = 0;
	while (!all_vars.is_end()) {
		variables[i] = *all_vars.getNext();
		i++;
	}
	string* equaisons = new string[size]();
	cout << "Enter equaisons" << endl;
	for (i = 0; i != size; i++) {
		cin >> equaisons[i];
	}
	double** rows = new double*[size]();
	for (i = 0; i != size; i++) {
		rows[i] = parse_row(equaisons[i],variables,size);
	}
	matrix full_eq(rows, size, size+1);
	for (int i = 0; i != full_eq.rows; i++) {
		fix_column(full_eq, i);
	}
	for (i = 0; i != size; i++) {
		cout << variables[i] << " = " << full_eq.get_element(i, size) << endl;
	}

	cin >> buff;
	return 0;
}