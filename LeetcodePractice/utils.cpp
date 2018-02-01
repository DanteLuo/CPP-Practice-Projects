#include "utils.h"
using namespace std;


void print_vec(vector<int>nums)
{
    cout<< "[";
    for (int i = 0; i < nums.size (); i++)
        cout << nums[i] <<",";
    cout <<"]"<< endl;
}


void print_matrix(vector<vector<int>> matrix)
{
    cout<<"["<<endl;
    for (int i=0; i<matrix.size(); i++)
        print_vec(matrix[i]);
    cout<<"]";
}
