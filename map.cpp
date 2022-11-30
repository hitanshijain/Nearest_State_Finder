#include <iostream>
#include <string.h>
#include <vector>
#include <map>
using namespace std;
 


void map_sort(vector<string> county, vector<double> distance, int n){
    map<double, string> mymap;
    for (int i = 0; i < county.size(); i++){
        mymap[distance[i]] = county[i];
    }
    int count = 0;
    for (auto it = mymap.begin(); (count < n) && (it != mymap.end()); ++it){
        count++;
        cout << it->second << " : "
             << it->first << '\n';
    }
}

int main()
{
    vector<string> county;
    vector<double> distance;

    county.push_back("Beijing");
    county.push_back("Shanghai");
    county.push_back("Boston");

    distance.push_back(2500);
    distance.push_back(1550);
    distance.push_back(1);

    map_sort(county, distance, 2);

    return 0;
}