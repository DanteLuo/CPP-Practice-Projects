#include <iostream>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include "utils.h"
using namespace std;

vector<vector<int>> threeSum(vector<int>& nums) 
{
    vector<vector<int>> results;
    if(nums.size()<3) return results;
    sort(nums.begin(), nums.end());
    
    for(int i=0; i<nums.size()-2; i++)
    {
        int target = -nums[i];
        int left = i+1, right = nums.size()-1;
        
        while(left<right)
        {
            if(nums[left]+nums[right]<target) left++;
            else if(nums[left]+nums[right]>target) right--;
            else
            {
                vector<int> curr = {nums[i], nums[left], nums[right]};
                results.push_back(curr);
                while(left<right && nums[left]==curr[1]) left++;
                while(left<right && nums[right]==curr[2]) right--;
            }
        }
        
        while(i<nums.size()-2 && nums[i+1]==nums[i]) i++;
    }
    
    return results;
}


void main_three_sum()
{
    // vector<int> nums = {-1, 0, 1, 2, -1, -4};
    vector<int> nums = {0};
    vector<vector<int>> results = threeSum(nums);
    print_matrix(results);
}
