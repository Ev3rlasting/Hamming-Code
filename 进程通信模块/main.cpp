#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include"trans.h"
using namespace std;

double random1() {
    return (double) rand() / RAND_MAX;
}

int random(int m) //生成[0,m-1]的随机数
{
    return (int) (random1()*(m - 1) + 0.5);
}

struct Hamming {
//private:
    int Len_Tru, Len_Red, Len_Ham, w_num; //原码，冗余码，海明码的长度,实际传输出错个数,出错位

public:
    bool *T, *H, *HH; //原码和海明码指针,传输后的海明码
    void create(int m) //随机生成一个长度为m的原码，并构造海明码
    {
        T = (bool *)malloc((m + 2) * sizeof (bool));
        H = (bool *)malloc((m + 110) * sizeof (bool));
        HH = (bool *)malloc((m + 110) * sizeof (bool));
        Len_Tru = m;
        Len_Red = 0;
        Len_Ham = 0;

        srand((unsigned) time(NULL));
        for (int i = 1; i <= m; i++) {
            if (random(2) == 1) T[i] = 1;
            else T[i] = 0;
        }

        int temp = 1, cnt = 0;
        for (int i = 1;; i++) {
            if (i == temp) { //效验码位

                H[i] = 0;
                Len_Red++;
                Len_Ham++;
                temp = temp << 1;
            } else {
                cnt++;
                int b = i;
                int wei = 1;
                while (b) {
                    if (b & 1) {
                        H[wei] = H[wei]^T[cnt];
                    }
                    b = b >> 1;
                    wei = wei << 1;
                }
                Len_Ham++;
                H[Len_Ham] = T[cnt];
            }
            if (cnt == m) break;
        }
    }

    void trans() //随机出错一位
    {
        srand((unsigned) time(NULL));
        int t = random(Len_Ham) + 1;
        w_num = t;
        for (int i = 1; i <= Len_Ham; i++) {
            if (i == t) HH[i] = !H[i];
            else HH[i] = H[i];
        }
        for(int h=1; h<=Len_Tru;++h){
            cout<<H[h];
        }
        cout << "\n传送中。。。" << endl;
        //cout << "on recv:" << endl;
        for (int i = 1; i <= Len_Ham; i++) if (HH[i]) cout << "1";
            else cout << "0";
        cout << endl << "发生错误的位置:" << w_num<< endl;
    }

    void decode() //解码以及纠错
    {
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
        cout << "错误位置为:  " << era << endl;
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

    void put() //打印原码和海明码
    {
        cout << "之前" << endl;
        cout << "原码：" << endl;
        for (int i = 1; i <= Len_Tru; i++) if (T[i]) cout << "1";
            else cout << "0";
        cout << endl << "原码长度： " << Len_Tru << endl;
        cout << "海明码：" << endl;
        for (int i = 1; i <= Len_Ham; i++) if (H[i]) cout << "1";
            else cout << "0";
        cout << endl << "海明码长度： " << Len_Ham << endl << "冗余码长度： " << Len_Red << endl << endl << endl;
    }
};

int main() {
    Hamming op;
    op.create(10);
    op.trans();
    Trans t;
    t.sendMsg(op.HH,op.Len_Ham);
}
