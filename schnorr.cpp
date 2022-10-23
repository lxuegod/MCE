/**
 * @file schnorr.cpp
 * @author gun (lxuegod@163.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-23
 * 
 * @copyright Copyright (c) 2022
 * 问题描述：
 * 
初始化：
首先选择一个大素数p，使在Zp中求解离散对数困难。然后选择一个生成元g∈Zp*，g^q ≡1(mod p)
最后选取随机数 1<x<q，计算y≡g^x	mod p，则公钥为(p,q,g,y)，私钥为x。

签名：
用户选择随机数k，对消息进行如下计算得签名值(e,s).  r≡g^k mod p
e=H(r,m)  s≡x*e+k mod q

验证：
接收方在收到消息m和签名值(e,s)后，进行以下步骤：
计算r1≡((g^s)*(y^(-e))) mod p
计算H(r1,m)
验证等式：H(r1,m)=e
如果等式成立，接受签名，否则签名无效。

签名体制的正确性证明：  
r1 ≡g^s*y^(-e) mod p 
≡g^s*g^(-x*e) mod p
≡g^(s-x*e) mod p
≡g^(x*e+k-x*e) mod p
≡g^k mod p
≡r  
H(r1,m)=e
r≡g^k mod p；e=H(r,m); s≡(x*e+k) mod q

实例：
初始化：
假设A选取素数p = 23和q = 11，其中(p-1)/ q = 2，选取生成元h = 11∈Zp* 。
并计算g = h^((p-1)/q) mod p = 11^2 mod 23 ≡6，
则有 g^q mod p = [h^((p-1)/q) mod p]^q mod p = h^(p-1) mod p ≡1
既然g≠1，那么g生成Zp* 中一个11阶循环子群。然后选取私钥x = 10 
计算y = g^x mod p = 610 mod 23 ≡ 4
则A的公钥是（p = 23,q = 11,g = 6,y = 4）, 私钥为（x = 10）

签名过程：
选取随机数k = 9，1≤k≤10，计算r = g^k mod p = 69 mod 23 = 16。
设有e = h(m||r) = 13 ，计算s = (x*e+k) mod q = (10×13 + 9) mod 11 ≡ 7。
因此消息m的签名为（e = 13,s = 7）

验证过程：
签名接收者B计算r1 = g^s*y^(-e) mod p = 67×4-13 mod 23= 16 ≡r
则有 h(m || r1) = h(m || r) = e = 13 
因此B接受签名

注：^为幂运算，*为乘运算
 */


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
