#include <iostream>
#include <stdio.h>
using namespace std;

int mySqrt(int x) 
{
    if(x < 0) return -1;
    else if(x == 0) return 0;
    
    int results = 0;
    int left = 1, right = x/2+1;
    
    while(left<=right)
    {
        int mid = (left+right)/2;
        
        if(x/mid >= mid && x/(mid+1)<(mid+1))
            return mid;
        else if(x/mid < mid)
            right = mid-1;
        else
            left = mid+1;
    }
    
    return 0;
}


int newton_sqrt(int x)
{
    if(x < 0) return -1;
    else if(x == 0) return 0;
    
    int results = 0;
    double y = 1, lastY = 1;
    
    while(lastY != y)
    {
        lastY = y;
        y = (y+x/y)/2;
    }
    
    return static_cast<int>(y);
}


void main_sqrt()
{
    int n = 8;
    int sqrt_n = mySqrt(n);
    cout<<"The sqrt of "<<n<<" is "<<sqrt_n<<" by binary search."<<endl;
    
    int newton_n = newton_sqrt(n);
    cout<<"The sqrt of "<<n<<" is "<<sqrt_n<<" by Newton method."<<endl;
}
