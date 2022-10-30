#ifndef BIGINT_H
#define BIGINT_H
#include <vector>
#include <string>

using namespace std;

class BigInt {
	static const int default_size = 1;
	long long base = 4'294'967'296;
	vector <unsigned int> number;
	char sign;
public:
	BigInt();
	BigInt(int a);
	BigInt(std::string str_num); // if not a number -  std::invalid_argument 
	BigInt(const BigInt&);
	~BigInt();

	BigInt& operator=(const BigInt& a);  //assignment

	BigInt operator~() const;

	BigInt& operator++();
	const BigInt operator++(int);
	BigInt& operator--();
	const BigInt operator--(int);

	BigInt& operator+=(const BigInt& num_2);
	BigInt& operator*=(const BigInt& num_2);
	BigInt& operator-=(const BigInt& num_2);
	BigInt& operator/=(const BigInt& num_2);
	BigInt& operator^=(const BigInt& num_2);
	BigInt& operator%=(const BigInt& num_2);
	BigInt& operator&=(const BigInt& num_2);
	BigInt& operator|=(const BigInt& num_2);

	BigInt operator+() const;  // unary +
	BigInt operator-() const;  // unary -

	bool operator==(const BigInt& num_2) const;
	bool operator!=(const BigInt& num_2) const;
	bool operator<(const BigInt& num_2) const;
	bool operator>(const BigInt& num_2) const;
	bool operator<=(const BigInt& num_2) const;
	bool operator>=(const BigInt& num_2) const;

	operator int() const;
	operator std::string() const;

	size_t size() const;  // size in bytes

	void carry(BigInt& number, long long new_number, int num_index);
	void carry_mul(BigInt& number, unsigned long long new_number, int num_index);
	void carry(BigInt& number, long long new_number, int num_index) const;
	int digits_in_highest_digit(BigInt& num, int num_of_used_digits);
	int digits_in_highest_digit(const BigInt& num, int num_of_used_digits);
	int digits_in_highest_digit(const BigInt& num, int used_digits) const;
	int used_digits(BigInt& num);
	int used_digits(const BigInt& num);
	int used_digits(const BigInt& num) const;
	int is_number(std::string& str_num);
	friend int is_zero(BigInt& number);
	friend int is_zero(const BigInt& number);
	void twos_complement(BigInt& number);
	char set_sign(char symbol);

};

BigInt operator+(const BigInt& num_1, const BigInt& num_2);
BigInt operator-(const BigInt& num_1, const BigInt& num_2);
BigInt operator*(const BigInt& num_1, const BigInt& num_2);
BigInt operator/(const BigInt& num_1, const BigInt& num_2);
BigInt operator^(const BigInt& num_1, const BigInt& num_2);
BigInt operator%(const BigInt& num_1, const BigInt& num_2);
BigInt operator&(const BigInt& num_1, const BigInt& num_2);
BigInt operator|(const BigInt& num_1, const BigInt& num_2);


std::ostream& operator<<(std::ostream& o, const BigInt& i);

void IncreaseDecNum(vector<int> &number, int bit);
#endif