#include <iostream>
#include <string.h>
#include <vector>
#include <map>
using namespace std;
 
class state_county{
    public:
        string state;
        string county;
};

map<double, state_county> map_sort(vector<state_county> county, vector<double> distance){
    map<double, state_county> mymap;
    for (int i = 0; i < county.size(); i++){
        mymap[distance[i]] = county[i];
    }
    return mymap;
}

string findMajority(vector<string> state)
{
    int i = -1;
    string candidate;
    int votes = 0;
    // Finding majority candidate
    for (i = 0; i < state.size(); i++) {
        if (votes == 0) {
            candidate = state[i];
            votes = 1;
        }
        else {
            if (state[i] == candidate)
                votes++;
            else
                votes--;
        }
    }
    int count = 0;
    // Checking if majority candidate occurs more than n/2
    // times
    for (i = 0; i < state.size(); i++) {
        if (state[i] == candidate)
            count++;
    }
 
    if (count > state.size() / 2)
        return candidate;
    return "None";
}

void county_state_print(map<double, state_county> mymap, int num_counties){
    int count = 0;
    vector<string> state_set;
    for (auto it = mymap.begin(); (count < num_counties) && (it != mymap.end()); ++it){
        count++;
        state_county temp = it->second;
        state_set.push_back(temp.state);
        cout << temp.county << ", " << temp.state << ": "
             << it->first << '\n';   
    }
    cout << findMajority(state_set);
}

int main()
{
    vector<state_county> county;
    vector<double> distance;
    
    state_county city_county_pair;
    city_county_pair.state = "MA";
    city_county_pair.county = "Boston";
    // city_county_pair.setVal("MA", "Boston");
    county.push_back(city_county_pair);
    city_county_pair.state = "MA";
    city_county_pair.county = "Brooks";
    // city_county_pair.setVal("MA", "Brooks");
    county.push_back(city_county_pair);
    city_county_pair.state = "MA";
    city_county_pair.county = "Cambridge";
    // city_county_pair.setVal("NY", "New York");
    county.push_back(city_county_pair);

    distance.push_back(2500);
    distance.push_back(1550);
    distance.push_back(1);

    map<double, state_county> mymap = map_sort(county, distance);

    int num_counties = 2;

    county_state_print(mymap, num_counties);

    // int count = 0;
    // for (auto it = mymap.begin(); (count < num_counties) && (it != mymap.end()); ++it){
    //     count++;
    //     state_county temp = it->second;
    //     state_set.push_back(temp.state);
    //     cout << temp.county << ", " << temp.state << ": "
    //          << it->first << '\n';
        
    // }

    // cout << findMajority(state_set);
    return 0;
}