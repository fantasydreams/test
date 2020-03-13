#include <iostream>
#include <vector>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <sys/select.h>

FD_CLR()

using namespace std;
const int Max_size = 1010;
int times[Max_size];
int times_[Max_size];
int main(){
    int n_num, num,x,max_num = 0,max_num_ = 0,old_num = 0;
    cin >> n_num >> num;
    for(int i=0;i<n_num;++i){
        cin >>x;
        if(++times[x] > times[max_num])max_num = x;
        if(++times_[x | num] > times_[max_num_]) {
            max_num_ = x | num;
            old_num = x;
        }
    }

    int max_times = times[max_num] < times_[max_num] ? times_[max_num_] : times[max_num];
    int max_times_ = times[max_num_] < times_[max_num_] ? times_[max_num_] : times[max_num];


}





int _size;
const int max_size = 51;
char matrix[2][max_size];

void get_ways(int i,int j,int& ways){
    if(i >= 2 || j >= _size || i < 0) return;

    if(matrix[i][j] == 'X')return;
    if(j==_size-1 && i == 1){
        ++ways;
        return;
    }
    get_ways(i,j+1,ways);
    get_ways(i-1,j+1,ways);
    get_ways(i+1,j+1,ways);
}




//int main(){
//    cin >> _size;
//    for(int i=0;i<2;++i){
//        for(int j=0;j<_size;++j){
//            cin >> matrix[i][j];
//        }
//
//    }
//
//    int ways = 0;
//    get_ways(0,0,ways);
//    if(ways == 0)ways = -1;
//    cout << ways;
//
//}


