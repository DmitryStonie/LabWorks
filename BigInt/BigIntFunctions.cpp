#include <string>
#include "BigInt.h"
#include <iostream>

int BigInt::is_number(std::string& str_num) {
	if (str_num.length() == 0) return 0;
	if ((str_num[0] == '+' || str_num[0] == '-' || (str_num[0] >= '0' && str_num[0] <= '9')) == 0) return 0;
	for (int i = 1; i < str_num.size(); i++) {
		if ((str_num[i] >= '0' && str_num[i] <= '9') == 0) return 0;
	}
	return 1;
}

char BigInt::set_sign(char symbol) {
	if (symbol == '-') return '-';
	else if (symbol == '+' || (symbol >= '0' && symbol <= '9')) return '+';
	return 0; //i don't use it
}

int BigInt::digits_in_highest_digit(BigInt& num, int used_digits) {	//returns number of used digits in highest unzero digit
	int i = 0;
	unsigned int tmp = num.number[used_digits - 1];
	for (; tmp > 0;) {
		tmp = tmp >> 1;
		i++;
	}
	if (used_digits == 1 && i == 0) return 1;		//	when number = zero
	return i;
}

int BigInt::digits_in_highest_digit(const BigInt& num, int used_digits) {	//returns number of used digits in highest unzero digit
	int i = 0;
	unsigned int tmp = num.number[used_digits - 1];
	for (; tmp > 0;) {
		tmp = tmp >> 1;
		i++;
	}
	if (used_digits == 1 && i == 0) return 1;		//	when number = zero
	return i;
}

int BigInt::digits_in_highest_digit(const BigInt& num, int used_digits) const {	//returns number of used digits in highest unzero digit
	int i = 0;
	unsigned int tmp = num.number[used_digits - 1];
	for (; tmp > 0;) {
		tmp = tmp >> 1;
		i++;
	}
	if (used_digits == 1 && i == 0) return 1;		//	when number = zero
	return i;
}

int BigInt::used_digits(BigInt& num) {	//return number of highest unzero digit
	int i = num.number.size() - 1;
	if (i == 0) return 1;
	for (; i >= 0;) {
		if (num.number[i] == 0) {
			i--;
		}
		else break;
	}
	if (i == -1) i = 0;
	return i + 1;
}

int BigInt::used_digits(const BigInt& num) {	//return number of highest unzero digit
	int i = num.number.size() - 1;
	if (i == 0) return 1;
	for (; i >= 0;) {
		if (num.number[i] == 0) {
			i--;
		}
		else break;
	}
	if (i == -1) i = 0;
	return i + 1;
}

int BigInt::used_digits(const BigInt& num) const {	
	int i = num.number.size() - 1;
	if (i == 0) return 1;
	for (; i >= 0;) {
		if (num.number[i] == 0) {
			i--;
		}
		else break;
	}
	if (i == -1) i = 0;
	return i + 1;
}

void BigInt::carry(BigInt& number, long long new_number, int num_index) { //carry ����������
	int num_size = number.number.size();
	int num_used_digits = used_digits(*this);
	if (num_index == num_size - 1 && new_number >= base) {//add new digit
		number.number.resize(num_size + 1);
		num_size++;
	}
	if (new_number >= base) { //carry up //need to find bigger nonzero digit
		long long tmp;
		for (int j = num_index + 1; j < num_size; j++) {
			tmp = (long long)number.number[j] + 1;
			if (tmp >= base) {
				number.number[j] = (unsigned int)(tmp % base);
			}
			else {
				number.number[j] = (unsigned int)tmp;
				break;
			}
		}
		number.number[num_index] = (unsigned int)new_number % base;
	}
	else if (new_number < 0) {
		for (int j = num_index + 1; j < num_used_digits; j++) {	//fing higher nonzero digit and carry 1 down through digits between
			if (number.number[j] > 0) {
				number.number[j]--;
				for (int k = j - 1; k >= num_index; k--) {	//carry 1 down
					number.number[k] = (unsigned int)(number.base - 1);
				}
				break;
			}
		}
		number.number[num_index] += new_number + 1;	// +1 because there are base - 1 but should be base
	}
	else {
		number.number[num_index] = (unsigned int)new_number;
	}
}

void BigInt::carry(BigInt& number, long long new_number, int num_index) const { //carry ����������
	int num_size = number.number.size();
	int num_used_digits = used_digits(*this);
	if (num_index == num_size - 1 && new_number >= base) {//add new digit
		number.number.resize(num_size + 1);
		num_size++;
	}
	if (new_number >= base) { //carry up //need to find bigger nonzero digit
		long long tmp;
		for (int j = num_index + 1; j < num_size; j++) {
			tmp = (long long)number.number[j] + 1;
			if (tmp >= base) {
				number.number[j] = (unsigned int)(tmp % base);
			}
			else {
				number.number[j] = (unsigned int)tmp;
				break;
			}
			if (j == num_size - 1 && tmp >= base) {
				number.number.resize(num_size + 1);
				num_size++;
			}
		}
		number.number[num_index] = (unsigned int)new_number % base;
	}
	else if (new_number < 0) {
		for (int j = num_index + 1; j < num_used_digits; j++) {	//fing higher nonzero digit and carry 1 down through digits between
			if (number.number[j] > 0) {
				number.number[j]--;
				for (int k = j - 1; k >= num_index; k--) {	//carry 1 down
					number.number[k] = (unsigned int)(number.base - 1);
				}
				break;
			}
		}
		number.number[num_index] += new_number + 1;	// +1 because there are base - 1 but should be base
	}
	else {
		number.number[num_index] = (unsigned int)new_number;
	}
}

void IncreaseDecNum(vector<int>& number, int bit) {
	for (int i = 0; i < number.size(); i++) {
		if (number[i] >= 1000000000) {
			if (i + 1 == number.size()) {
				number.resize(number.size() + 1);
				number[number.size() - 1] = 0;
			}
			number[i + 1] += 1;
			number[i] = number[i] % 1000000000;
		}
		number[i] = number[i] * 2;
		if (number[i] >= 1000000000) {
			if (i + 1 == number.size()) {
				number.resize(number.size() + 1);
				number[number.size() - 1] = 0;
			}
			number[i + 1] += 1;
			number[i] = number[i] % 1000000000;
		}
	}
	if (bit == 1) {
		number[0]++;
		for (int i = 0; number[i] >= 1000000000; i++) {
			if (i + 1 == number.size()) number.resize(number.size() + 1);
			number[number.size() - 1] = 0;
			number[i + 1] += 1;
			number[i] = number[i] % 1000000000;
		}
	}
}

void BigInt::carry_mul(BigInt& number, unsigned long long new_number, int num_index) { //carry ����������
	int num_size = number.number.size();
	if (new_number >= number.base) { //carry up //need to find bigger nonzero digit
		number.number[num_index] = (unsigned int)(new_number % (unsigned long long)number.base);
		unsigned long long tmp = new_number;
		for (int j = num_index + 1; j < num_size; j++) {
			tmp = tmp / number.base + (long long)number.number[j];
			if (tmp >= number.base) {
				number.number[j] = (unsigned int)(tmp % number.base);
			}
			else {
				number.number[j] = (unsigned int)tmp;
				break;
			}
		}
	}
	else {
		number.number[num_index] = (unsigned int)new_number;
	}
}

int is_zero(BigInt& number) {
	for (int i = number.number.size() - 1; i >= 0; i--) {
		if (number.number[i] != 0) {
			return 0;
		}
	}
	return 1;
}

int is_zero(const BigInt& number) {
	for (int i = number.number.size() - 1; i >= 0; i--) {
		if (number.number[i] != 0) {
			return 0;
		}
	}
	return 1;
}

void BigInt::twos_complement(BigInt& number) {
	for (int i = 0; i < number.number.size(); i++) {
		number.number[i] = ~number.number[i];
	}
	carry(number, ((long long)number.number[0]) + 1, 0);
}