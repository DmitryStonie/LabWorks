#include <iostream>
#include <exception>
#include "BigInt.h"

BigInt& BigInt::operator=(const BigInt& num_2) {
	sign = num_2.sign;
	number = num_2.number;
	return *this;
}

BigInt BigInt::operator~() const {
	BigInt tmp = (*this);
	if (sign == '+') {
		tmp.sign = '-';
		carry(tmp, ((long long)tmp.number[0]) + 1, 0);
	}
	else {
		tmp.sign = '+';
		carry(tmp, ((long long)tmp.number[0]) - 1, 0);
	}
	return tmp;
}

BigInt& BigInt::operator++() {
	if (sign == '+') carry(*this, ((long long)number[0]) + 1, 0);
	else carry(*this, ((long long)number[0]) - 1, 0);
	return *this;
}

const BigInt BigInt::operator++(int) {
	const BigInt temp = *this;
	if (sign == '+') carry(*this, ((long long)number[0]) + 1, 0);
	else carry(*this, ((long long)number[0]) - 1, 0);
	return temp;
}

BigInt& BigInt::operator--() {
	if (sign == '-') carry(*this, ((long long)number[0]) + 1, 0);
	else carry(*this, ((long long)number[0]) - 1, 0);
	return *this;
}

const BigInt BigInt::operator--(int) {
	const BigInt temp = *this;
	if (sign == '-') carry(*this, ((long long)number[0]) + 1, 0);
	else carry(*this, ((long long)number[0]) - 1, 0);
	return temp;
}

BigInt& BigInt::operator+=(const BigInt& num_2) {
	int num_2_digits = used_digits(num_2);
	if (number.size() < num_2_digits) {
		number.resize(num_2_digits);
	}
	long long tmp = 0;
	if ((*this).sign == num_2.sign) {
		for (int i = 0; i < num_2_digits; i++) {
			tmp = (long long)(*this).number[i] + (long long)num_2.number[i];
			carry((*this), tmp, i);
		}
	}
	else {
		BigInt tmp_num(*this);
		if (sign == '+' && ((*this) < -num_2)) {
			*this = num_2;
		}
		else if (sign == '+' && ((*this) >= -num_2)) {
			tmp_num = num_2;
		}
		else if(sign == '-' && (-(*this) < num_2)){
			*this = num_2;
		}
		else { //sign == '-' && (-(*this) >= num_2)
			tmp_num = num_2;
		}
		for (int i = 0; i < num_2_digits; i++) {
			tmp = (long long)(*this).number[i] - (long long)tmp_num.number[i];
			carry((*this), tmp, i);
		}
	}
	if (is_zero(*this)) sign = '+';
	return *this;
}

BigInt& BigInt::operator*=(const BigInt& num_2) {
	int num_1_digits = used_digits(*this);
	int num_2_digits = used_digits(num_2);
	unsigned long long tmp = 0;
	BigInt num_1_tmp;
	BigInt num_2_tmp;
	if (num_1_digits >= num_2_digits) {
		num_1_tmp = *this;
		num_2_tmp = num_2;
	}
	else {
		num_1_tmp = num_2;
		num_2_tmp = *this;
	}
	num_1_digits = used_digits(num_1_tmp);
	num_2_digits = used_digits(num_2_tmp);
	BigInt answer;
	answer.number.resize(num_1_digits + num_2_digits);
	for (int i = 0; i < num_2_digits; i++) {	//digit of second number
		for (int j = 0; j < num_1_digits; j++) {	//digits of first number
			tmp = (long long)(num_1_tmp.number[j]) * (long long)(num_2_tmp.number[i]) + (long long)(answer.number[i + j]);
			carry_mul(answer, tmp, i + j);
		}
	}
	if (sign != num_2.sign) {
		answer.sign = '-';
	} else answer.sign = '+';
	if (is_zero(answer)) answer.sign = '+';
	*this = answer;
	return *this;
}

BigInt& BigInt::operator-=(const BigInt& num_2) {
	BigInt tmp = num_2;
	tmp = -tmp;
	*this += tmp;
	return *this;
}

BigInt& BigInt::operator/=(const BigInt& num_2) {	//remainder is always 0 <= r < b
	int num_1_digits = used_digits(*this);
	int num_2_digits = used_digits(num_2);
	BigInt tmp_answer(0);
	BigInt tmp_1(*this);
	BigInt tmp_2(num_2);
	if (is_zero(num_2)) {
		throw std::exception("Error: zero division.\n");
	}
	if (sign == '-')tmp_1.sign = '+';
	if (num_2.sign == '-')tmp_2.sign = '+';
	if (tmp_2 > tmp_1) {
		*this = tmp_answer;
		return *this;
	}
	long long left_bound = 0;
	long long right_bound = 0;
	long long current_multiplier = 0;
	tmp_answer.number.resize(num_1_digits - num_2_digits + 1);
	int i = 0;
	if (num_1_digits > num_2_digits )i = num_1_digits - num_2_digits;//why not -1?
	if (i > 0) {
		tmp_answer.number[i] = 1;
		if (tmp_answer * tmp_2 > tmp_1) {
			tmp_answer.number[i] = 0;
			i--;
		}
		else{
			tmp_answer.number[i] = 0;
		}
	}
	if (i < 0) i = 0;
	for(; i >= 0; i--) {
		left_bound = -1;	//set digit of tmp_answer
		right_bound = base - 1;
		for (; left_bound < right_bound - 1;) {
			current_multiplier = (left_bound + right_bound) / 2;
			tmp_answer.number[i] = current_multiplier;
			if (tmp_answer * tmp_2 <= tmp_1) {
				left_bound = current_multiplier;
			}
			else{
				right_bound = current_multiplier;
			}
		}
		tmp_answer.number[i] = left_bound;
		if (tmp_answer * tmp_2 > tmp_1) {
			tmp_answer.number[i] = 0;
		}
	}
	if (sign != num_2.sign) tmp_answer.sign = '-';
	else tmp_answer.sign = '+';
	if ((*this) - (tmp_answer * num_2) < BigInt(0)) {//for positive reminder
		tmp_answer += 1;
	}
	if (is_zero(tmp_answer)) tmp_answer.sign = '+';
	*this = tmp_answer;
	return *this;
}

BigInt& BigInt::operator^=(const BigInt& num_2) {
	int num_2_digits = used_digits(num_2);
	int num_1_digits = used_digits(*this);
	BigInt num_1_tmp(*this);//need to create copies and make ones complement
	BigInt num_2_tmp(num_2);
	if (num_1_digits < num_2_digits) {
		num_1_tmp.number.resize(num_2_digits);
		num_1_digits = num_2_digits;
	}
	else {
		num_2_tmp.number.resize(num_1_digits);
		num_2_digits = num_1_digits;
	}
	if (num_1_tmp.sign == '-') {
		twos_complement(num_1_tmp);
	}
	if (num_2_tmp.sign == '-') {
		twos_complement(num_2_tmp);
	}
	for (int i = 0; i < num_2_digits; i++) {	//-1 due to coorrect work in highest digit
		num_1_tmp.number[i] ^= num_2_tmp.number[i];
	}
	if ((num_1_tmp.sign == '-' && num_2_tmp.sign == '+') || (num_2_tmp.sign == '-' && num_1_tmp.sign == '+')) {	//inverse
		twos_complement(num_1_tmp);
	}
	if (num_1_tmp.sign ==  num_2_tmp.sign) num_1_tmp.sign = '+';
	else num_1_tmp.sign = '-';
	*this = num_1_tmp;
	return *this;
}

BigInt& BigInt::operator%=(const BigInt& num_2) {	//remainder is always 0 <= r < b
	BigInt tmp(*this);
	tmp = tmp - (tmp / num_2) * num_2;
	(*this) = tmp;
	return *this;
}

BigInt& BigInt::operator&=(const BigInt& num_2) {
	int num_2_digits = used_digits(num_2);
	int num_1_digits = used_digits(*this);
	BigInt num_1_tmp(*this);//need to create copies and make ones complement
	BigInt num_2_tmp(num_2);
	if (num_1_digits < num_2_digits) {
		num_1_tmp.number.resize(num_2_digits);
		num_1_digits = num_2_digits;
	}
	else {
		num_2_tmp.number.resize(num_1_digits);
		num_2_digits = num_1_digits;
	}
	if (num_1_tmp.sign == '-') {
		twos_complement(num_1_tmp);
	}
	if (num_2_tmp.sign == '-') {
		twos_complement(num_2_tmp);
	}
	for (int i = 0; i < num_2_digits; i++) {	//-1 due to coorrect work in highest digit
		num_1_tmp.number[i] &= num_2_tmp.number[i];
	}
	if (num_1_tmp.sign == '-' && num_2_tmp.sign == '-') {	//inverse
		twos_complement(num_1_tmp);
	}
	if (num_1_tmp.sign == '-' && num_2_tmp.sign == '-') num_1_tmp.sign = '-';
	else num_1_tmp.sign = '+';
	*this = num_1_tmp;
	return *this;
}

BigInt& BigInt::operator|=(const BigInt& num_2) {
	int num_2_digits = used_digits(num_2);
	int num_1_digits = used_digits(*this);
	BigInt num_1_tmp(*this);//need to create copies and make two's complement
	BigInt num_2_tmp(num_2);
	if (num_1_digits < num_2_digits) {
		num_1_tmp.number.resize(num_2_digits);
		num_1_digits = num_2_digits;
	}
	else {
		num_2_tmp.number.resize(num_1_digits);
		num_2_digits = num_1_digits;
	}
	if (num_1_tmp.sign == '-') {
		twos_complement(num_1_tmp);
	}
	if (num_2_tmp.sign == '-') {
		twos_complement(num_2_tmp);
	}

	for (int i = 0; i < num_2_digits; i++) {	//-1 due to coorrect work in highest digit
		num_1_tmp.number[i] |= num_2_tmp.number[i];
	}
	if ((num_1_tmp.sign == '-'  ||  num_2_tmp.sign == '-')) {	//inverse
		for (int i = 0; i < num_1_digits; i++) {
			num_1_tmp.number[i] = ~num_1_tmp.number[i];
		}
		carry(num_1_tmp, ((long long)num_1_tmp.number[0]) + 1, 0);
	}

	if (num_1_tmp.sign == '-' || num_2_tmp.sign == '-') {//need transition back
		num_1_tmp.sign = '-';
	}
	else num_1_tmp.sign = '+';
	*this = num_1_tmp;
	return *this;
}

BigInt BigInt::operator+() const {	// unary +
	return *this;
}

BigInt BigInt::operator-() const {	// unary -
	BigInt temp = *this;
	if (sign == '+') temp.sign = '-';
	else temp.sign = '+';
	return temp;
}

BigInt::operator int() const {
	BigInt tmp = *this;
	int answer = number[0] << 1 >> 1;
	if (sign == '-') {
		answer = ~answer + 1;
		if (number[0] == 2'147'483'648) answer = -2'147'483'648;
	}
	return answer;
}

BigInt::operator std::string() const {
	string str = "";
	if (*this == BigInt(0) || *this == BigInt(-0)) {
		str.push_back('0');
		return str;
	}
	if (sign == '-') str.push_back('-');
	vector<int>dec_num(1);
	dec_num[0] = 1;	
	long long tmp = 0;
	long long decs[9] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};
	int dec_base = 1000000000, carry = 0;
	long long index = (used_digits(*this) - 1)* 32 + digits_in_highest_digit(*this, used_digits(*this)) - 2; //-1 for index - 1 for first digit
	for (int i = index; i >= 0; i--) {
		carry = number[i / 32] >> (i % 32) & 1;
		for (int j = 0; j < dec_num.size(); j++) {
			dec_num[j] = dec_num[j] * 2 + carry;
			carry = 0;
			if (dec_num[j] >= dec_base) {
				carry = 1;
				dec_num[j] = dec_num[j] % dec_base;
			}
		}
		if (carry) {
			dec_num.resize(dec_num.size() + 1);
			dec_num[dec_num.size() - 1] = 1;
		}
	}
	int flag = 1;
	char char_convert = 48;
	for (int j = 8; j >= 0; j--) {
		if (flag && (dec_num[dec_num.size() - 1] / decs[j]) % 10 == 0) {
			continue;
		}
		else flag = 0;
		str.push_back((dec_num[dec_num.size() - 1] / decs[j]) % 10 + char_convert);
	}
	for (int i = dec_num.size() - 2; i >= 0; i--) {
		for (int j = 8; j >= 0; j--) {
			str.push_back((dec_num[i] / decs[j]) % 10 + char_convert);
		}
	}
	return str;
}

size_t BigInt::size() const {
	size_t answer = number.size() * sizeof(unsigned int) + sizeof(sign) + sizeof(default_size) + sizeof(base);
	return answer;
}

BigInt operator+(const BigInt& num_1, const BigInt& num_2) {
	BigInt tmp(num_1);
	return tmp += num_2;
}

BigInt operator-(const BigInt& num_1, const BigInt& num_2) {
	BigInt tmp(num_1);
	return tmp -= num_2;
}

BigInt operator*(const BigInt& num_1, const BigInt& num_2) {
	BigInt tmp(num_1);
	return tmp *= num_2;
}

BigInt operator/(const BigInt& num_1, const BigInt& num_2) {
	BigInt tmp(num_1);
	return tmp /= num_2;
}

BigInt operator^(const BigInt& num_1, const BigInt& num_2) {
	BigInt tmp(num_1);
	return tmp ^= num_2;
}

BigInt operator%(const BigInt& num_1, const BigInt& num_2) {
	BigInt tmp(num_1);
	tmp %= num_2;
	return tmp;
}

BigInt operator&(const BigInt& num_1, const BigInt& num_2) {
	BigInt tmp(num_1);
	return tmp &= num_2;
}

BigInt operator|(const BigInt& num_1, const BigInt& num_2) {
	BigInt tmp(num_1);
	return tmp |= num_2;
}

std::ostream& operator<<(std::ostream& o, const BigInt& i) {
	o << (std::string)i;
	return o;
}

