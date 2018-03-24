#include <bits/stdc++.h>
#define maxn 100010
#define inf 0x3f3f3f3f
#define REP(i,x,y) for(int i=x;i<(y);i++)
#define RREP(i,x,y) for(int i=x;i>(y);i--)
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;
char ma[30][30]={{'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'}
                    ,{'b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','a'}
		            ,{'c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','a','b'}
		            ,{'d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','a','b','c'}
		            ,{'e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','a','b','c','d'}
		            ,{'f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','a','b','c','d','e'}
		            ,{'g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','a','b','c','d','e','f'}
		            ,{'h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','a','b','c','d','e','f','g'}
		            ,{'i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','a','b','c','d','e','f','g','h'}
		            ,{'j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','a','b','c','d','e','f','g','h','i'}
		            ,{'k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','a','b','c','d','e','f','g','h','i','j'}
		            ,{'l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','a','b','c','d','e','f','g','h','i','j','k'}
		            ,{'m','n','o','p','q','r','s','t','u','v','w','x','y','z','a','b','c','d','e','f','g','h','i','j','k','l'}
		            ,{'n','o','p','q','r','s','t','u','v','w','x','y','z','a','b','c','d','e','f','g','h','i','j','k','l','m'}
		            ,{'o','p','q','r','s','t','u','v','w','x','y','z','a','b','c','d','e','f','g','h','i','j','k','l','m','n'}
		            ,{'p','q','r','s','t','u','v','w','x','y','z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o'}
		            ,{'q','r','s','t','u','v','w','x','y','z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p'}
		            ,{'r','s','t','u','v','w','x','y','z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q'}
		            ,{'s','t','u','v','w','x','y','z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r'}
		            ,{'t','u','v','w','x','y','z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s'}
		            ,{'u','v','w','x','y','z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t'}
		            ,{'v','w','x','y','z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u'}
		            ,{'w','x','y','z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v'}
		            ,{'x','y','z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w'}
		            ,{'y','z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x'}
		            ,{'z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y'}};
double d[] = {0.082,0.015,0.028,0.043,0.127,0.022,0.020,0.061,0.070,0.002,0.008,0.040,0.024,0.067,0.075,0.019
    		,0.001,0.060,0.063,0.091,0.028,0.010,0.024,0.002,0.020,0.001};
int n;
vector<int>vec;
vector<int>gcds;
string key;
string s;           //明文
string t;           //密文
string sa;          //解密后的字符串
inline char encrypt(char ch,int pos) {
    int row=ch-'a';
    int col=key[pos%key.length()]-'a';
    return ma[row][col];
}
inline char decrypt(char ch,int pos) {
    int tmp=key[pos%key.length()]-'a';
    return (char)((ch-'a'+26-tmp)%26+'a');
}
int gcd(int a,int b) {
    return b==0 ? a : gcd(b,a%b);
}
double c[30];
const double eps=1e-3;
vector<string>SS;
int cnt_t[30];
int cnt_s[30];
int main()
{
    srand(time(NULL));
    printf("Please enter the length of key: \n");
    scanf("%d",&n);
    REP(i,1,n+1) {
        char tmp=(char)(rand()%26+'a');
        key=key+tmp;
    }
    ofstream out("key.txt",ios::out);
    out<<"key: "<<" "<<key<<endl;
    //读取文件
    ifstream in("text.txt");
    char ch;
    int pos=0;
    while(in>>ch) {
        if(!isalpha(ch)) continue;
        if(isalpha(ch)==1) ch=ch-'A'+'a';
        s+=ch;
        t+=encrypt(ch,pos);
        pos++;
        cnt_s[ch-'a']++;
    }
    in.close();
    ofstream out1("cipher.txt",ios::out);
    //cout<<"s:"<<" "<<s<<endl;
    out1<<t<<endl;           //t为加密后的串
  //  cout<<t<<endl;
    int len=t.length();
    REP(i,0,len) cnt_t[t[i]-'a']++;
    for(int i=0;i<len;i++)
        sa=sa+decrypt(t[i],i);
    ofstream out2("decrypt.txt");
    out2<<sa<<endl;
    //计算重合指数
    double sum=0;
    REP(i,0,26) sum+=1.0*cnt_s[i]*(cnt_s[i]-1);
    sum/=(double)(len*(len-1));
    double sum2=0;
    REP(i,0,26) sum2+=1.0*cnt_t[i]*(cnt_t[i]-1);
    sum2/=(double)(len*(len-1));
    ofstream out3("CI.txt");
    out3<<sum<<" "<<sum2<<endl;
    //通过kasiski法找寻密钥长度
    pos=0;
    for(int i=0;i<len-2;i++) {
        string need=string(1,t[i]);
        need=need+t[i+1];
        need=need+t[i+2];
        //cout<<need<<endl;
        int j=0,flag=0;
        vec.clear();
        while(j<len&&t.find(need,j)!=-1) {
            flag=1;
            j=t.find(need,j);
            vec.push_back(j);
            j+=3;
        }           //计算距离
        if(flag) {  //求gcd
            int tmp=vec[0];
            REP(i,1,vec.size()) tmp=gcd(tmp,vec[i]);
            gcds.push_back(tmp);
        }
    }
    ofstream out4("kasiski.txt");
    out4<<"The length of key maybe: "<<endl;
    REP(i,0,gcds.size())
       out4<<" "<<gcds[i];
    out4<<endl;
    //明文字符频率
    for(int i=0;i<26;i++) cout<<i<<" "<<1.0*cnt_s[i]/(double)s.length()<<endl;
    for(int i=0;i<26;i++) cout<<i<<" "<<1.0*cnt_t[i]/(double)t.length()<<endl;

}
