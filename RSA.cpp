/**
 * @file RSA.cpp
 * @author gun (lxuegod@163.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-23
 * 
 * @copyright Copyright (c) 2022
 * 
问题描述：
计算n=p*q, ψ(n)=(p-1)(q-1)
随机选取整数e(1<e<ψ(n))，满足gcd(e,ψ(n))=1
计算d，满足d*e=1(mod ψ(n)) 
注：n公开， p和q保密。e为公钥，d为私钥

签名算法：
利用一个安全的Hash函数h来产生消息摘要h(m)
用签名算法计算签名s=Signk(m)=h(m)^d mod n

验证算法：
首先利用一个安全的Hash函数h计算消息摘要h(m)
用检验等式h(m) mod n=se mod n 是否成立，若相等签名有效；否则，签名无效。
由于s=h(m)^d mod n
d*e=1(modψ(n))
所以se mod n=h(m)^(ed) mod n
= h(m)^(kψ(n)+1) mod n
= h(m)*h(m)^(kψ(n)) mod n
= h(m)*((h(m)^ψ(n))^k) mod n
= h(m)

实例：
初始化：
假设A选取p = 13，q = 11，e = 13，则有n = p*q = 143，φ(n) = (p-1)*(q-1) = 12×10 = 120。
求解e*d = 13*d ≡ 1(mod 120) 得 d = 37
因此：A的公钥为(n = 143,e = 13)；私钥为d = 37

签名过程：
假定消息m的Hash值h(m) = 16，则计算m签名s = h(m)^d mod n =  1637 mod 143 = 3

验证过程：
接受者B收到签名后，计算s*e mod n = 313 mod 143 = 16，h(m) mod n = 16 mod 143 = 16
等式 h(m) mod n = s*e mod n成立。因此，B验证此签名有效。
注：^为幂运算，*为乘运算
 */



#include <iostream>
#include <conio.h>
using namespace std;

class RSA {
private:
    //  两个质数
    int primeNumber1, primeNumber2;
    //  两个素数相乘的结果
    int multiplication;
    //  欧拉函数公式计算的存储结果
    int euler;
    //  一个小于euler并与euler互素的整数
    int primeEuler;
    //  明文
    int explicitText;
    //  密文
    int cipherText;
    //  primeEuler关于euler的模反元素
    int modeInverse;
    //  输入数字
    int input;

public:
    void Output();
    int Work(int text, int number, int multiplication);
};

int RSA::Work(int text, int number, int multiplication) { 
    int result = 1;
    number++;
    while(number != 1) {
        result = result * text;
        result = result % multiplication;
        number--;
    }
    //cout<<result<<endl;
    return result;
}

void RSA::Output() {
    //  输入两个不相等的质数
    cout << "please input two unequal prime numbers: ";
    cin >> primeNumber1 >> primeNumber2;
    while (primeNumber1 == primeNumber2) {
        cout << "the input is error,please input agin: " << endl;
        cin >> primeNumber1 >> primeNumber2;
    }

    //  计算两个素数的乘积
    multiplication = primeNumber1 * primeNumber2;
    cout << "the multiplication is " << multiplication << endl;

    euler = (primeNumber1 - 1) * (primeNumber2 - 1);
    cout << "the euler number is " << euler << endl;

    //   选择一个小于euler并与euler互素的整数
    cout << "please input an integer less than euler: ";
    cin >> primeEuler;

    //  提醒重新输入
    while (primeEuler < 1 || primeEuler > euler) {
        cout << "the number is error, please input again: ";
        cin >> primeEuler;
    }

    modeInverse = 1;
    //  求模反
    while(((primeEuler * modeInverse) % euler) != 1) {
        modeInverse++;
    }
    //  输出模反结果
    cout << "then calculate out that the mode inverse result is: " << modeInverse << endl;

    //  选择加密还是解密，1位加密；2为解密
    cout << "If you want to encipher,please input 1" << endl << "If you want to decipher,please input 2" << endl;

    cin >> input;
    switch(input) {
        case 1: 
            cout << "input the explicit text: ";
            //  输入要加密的明文数字
            cin >> explicitText;          
            cipherText = Work(explicitText, primeEuler, multiplication);
            cout << "the cipherText is " << cipherText << endl;
            break;
        case 2: 
            cout << "input the cipher text: ";
            //  输入要解密的密文数字
            cin >> cipherText;             
            explicitText = Work(cipherText, modeInverse, multiplication);
            cout << "the explicit text is " << explicitText << endl;
            break;
    }
    getch();
}

int main() {
    RSA obj;
    obj.Output();
    return 0; 
}