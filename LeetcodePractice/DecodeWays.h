#include "utils.h"


int numDecodings(string s) 
{
    if(s[0] == '0' || s.empty()) return 0;
    
    int num1=1, num2=1, num3=1;
    
    for (int i=1; i<s.size(); i++)
    {
        if(s[i]=='0')
        {
            if(s[i-1]=='1'||s[i-1]=='2') num3 = num1;
            else return 0;
        }
        else if(s[i-1]=='1') num3 = num2+num1;
        else if(s[i-1]=='2' && s[i]>='1' && s[i]<='6') num3 = num2+num1;
        else num3 = num2;
        num1 = num2;
        num2 = num3;
    }
    return num3;
}


void main_decodeWays()
{
    string s = "234558484";
    int n = numDecodings(s);
    cout<<"Number of decoding "<<s<<" is "<<n<<endl;
    return;
}
