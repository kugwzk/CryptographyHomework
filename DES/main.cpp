#include "DES.h"
void ECBDESENrun(char Out[8], char In[8])
{
    static bool M[64], Tmp[32], *Li = &M[0], *Ri = &M[32];
    ByteToBit(M, In, 64);
    Transform(M, M, IP, 64);
    for (int i=0; i<16; i++)
    {
        memcpy(Tmp, Ri, 32);
        FFunc(Ri, SubKey[i]);
        Xor(Ri, Li, 32);
        memcpy(Li, Tmp, 32);
    }
    Transform(M, M, IP_inv, 64);
    BitToByte(Out, M, 64);
}
void ECBDESDErun(char Out[8], char In[8])
{
    static bool M[64], Tmp[32], *Li = &M[0], *Ri = &M[32];
    ByteToBit(M, In, 64);
    Transform(M, M, IP, 64);
    for (int i=15; i>=0; i--)
    {
        memcpy(Tmp, Li, 32);
        FFunc(Li, SubKey[i]);
        Xor(Li, Ri, 32);
        memcpy(Ri, Tmp, 32);
    }
    Transform(M, M, IP_inv, 64);
    BitToByte(Out, M, 64);
}
void CBCDESENrun(char Out[8], char In[8])
{
    static bool M[64],N[64], Tmp[32];
    static bool *Li = &M[0];
    static bool *Ri = &M[32];
    ByteToBit(M, In, 64);
    ByteToBit(N, Out, 64);
    Xor(M,N, 64);
    Transform(M, M, IP, 64);
    for (int i=0; i<16; i++)
    {
        memcpy(Tmp, Ri, 32);
        FFunc(Ri, SubKey[i]);
        Xor(Ri, Li, 32);
        memcpy(Li, Tmp, 32);
    }
    Transform(M, M, IP_inv, 64);
    BitToByte(Out, M, 64);
}
void CBCDESDErun(char Out[8], char In[8])
{
    static bool M[64],N[64],Tmp[32];
    static bool *Li = &M[0];
    static bool *Ri = &M[32];
    ByteToBit(M, In, 64);
    ByteToBit(N, Out, 64);
    Transform(M, M, IP, 64);
    for (int i=15; i>=0; i--)
    {
        memcpy(Tmp, Li, 32);
        FFunc(Li, SubKey[i]);
        Xor(Li, Ri, 32);
        memcpy(Ri, Tmp, 32);
    }
    Transform(M, M, IP_inv, 64);
    Xor(M,N, 64);
    BitToByte(Out, M, 64);
}

void GetKey(void)
{
    char Key[9]="kugwzkku";
    static bool K[64];
    static bool *KL = &K[0];
    static bool *KR = &K[28];
    ByteToBit(K, Key, 64);
//pc_1
    Transform(K, K, PC1, 56);
    for (int i=0; i<16; i++)
    {
        RotateL(KL, 28, LOOP[i]);
        RotateL(KR, 28, LOOP[i]);
//pc_2
        Transform(SubKey[i], K, PC2, 48);
    }
}

//A Round of DES
void FFunc(bool In[32], const bool Ki[48])
{
    static bool MR[48];
//E Transform
    Transform(MR, In, E, 48);
    Xor(MR, Ki, 48);
    SFunc(In, MR); //S BOX
//P Transform
    Transform(In, In, P, 32);
}

void SFunc(bool Out[32], const bool In[48])
{
//i<8 <==> 8's S-BOX;In(6)==>Out(4)
    for (char i=0,j,k; i<8; i++,In+=6,Out+=4)
    {
        j = (In[0]<<1) + In[5];
        k = (In[1]<<3) + (In[2]<<2) + (In[3]<<1) + In[4];
        ByteToBit(Out, &SBox[i][j][k], 4);
    }
}

//Trabsform through TABLE
void Transform(bool *Out, bool *In, const char *Table, int len)
{
    static bool Tmp[256];
    for (int i=0; i<len; i++)
        Tmp[i] = In[ Table[i]-1 ];
    memcpy(Out, Tmp, len);
}
void Xor(bool *InA, const bool *InB, int len)
{
    for (int i=0; i<len; i++)
        InA[i] ^= InB[i];
}
void RotateL(bool *In, int len, int loop)
{
    static bool Tmp[256];
    memcpy(Tmp, In, loop);
    memcpy(In, In+loop, len-loop);
    memcpy(In+len-loop, Tmp, loop);
}
void ByteToBit(bool *Out, const char *In, int bits)
{
    for (int i=0; i<bits; i++)
        Out[i] = (In[i/8]>>(i%8)) & 1;
//Get the last of the Byte
}
void BitToByte(char *Out, const bool *In, int bits)
{
    memset(Out, 0, (bits+7)/8);
    for (int i=0; i<bits; i++)
        Out[i/8] |= In[i]<<(i%8);
}
void EncryptFile() {
    char plain[8]={0};
    char header[62]={0};
    char cipher2[8] = {0};
    char cipher1[8] = {'k','u','g','w','z','k','k','u'};
    int len=0;
    FILE *iobmp  = NULL;
    FILE *cbcbmp = NULL;
    FILE *ecbbmp = NULL;
    iobmp = fopen("b.bmp","rb");
    cbcbmp = fopen("CBC.bmp","wb");
    ecbbmp = fopen("ECB.bmp","wb");
    if (iobmp==NULL||cbcbmp == NULL||ecbbmp == NULL) {
        cout<<"Open File Error!"<<endl;
        return;
    }
    GetKey();
    //读位图
    fread(header,sizeof(char),62,iobmp);
    fwrite(header,sizeof(char),62,cbcbmp);
    fwrite(header,sizeof(char),62,ecbbmp);
    while ((len=fread(plain,sizeof(char),8,iobmp))==8) {
        CBCDESENrun(cipher1,plain);
        fwrite(cipher1,sizeof(char),8,cbcbmp);
        ECBDESENrun(cipher2,plain);
        fwrite(cipher2,sizeof(char),8,ecbbmp);
    }
    //剩余部分
    CBCDESENrun(cipher1,plain);
    fwrite(cipher1,sizeof(char),len,cbcbmp);
    ECBDESENrun(cipher2,plain);
    fwrite(cipher2,sizeof(char),len,ecbbmp);

    fclose(iobmp);
    fclose(cbcbmp);
    fclose(ecbbmp);
}
void DecryptFile() {
    char header1[62]={0};
    char header2[62]={0};
    char plain[8]={'k','u','g','w','z','k','k','u'};
    char cipher[8]={0};
    int len=0;
    FILE* cbcbmp=fopen("CBC.bmp","rb");
    FILE* ecbbmp=fopen("ECB.bmp","rb");
    FILE* cbcBbmp = fopen("CBCback.bmp","wb");
    FILE* ecbBbmp = fopen("ECBback.bmp","wb");
    GetKey();
    fread(header1,sizeof(char),62,cbcbmp);
    fwrite(header1,sizeof(char),62,cbcBbmp);
    fread(header2,sizeof(char),62,ecbbmp);
    fwrite(header2,sizeof(char),62,ecbBbmp);
    while((len=fread(cipher,sizeof(char),8,cbcbmp))==8) {
        CBCDESDErun(plain,cipher);      //plain保留上次
        fwrite(plain,sizeof(char),8,cbcBbmp);
        for (int i=0;i<8;i++) plain[i] = cipher[i];
    }
    CBCDESDErun(plain,cipher);
    fwrite(plain,sizeof(char),8,cbcBbmp);
    while((len=fread(cipher,sizeof(char),8,ecbbmp))==8) {
        ECBDESDErun(plain,cipher);
        fwrite(plain,sizeof(char),8,ecbBbmp);
    }
    ECBDESDErun(plain,cipher);
    fwrite(plain,sizeof(char),len,ecbBbmp);
    fclose(cbcbmp);
    fclose(ecbbmp);
    fclose(cbcBbmp);
    fclose(ecbBbmp);
}
void GetRandSBox() {
    int Temp[16]={0};
    int MaxIndex=15;
    for (int i=0;i<16;i++) Temp[i] = i;
    for (int i = 0;i<8;i++)
    {
        for (int j=0;j<4;j++)
        {
            for (int k=0;k<16;k++)
            {
                int temp = rand()%(MaxIndex+1);
                SBox[i][j][k] = Temp[temp];
                if (MaxIndex > temp)
                    Temp[temp] = Temp[MaxIndex];
                MaxIndex--;
            }
            MaxIndex = 15;
            for (int l=0;l<16;l++)
                Temp[l] = l;
        }
    }
}
void GetLineSBox() {
    for (int j=0;j<4;j++)
        for (int k=0;k<16;k++)
            for (int i = 0;i<8;i++)
                SBox[i][j][k] = k;
}

void SnowAnalysis(bool bits1[],bool bits2[]) {
    int pos=rand()%64;
    int val=rand()&1;
    bits1[pos]=val;
    bits2[pos]=val^1;
    REP(i,0,64) {
        if(i!=pos) bits1[i]=bits2[i]=(rand()&1);
    }
    static bool Tmp[32];
    static bool *Li1 = &bits1[0];
    static bool *Ri1 = &bits1[32];
    static bool *Li2 = &bits2[0];
    static bool *Ri2 = &bits2[32];
    int cnt=0;
    REP(i,0,16) {
        memcpy(Tmp, Ri1, 32);
        FFunc(Ri1, SubKey[i]);
        Xor(Ri1, Li1, 32);
        memcpy(Li1, Tmp, 32);
        memcpy(Tmp, Ri2, 32);
        FFunc(Ri2, SubKey[i]);
        Xor(Ri2, Li2, 32);
        memcpy(Li2, Tmp, 32);
        cnt=0;
        REP(j,0,64) if (bits1[j]!=bits2[j]) cnt++;
        cout<<"The "<<i+1<<" Round's Different Bits: "<<cnt<<endl;
    }
}
void CompleteAnalysis() {
    bool Text[PLAINT_SIZE][64];
    int pos=rand()%64;
    int val=rand()&1;
    for(int i=0;i<PLAINT_SIZE;i++) {
        Text[i][pos]=val;
        for(int j=0;j<64;j++) {
            if(j!=pos) Text[i][j]=rand()&1;
        }
    }
    static bool tmp[32], *Li , *Ri;
    int cnt[64][2]={0};
    for (int i=0;i<PLAINT_SIZE;i++) {
        Li = &Text[i][0];
        Ri = &Text[i][32];
        for(int j=0; j<16; j++) {
            memcpy(tmp, Ri, 32);
            FFunc(Ri, SubKey[i]);
            Xor(Ri, Li, 32);
            memcpy(Li, tmp, 32);
        }
        for (int j=0;j<64;j++){
            if(Text[i][j]==0) cnt[j][0]++;
            else cnt[j][1]++;
        }
    }
    cout<<"Every bit is 1's frequency: "<<endl;
    for (int i=0;i<64;i++)
    {
        cout<<"The "<<i+1<<" bit: "<<100.0*cnt[i][1]/(double)PLAINT_SIZE<<"%"<<endl;
    }
    cout<<"Every bit is 0's frequency: "<<endl;
    for (int i=0;i<64;i++){
        cout<<"The "<<i+1<<" bit: "<<100.0*cnt[i][0]/(double)PLAINT_SIZE<<"%"<<endl;
    }
}

void IntToBit(int var,bool *bit,int len) {
    int tmp = var;
    for(int i=0;i<len;i++) {
        bit[i] = tmp & 1;
        tmp >>= 1;
    }
}
int BitToInt(bool bit[],int len) {
    int var = 0;
    REP(i,0,len) var=var*2+bit[len-i-1];
    return var;
}
void SBoxSingle(bool In[6],bool Out[4],int num) {
    int Line,Row;
    Line = 2*In[0] + In[5];
    Row = 8*In[1]+4*In[2]+2*In[3]+In[4];
    IntToBit(SBox[num][Line][Row],Out,4);
}

void DifferenceAnalysis() {
    int cnt[8][64][16]={0};
    for(int i=0;i<8;i++) {
        cout<<"The "<<i+1<<" S-Box Difference Distributing Table:\n ";
        REP(j,0,16) cout<<" "<<j+1;       //row
        puts("");
        for(int j = 0;j<64;j++) {
            bool In[6]={0};
            IntToBit(j,In,6);
            for (int k=0;k<64;k++){
                bool In1[6]={0},In2[6]={0},Out1[4]={0},Out2[4]={0};
                IntToBit(k,In1,6);
                SBoxSingle(In1,Out1,i);
                IntToBit(j^k,In2,6); // Xor(In,In1,6);
                SBoxSingle(In2,Out2,i);
                Xor(Out1,Out2,4); //Out1 <== Out1 ^ Out2 (Out == Out1)
                cnt[i][j][BitToInt(Out1,4)] ++;
            }
            cout<<j<<": ";
            for (int l=0;l<16;l++)
                cout<<cnt[i][j][l]<<" ";
            cout<<endl;
        }
        cout<<endl;
        system("pause");
    }

}
int main()
{
    cout<<"Please Wait..."<<endl;
    //加密
    EncryptFile();
    DecryptFile();
    bool bits1[64]={0},bits2[64]={0};
    srand(time(NULL));
    cout<<"The analysis of Avalanche effect:"<<endl;
    SnowAnalysis(bits1,bits2);
    CompleteAnalysis();
    DifferenceAnalysis();
    //生成线性盒
    GetLineSBox();
    memset(bits1,0,sizeof(bits1));
    memset(bits2,0,sizeof(bits2));
    SnowAnalysis(bits1,bits2);
    CompleteAnalysis();
    DifferenceAnalysis();
    memset(bits1,0,sizeof(bits1));
    memset(bits2,0,sizeof(bits2));
    //生成随机盒
    GetRandSBox();
    SnowAnalysis(bits1,bits2);
    CompleteAnalysis();
    DifferenceAnalysis();
}
