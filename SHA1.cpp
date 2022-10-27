/**
 * @file SHA1.cpp
 * @author gun (lxuegod@163.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-27
 * 
 * @copyright Copyright (c) 2022
 * 
 * 一、实验目的
1.	了解散列算法SHA-1的实现过程
2.	实现并掌握SHA-1散列算法。
二、实验环境
运行Windows操作系统的PC机，具有C++11及其以上的编译环境，IDE为Visual Studio Code2022
三、	实验步骤
1. 对SHA1算法的理解
	SHA1算法（公开密钥算法）的原理：
1)	首先将明文转化为ASCII码的十六进制表示方式
2)	对于任意长度的明文，SHA1的明文分组过程，首先需要对明文添加位数，使明文总长度为448（mod512）位。在明文后添加位的方法是第一个添加位是l，其余都是0。然后将真正明文的长度（没有添加位以前的明文长度）以64位表示，附加于前面已添加过位的明文后，此时的明文长度正好是512位的倍数。SHA1的原始报文长度不能超过2的64次方，另外SHA1的明文长度从低位开始填充。
3)	将得到数据进行拆分，按照512位分块。SHA1有4轮运算，每一轮包括20个步骤（一共80步），最后产生160位摘要。

 */

#include <iostream>
#include <bits/stdc++.h>

using namespace std;

#define NUM 8    //一个字等价于8个16进制数 
#define HEX 16   //16进制 
#define BIT 512 //消息认证码512 bite一组

class SHA_1{
    public:
        //辅助函数 
        char hexConvert(int a, int b, int c, int d); //把一个四位二进制串转为一个十六进制字符 

        string binaryConvert(string a); //把一个十六进制字符转为四位二进制串

        vector<int> sysConvert(const unsigned long long &a, int sys, int length); //把整数a转换为长度为length的sys进制向量形式 

        vector<int> char_to_hex(const string &word); //十六进制符号串（字）转十六进制整数串

        string hex_to_char(const vector<int> &a); //十六进制整数串转十六进制符号串 

        string char_to_binary(const char a); //字符转八位二进制串 

        //运算函数 
        string w_AND(const string &a, const string &b); //字,与操作

        string w_OR(const string &a, const string &b); //字,或操作

        string w_XOR(const string &a, const string &b); //字，异或

        string w_CPL(const string &a); //字，补

        string w_ADD(const string &a, const string &b); //字，模2^32整数加

        string ROTL(const string &a, int s); //a循环左移s个位置（0<= s <=31）,a不改变 

        string Ft(int t, const string &B, const string &C, const string &D);

        string K(int t); //字常数K

        //主要操作函数 
        vector<vector<int> > SHA_1_PAD(const string &b_msg); //填充函数,输入为消息的二进制串 

        vector<vector<string> > divide(const vector<vector<int> > &result); //将填充后的消息分成以字（八个十六进制数）为单位的串联

        string result_SHA_1(const string &msg); //获得消息摘要 
    private:
        string H0 = "67452301"; //调用result_SHA_1函数后这些值会改变 
        string H1 = "EFCDAB89";
        string H2 = "98BADCFE";
        string H3 = "10325476";
        string H4 = "C3D2E1F0";
}; 

//辅助函数 
char SHA_1::hexConvert(int a, int b, int c, int d){ //把一个四位二进制串转为一个十六进制字符
    int x = 8 * a + 4 * b + 2 * c + d;
    string out;
    stringstream ss;
    ss << std::hex <<x;
    ss >> out ;
    transform(out.begin(), out.end(), out.begin(), ::toupper);
    return out[0];
}

string SHA_1::binaryConvert(string a){ //把一个十六进制字符转为四位二进制string 
    bitset<4> bit(stoi(a, nullptr, 8));
    return bit.to_string();
}

vector<int> SHA_1::sysConvert(const unsigned long long &a, int sys, int length){ //把整数a转换为长度为length的sys进制向量形式 
    vector<int> result;
    stack<int> tmp;
    unsigned long long a1 = a;
    do{
        int temp = a1 % sys;
        tmp.push(temp);
        a1 /= sys; 
    }while(a1 != 0);

    for(int i = 0; i < length - tmp.size(); i++)    //填充0 
        result.push_back(0);                         

    while(!tmp.empty()){
        result.push_back(tmp.top());
        tmp.pop();
    }

    return result;
}

vector<int> SHA_1::char_to_hex(const string &word){ //十六进制符号串（字）转十六进制整数串
    vector<int> result(NUM, 0);
    for(int i = 0; i < NUM; i++){
        if(word[i] >= '0' && word[i] <= '9')
            result[i] = word[i] - '0';
        else if(word[i] >= 'A' && word[i] <= 'Z')
            result[i] = 10 + word[i] - 'A';
    }
    return result;
}

string SHA_1::hex_to_char(const vector<int> &a){ //十六进制整数串转十六进制符号串
    string result;
    for(int i = 0; i < NUM; i++){
        if(a[i] >= 0 && a[i] <= 9)
            result += a[i] + '0';
        else if(a[i] >= 10 && a[i] <= 15)
            result += a[i] - 10 + 'A';
    }
    return result;
}

string SHA_1::char_to_binary(const char a){ //字符转八位二进制串 
    int a_int;
    if(a >= '0' && a <= '9')
        a_int = a - '0' + 48;
    else if(a >= 'A' && a <= 'Z')
        a_int = a - 'A' + 65;
    else if(a >= 'a' && a <= 'z')
        a_int = a - 'a' + 97;

    vector<int> temp = sysConvert(a_int, 2, 8);
    string result;
    for(int i = 0; i < temp.size(); i++)
        result += temp[i] + '0';

    return result;
} 

//运算函数 
string SHA_1::w_AND(const string &a, const string &b) { //字,与操作
    vector<int> a_int = char_to_hex(a);
    vector<int> b_int = char_to_hex(b);
    vector<int> r_int(NUM, 0);

    for(int i = 0; i < NUM; i++)
        r_int[i] = a_int[i] & b_int[i];

    return hex_to_char(r_int);
}

string SHA_1::w_OR(const string &a, const string &b){ //字,或操作
    vector<int> a_int = char_to_hex(a);
    vector<int> b_int = char_to_hex(b);
    vector<int> r_int(NUM, 0);

    for(int i = 0; i < NUM; i++)
        r_int[i] = a_int[i] | b_int[i];

    return hex_to_char(r_int);
}

string SHA_1::w_XOR(const string &a, const string &b){ //字，异或
    vector<int> a_int = char_to_hex(a);
    vector<int> b_int = char_to_hex(b);
    vector<int> r_int(NUM, 0);

    for(int i = 0; i < NUM; i++)
        r_int[i] = a_int[i] ^ b_int[i];

    return hex_to_char(r_int);
}

string SHA_1::w_CPL(const string &a){ //字，补
    vector<int> a_int = char_to_hex(a);
    vector<int> r_int(NUM, 0);

    for(int i = 0; i < NUM; i++)
        r_int[i] = HEX - 1 - a_int[i];

    return hex_to_char(r_int); 
} 

string SHA_1::w_ADD(const string &a, const string &b){ //字，模2^32整数加
    vector<int> A = char_to_hex(a);
    vector<int> B = char_to_hex(b);
    vector<int> Result(NUM, 0);

    int sign = 0;
    for(int i = NUM - 1; i >= 0; i--){
        Result[i] = A[i] + B[i] + sign;
        sign = Result[i] / HEX;
        Result[i] %= HEX;           
    }
    return hex_to_char(Result);
}

string SHA_1::ROTL(const string &a, int s){ //a循环左移s个位置（0<= s <=31）
    vector<int> A = char_to_hex(a);
    unsigned long a_int = 0;    //字的整数值 

    for(int i = 0; i < NUM; i++)
        a_int += A[NUM - 1 - i] * pow(HEX, i);

    a_int = (a_int >> (NUM * 4 - s)) | (a_int << s); //循环左移s位

    vector<int> A1 = sysConvert(a_int, HEX, NUM);
    return hex_to_char(A1); 
}

string SHA_1::Ft(int t, const string &B, const string &C, const string &D){
    if(t >= 0 && t <= 19)
        return w_OR(w_AND(B, C), w_AND(w_CPL(B), D));
    else if(t >= 20 && t <= 39)
        return w_XOR(w_XOR(B, C), D);
    else if(t >= 40 && t <= 59)
        return w_OR(w_OR(w_AND(B, C), w_AND(B, D)), w_AND(C, D));
    else if(t >= 60 && t <= 79)
        return w_XOR(w_XOR(B, C), D); 
}

string SHA_1::K(int t){ //字常数K
    if(t >= 0 && t <= 19) 
        return "5A827999";
    else if(t >= 20 && t <= 39)
        return "6ED9EBA1";
    else if(t >= 40 && t <= 59)
        return "8F1BBCDC";
    else if(t >= 60 && t <= 79)
        return "CA62C1D6";
} 

//主要操作函数 
vector<vector<int> > SHA_1::SHA_1_PAD(const string &b_msg){ //消息的填充 
    unsigned long long length = b_msg.size();
    int d = (447 - length) % BIT;
    vector<int> l = sysConvert(length, 2, 64); //信息长度的二进制表示,且这二进制表示的长度是64 

    vector<vector<int> > result;
    string str_result;
    str_result += b_msg;
    str_result += '1';
    for(int i = 0; i < d; i++)
        str_result += '0';
    for(int i = 0; i < l.size(); i++)
        str_result += '0' + l[i];

    int n = str_result.size() / BIT;    //填充后的消息有n组BIT比特数据
    result.resize(n, vector<int>(BIT, 0));

    for(int i = 0; i < n; i++)
        for(int j = 0; j < BIT; j++)
            result[i][j] = str_result[i * BIT + j] - '0'; 

    return result;
}

vector<vector<string> > SHA_1::divide(const vector<vector<int> > &result){ //将填充后的消息分成以字（八个十六进制数）为单位的串联
    vector<vector<string> > w_result(result.size(), vector<string>(HEX, ""));
    for(int i = 0; i < result.size(); i++)
        for(int j = 0; j < HEX; j++){   //对每个string，由result中的32位二进制数得出一个八位十六进制符号的string. 
            w_result[i][j] += hexConvert(result[i][j * 32], result[i][j * 32 + 1], result[i][j * 32 + 2], result[i][j * 32 + 3]);
            w_result[i][j] += hexConvert(result[i][j * 32 + 4], result[i][j * 32 + 5], result[i][j * 32 + 6], result[i][j * 32 + 7]);
            w_result[i][j] += hexConvert(result[i][j * 32 + 8], result[i][j * 32 + 9], result[i][j * 32 + 10], result[i][j * 32 + 11]);
            w_result[i][j] += hexConvert(result[i][j * 32 + 12], result[i][j * 32 + 13], result[i][j * 32 + 14], result[i][j * 32 + 15]);
            w_result[i][j] += hexConvert(result[i][j * 32 + 16], result[i][j * 32 + 17], result[i][j * 32 + 18], result[i][j * 32 + 19]);
            w_result[i][j] += hexConvert(result[i][j * 32 + 20], result[i][j * 32 + 21], result[i][j * 32 + 22], result[i][j * 32 + 23]);
            w_result[i][j] += hexConvert(result[i][j * 32 + 24], result[i][j * 32 + 25], result[i][j * 32 + 26], result[i][j * 32 + 27]);
            w_result[i][j] += hexConvert(result[i][j * 32 + 28], result[i][j * 32 + 29], result[i][j * 32 + 30], result[i][j * 32 + 31]);
        }
    return w_result; 
}

string SHA_1::result_SHA_1(const string &msg){ //获得消息摘要 
     string b_msg;
     for(int i = 0; i < msg.size(); i++)
        b_msg += char_to_binary(msg[i]);

     vector<vector<int> > y = SHA_1_PAD(b_msg);
     vector<vector<string> > w_y = divide(y);

     vector<string> W(80, "");  //存放 W参数 
     for(int i = 0; i < y.size(); i++){ //主循环 

        for(int t = 0; t <= 15; t++)
            W[t] = w_y[i][t];   

        for(int t = 16; t <= 79; t++){
            string tmp = w_XOR(w_XOR(w_XOR(W[t - 3], W[t - 8]), W[t - 14]), W[t - 16]);
            W[t] = ROTL(tmp, 1);
         }

        string A = H0;
        string B = H1;
        string C = H2;
        string D = H3;
        string E = H4;

        for(int t = 0; t <= 79; t++){
            string temp;
            temp = w_ADD(w_ADD(w_ADD(w_ADD(ROTL(A, 5), Ft(t, B, C, D)), E), W[t]), K(t));
            E = D;
            D = C;
            C = ROTL(B, 30);
            B = A;
            A = temp;
        }
        H0 = w_ADD(H0, A);
        H1 = w_ADD(H1, B);
        H2 = w_ADD(H2, C);
        H3 = w_ADD(H3, D);
        H4 = w_ADD(H4, E);
     } 

     string result = H0 + H1 + H2 + H3 + H4;

     for(int i = 0; i < result.size(); i++){  //大写变小写 
        if(result[i] >= 'A' && result[i] <= 'Z')
            result[i] = result[i] - 'A' + 'a';
     }
     return result;
}

int main(){
    SHA_1 sha_1;
    string msg;
    cout << "please input message:" << endl;
    cin >> msg;
    cout << "SHA_1(x)为：" << endl << sha_1.result_SHA_1(msg) << endl;
    return 0;
}
