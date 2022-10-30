#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include "BigInt.h"

using namespace std;

BigInt::BigInt() {
	number.resize(default_size);
	sign = '+';
}

BigInt::BigInt(int a) {
	number.resize(default_size);
	if (a < 0) {
		sign = '-';
		long long tmp = a;
		tmp = llabs(tmp);
		number[0] = (unsigned int)(tmp % base);
	}
	else {
		sign = '+';
		number[0] = (unsigned int)a;
	}
}

BigInt::BigInt(std::string str_num) {
	if (is_number(str_num) == 0) {
		sign = '+';
		throw std::invalid_argument("Error: not a number.\n");
	}
	string tmp = str_num;											
	int last_index = str_num.length() - 1, first_index = 0;
	for (int i = 0; i <= last_index; i++) tmp[i] -= 48;				//from chars to numbers
	sign = set_sign(str_num[0]);
	number.resize(default_size);
	if (str_num[0] == '-' || str_num[0] == '+') {					//if first + or -
		first_index = 1;
	}
	int bit = 0, carry = 0, new_carry = 0, BitsWritten = 0;
	for (int i = first_index; i <= last_index;) {
		carry = 0, new_carry = 0;
		for (int left_ind = i; left_ind <= last_index; left_ind++) {
			new_carry = (carry * 10 + tmp[left_ind]) % 2;
			tmp[left_ind] = (carry * 10 + tmp[left_ind]) / 2;
			carry = new_carry;
		}//in the end result bit to write in carry
		if (BitsWritten / 32 >= number.size()) {
			number.resize(number.size() + 1);
			number[number.size() - 1] = 0;
		}
		number[BitsWritten / 32] = number[BitsWritten / 32] | (carry << (BitsWritten % 32));
		BitsWritten++;
		for (; i <= last_index;) {
			if (tmp[i] == 0) {
				i++;
			}
			else {
				break;
			}
		}
	}
	if (is_zero(*this)) {
		sign = '+';
	}
}

BigInt::BigInt(const BigInt& num_2) {
	sign = num_2.sign;
	number = num_2.number;
}

BigInt::~BigInt() {

}