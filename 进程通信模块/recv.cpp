#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include"trans.h"
using namespace std;
struct Hamming {
    int  Len_Red, Len_Ham, w_num; //原码，冗余码，海明码的长度,实际传输出错个数,出错位
    bool *T, *H, *HH; //原码和海明码指针,传输后的海明码
    char *recv;
    Hamming(){
        recv = (char*)malloc(100*sizeof(char));
    }

    void decode() //解码以及纠错
    {
        HH = (bool*)malloc((Len_Ham+2)*sizeof(bool));
        for(int t=0;t<Len_Ham;++t){
            HH[t+1]= (recv[t]=='1'?1:0);
        }
        int era = 0;
        for (int i = 1; i <= Len_Ham; i++) {
            if (HH[i] == 0) continue;
            int b = i;
            int wei = 1;
            while (b) {
                if (b & 1) {
                    era ^= wei;
                }
                b = b >> 1;
                wei = wei << 1;
            }
        }
        HH[era] = !HH[era];
        cout << "解码后" << endl;
        cout << "出错位置为：:  " << era << endl;
        cout << "修复错误后: " << endl;
        for (int i = 1; i <= Len_Ham; i++)
            if (HH[i]) cout << "1";
            else cout << "0";
        int temp = 1;
        cout << endl << "还原后的原码为:" << endl;
        for (int i = 1; i <= Len_Ham; i++) {
            if (i == temp) {
                temp = temp << 1;
            } else if (HH[i]) cout << "1";
            else cout << "0";
        }
        cout << endl << endl;
    }


};
int main() {
    Hamming op;
    Trans t;
    op.Len_Ham = t.recvMsg(op.recv);
    cout<<"\n海明码长度:"<<op.Len_Ham<<endl;
    for(int i=0;i<op.Len_Ham;++i)
        cout<<op.recv[i];
    cout<<endl;
    op.decode();
    return 0;
}
