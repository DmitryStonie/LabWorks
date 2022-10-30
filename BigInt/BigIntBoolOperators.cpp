#include "BigInt.h"

bool BigInt::operator==(const BigInt& num_2) const {
	int num_1_digits = used_digits(*this);
	int num_2_digits = used_digits(num_2);
	if (sign != num_2.sign || num_1_digits != num_2_digits) return 0;
	for (int i = 0; i < num_1_digits; i++) {
		if (number[i] != num_2.number[i]) return 0;
	}
	return 1;
}

bool BigInt::operator!=(const BigInt& num_2) const {
	return !((*this) == num_2);
}

bool BigInt::operator<(const BigInt& num_2) const {
	int num_1_digits = used_digits(*this);
	int num_2_digits = used_digits(num_2);
	if ((sign == '-' && num_2.sign == '+')) return 1;
	else if ((sign == '+' && num_2.sign == '-')) return 0;
	else if (sign == '+' && num_2.sign == '+') {
		if (num_1_digits > num_2_digits) return 0;
		else if (num_1_digits < num_2_digits) return 1;
		else {
			for (int i = num_1_digits - 1; i >= 0; i--) {
				if (num_2.number[i] > number[i]) return 1;
				else if (num_2.number[i] < number[i]) return 0;
			}
			return 0;		//when equal
		}
	}
	else { //sign == '-' && num_2.sign == '-'
		if (num_1_digits > num_2_digits) return 1;
		else if (num_1_digits < num_2_digits) return 0;
		else {
			for (int i = num_1_digits - 1; i >= 0; i--) {
				if (num_2.number[i] < number[i]) return 1;
				else if (num_2.number[i] > number[i]) return 0;
			}
			return 0;		//when equal
		}
	}
	return 0;		//if i had a mistake - undefined behaviour
}

bool BigInt::operator>(const BigInt& num_2) const {
	if ((*this) == num_2 || (*this) < num_2) return 0;
	return 1;
}

bool BigInt::operator<=(const BigInt& num_2) const {
	if ((*this) == num_2 || (*this) < num_2) return 1;
	return 0;
}

bool BigInt::operator>=(const BigInt& num_2) const {
	if ((*this) < num_2) return 0;
	return 1;
}
