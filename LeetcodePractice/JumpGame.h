#include <iostream>
#include <vector>
#include <stdio.h>
#include <algorithm>
using namespace std;


bool canJump(vector<int>& nums) 
{
    int currHeight = 0;
    for (int i = 0; i < nums.size() && i <= currHeight; i++)
    {
        currHeight = max(i + nums[i], currHeight);
    }
        
    if(currHeight>=nums.size())
        cout<<"You can reach the end of vector!"<<endl;
    else
        cout<<"You fail to reach the end of vector!"<<endl;
        
    return currHeight >= nums.size();
}


void main_jumpGame()
{
    vector<int> nums = {2,3,1,1,4};
    canJump(nums);
    return;
}