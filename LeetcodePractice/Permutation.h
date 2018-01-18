#include <iostream>
#include <vector>
#include <stdio.h>
using namespace std;

void
print_vec (vector<int>nums)
{
  for (int i = 0; i < nums.size (); i++)
    cout << nums[i];
  cout << endl;
}

void
permute_rec (vector<int>nums, vector<vector<int>>& results, int start,
	     int end)
{
  if (start == end)
    {
      results.push_back (nums);
      return;
    }
  else
    {
      for (int i = start; i <= end; i++)
	{
	  swap (nums[i], nums[start]);
	  permute_rec (nums, results, start + 1, end);
 	  swap (nums[i], nums[start]);
	}
    }
}

vector<vector<int>>
permute (vector<int>&nums)
{
  vector<vector<int>>results;
  permute_rec (nums, results, 0, nums.size () - 1);
  for(int i=0; i<results.size();i++)
    print_vec(results[i]);
  return results;
}

int
main_permute ()
{
  vector<int>nums;
  nums.push_back(1);
  nums.push_back(2);
  nums.push_back(3);
  permute(nums);
  return 0;
}
