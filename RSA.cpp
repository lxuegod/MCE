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