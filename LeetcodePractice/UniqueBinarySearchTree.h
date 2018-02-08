#include "utils.h"


int numTrees(int n) 
{
    if (n<=0) return 0;
    
    vector<int> res(n+1,0);
    res[0] = 1;
    res[1] = 1;
    
    for (int i=2; i<=n; i++)
    {
        for (int j=0; j<i/2; j++)
        {
            res[i] += res[j]*res[i-j-1];
        }
        
        res[i] *= 2;
        
        if(i%2)
            res[i] += res[i/2]*res[i/2];
    }
    
    return res.back();
}


void main_uniqueBSTNumber()
{
    int n = 17;
    int num = numTrees(n);
    cout<<"The number of unique structured BST is "<<num<<endl;
    return;
}
