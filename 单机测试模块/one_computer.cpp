#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<ctime>
using namespace std;

double random1() {
    return (double) rand() / RAND_MAX;
}

int random(int m) //����[0,m-1]�������
{
    return (int) (random1()*(m - 1) + 0.5);
}

struct Hamming {
//private:
    int Len_Tru, Len_Red, Len_Ham, w_num; //ԭ�룬�����룬������ĳ���,ʵ�ʴ���������,����λ

public:
    bool *T, *H, *HH; //ԭ��ͺ�����ָ��,�����ĺ�����
    void create(int m) //�������һ������Ϊm��ԭ�룬�����캣����
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
            if (i == temp) { //Ч����λ

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

    void trans() //�������һλ
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
        cout << "\ntransing" << endl;
        //cout << "on recv:" << endl;
        for (int i = 1; i <= Len_Ham; i++) if (HH[i]) cout << "1";
            else cout << "0";
        cout << endl << "error pos:" << w_num<< endl;
    }

    void decode() //�����Լ�����
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
        cout << "After decode" << endl;
        cout << "the wrong position is:  " << era << endl;
        cout << "After fixing th error: " << endl;
        for (int i = 1; i <= Len_Ham; i++)
            if (HH[i]) cout << "1";
            else cout << "0";
        int temp = 1;
        cout << endl << "huanyuan hou de yuanma wei:" << endl;
        for (int i = 1; i <= Len_Ham; i++) {
            if (i == temp) {
                temp = temp << 1;
            } else if (HH[i]) cout << "1";
            else cout << "0";
        }
        cout << endl << endl;
    }

    void put() //��ӡԭ��ͺ�����
    {
        cout << "before" << endl;
        cout << "origin��" << endl;
        for (int i = 1; i <= Len_Tru; i++) if (T[i]) cout << "1";
            else cout << "0";
        cout << endl << "origin length�� " << Len_Tru << endl;
        cout << "ham��" << endl;
        for (int i = 1; i <= Len_Ham; i++) if (H[i]) cout << "1";
            else cout << "0";
        cout << endl << "ham length�� " << Len_Ham << endl << "rongyu length�� " << Len_Red << endl << endl << endl;
    }
};

int main() {
    Hamming op;
    op.create(10);
    op.put();
    op.decode();
    op.trans();
}
