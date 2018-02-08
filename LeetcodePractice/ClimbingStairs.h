int climbStairs(int n) 
{
    int fn_1 = 2, fn_2 = 1, fn;
    
    if(n == 1) return fn_2;
    if(n == 2) return fn_1;
    
    for(int i=2; i<n; i++)
    {
        fn = fn_1+fn_2;
        fn_2 = fn_1;
        fn_1 = fn;
    }
    
    return fn;
}


void main_climbingStaris()
{
    int n = 8;
    int num = climbStairs(n);
    cout<<"The number of ways to climb "<<n<<" stairs is "<<num<<endl;
    return;
}
