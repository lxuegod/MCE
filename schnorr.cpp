#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
using namespace std;

class Schnorr {
private:
	int s,e;
	//	存放逆元的数组
	int array[2];

	//	质数（p）
	int primeNumber1;
	//	质数（q）
	int primeNumber2;

	//	生成元（h）
	int generator;
	//	h的计算结果g
	int hToG;

	//	g的计算结果y
	int gToY;

	//	随机数(x)为私钥
	int randomNumber;

	//	用户选择随机数（k）
	int userRandom;

	//	用户选择随机数k的计算结果r
	int kToR;

	//	验证生成元
	int validationGenerator;

	//	验证随机数
	int validationRandomNumber;
public:
	//	幂运算 res = x^y
	int Pow(int x, int y);
	//	公式res = a^b mod m
	int MyPow(int a, int b, int modNumber);
	// 求 a mod b 的逆元
	void modInverseElement(int a, int b);
	//	输入输出
	void Output();
};


//	公式res = a^b mod m
int Schnorr::MyPow(int a, int b, int modNumber) {
    int res = 1;
    a %= modNumber;
    while (b != 0) {
    if ((b & 1) == 1)
    	res = (res * a) % modNumber;
    a = (a * a) % modNumber;
    b >>= 1;
    }
	//cout << "res = " << endl;
    return res;
}

//	幂运算 res = x^y
int Schnorr::Pow(int x, int y) {
	int temp = y;
	while ( x > 1) {
		y *= temp;
		x--;
	}
	return y;
}

// 求 a mod b 的逆元
void Schnorr::modInverseElement(int a, int b) {
    if (b == 0) {
        array[0] = 1;
        array[1] = 0;
    } else {
        modInverseElement(b, a % b);
        int x = array[0];
        array[0] = array[1];
        array[1] = x - (a / b) * array[1];
    }
}

void Schnorr::Output() {
	//	输入质数p,q
	cout << "请输入两个质数，满足第二个数字是第一个数字中减一的因子" << endl;
	cin >> primeNumber1 >> primeNumber2;

	while(((primeNumber1 - 1) % primeNumber2) != 0) {
		cout << "输入不符合要求，请再次输入，两个质数满足第二个数字是第一个数字中减一的因子：" << endl;
		cin >> primeNumber1 >> primeNumber2;
	}

	//	输入生成元h
	cout << "请输入一个生成元：" << endl;
	cin >> generator;

	

	//	计算g
	hToG = Pow((primeNumber1 - 1) / primeNumber2, generator) % primeNumber1;
	//cout << "g = " << hToG << endl;
	
	//	设置随机数种子
	srand(time(NULL)); 
	//	生成随机数
	randomNumber = rand() % primeNumber2;

	//	计算y = g^x mod p，y是公开密钥
	gToY = MyPow(hToG, randomNumber, primeNumber1);
	cout << "生成元的计算结果是：" << gToY <<endl;
    
	//	用户输入随机数
	cout << "请输入一个随机数：" << endl;
	cin >> userRandom;

	// r = g^k mod p
    kToR = MyPow(hToG, userRandom, primeNumber1);
    cout << "随机数的计算结果是：" << kToR << endl;

	// e = h(m||r)
	e = 13;
	//	s = (x*e+k) mod q
	s = (randomNumber * e + userRandom) % primeNumber2;
	cout << "签名信息(e,s)=(" << e << "," << s << ")" << endl;

	//	求逆元，放在数组array[0]
	modInverseElement(gToY, primeNumber1);
	validationGenerator = array[0];
	cout << "验证生成元 = " << validationGenerator << endl;

	validationRandomNumber = MyPow(hToG, s, primeNumber1) * MyPow(validationGenerator, e, primeNumber1) % primeNumber1;

	//	验证签名是否有效
	if(kToR == validationRandomNumber){
		cout << "签名有效" << endl;
	}else{
		cout << "签名无效" << endl;
	}	
}

int main() {
	Schnorr obj;
	obj.Output();
	return 0;
}
