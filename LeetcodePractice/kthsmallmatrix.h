#include <iostream>
#include <vector>
#include <stdio.h>
#include <algorithm>
using namespace std;

int kthSmallest(vector<vector<int>>& matrix, int k)
	{
		int n = matrix.size();
		int le = matrix[0][0], ri = matrix[n - 1][n - 1];
		int mid = 0;
		while (le < ri)
		{
			mid = le + (ri-le)/2;
			int num = 0;
			for (int i = 0; i < n; i++)
			{
				int pos = upper_bound(matrix[i].begin(), matrix[i].end(), mid) - matrix[i].begin();
				cout<<pos<<endl;
				num += pos;
			}
			if (num < k)
			{
				le = mid + 1;
			}
			else
			{
				ri = mid;
			}
		}
		return le;
	}
	
int main_kth()
{
    vector<vector<int>> matrix;
    vector<int> vec={1,5,9};
    matrix.push_back(vec);
    vec = {10,11,13};
    matrix.push_back(vec);
    vec = {12,13,15};
    matrix.push_back(vec);
    
    int result = kthSmallest(matrix,2);
    cout<<result<<endl;
    return 0;
}