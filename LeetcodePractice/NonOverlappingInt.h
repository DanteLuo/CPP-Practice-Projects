#include <iostream>
#include <vector>
#include <stdio.h>
#include <algorithm>
using namespace std;


struct Interval 
{
    int start;
    int end;
    Interval() : start(0), end(0) {}
    Interval(int s, int e) : start(s), end(e) {}
};
 
 
int eraseOverlapIntervals(vector<Interval>& intervals, bool verbose=false) 
{
    int results = 0;
    sort(intervals.begin(), intervals.end(), [](Interval lhs, Interval rhs) { return lhs.end < rhs.end; });
    int endOfLast = intervals[0].end;
    
    for (int i=1; i<intervals.size(); i++)
    {
        if(verbose)
        {
            cout<<"The start of next event is "<<intervals[i].start<<" and the end of last event is "
                <<intervals[i-1].end<<endl;
        }

        if(intervals[i].start<endOfLast)
        {
            results++;
            continue;
        }
        
        endOfLast = intervals[i].end;
    }
    
    cout<<"We should skip "<<results<<" events."<<endl;
    
    return results;
} 


void main_overlap(bool verbose=false)
{
    Interval a(1,100), b(1,11), c(2,12), d(11,22);
    vector<Interval> intervals = {a,b,c,d};
    eraseOverlapIntervals(intervals, verbose);
    return;
}