#include <iostream>
#include <string.h>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <math.h>
#include <algorithm>
#include <chrono>

using namespace std;

const double PI = 3.14159265358979323846;
const double RADIO_TERRESTRE = 6372797.56085;
const double GRADOS_RADIANES = PI / 180;

class state_county
{
public:
    string state;
    string county;
};

map<double, state_county> map_sort(vector<state_county> county, vector<double> distance)
{
    map<double, state_county> mymap;
    for (int i = 0; i < county.size(); i++)
    {
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
    for (i = 0; i < state.size(); i++)
    {
        if (votes == 0)
        {
            candidate = state[i];
            votes = 1;
        }
        else
        {
            if (state[i] == candidate)
                votes++;
            else
                votes--;
        }
    }
    int count = 0;
    // Checking if majority candidate occurs more than n/2
    // times
    for (i = 0; i < state.size(); i++)
    {
        if (state[i] == candidate)
            count++;
    }

    // if (count > state.size() / 2)
    //     return candidate;
    return candidate;
}

void county_state_print(map<double, state_county> mymap, int num_counties)
{
    int count = 0;
    vector<string> state_set;
    for (auto it = mymap.begin(); (count < num_counties) && (it != mymap.end()); ++it)
    {
        count++;
        state_county temp = it->second;
        state_set.push_back(temp.state);
        cout << temp.county << ", " << temp.state << ": "
             << it->first << '\n';
    }
    cout << findMajority(state_set) << endl;
}

double CalcGPSDistance(double latitud1, double longitud1, double latitud2, double longitud2){
    double haversine;
    double temp;
    double distancia_puntos;

    latitud1  = latitud1  * GRADOS_RADIANES;
    longitud1 = longitud1 * GRADOS_RADIANES;
    latitud2  = latitud2  * GRADOS_RADIANES;
    longitud2 = longitud2 * GRADOS_RADIANES;

    haversine = (pow(sin((1.0 / 2) * (latitud2 - latitud1)), 2)) + ((cos(latitud1)) * (cos(latitud2)) * (pow(sin((1.0 / 2) * (longitud2 - longitud1)), 2)));
    temp = 2 * asin(min(1.0, sqrt(haversine)));
    distancia_puntos = RADIO_TERRESTRE * temp;

   return distancia_puntos;
}

int main()
{
    vector<vector<string>> content;
    vector<state_county> county;
    vector<double> distance;
    
    state_county city_county_pair;

    string fname;
    string line, word;
    vector<string> row;

    cout << "Enter the file name: ";
    cin >> fname;

    fstream file(fname, ios::in);
    if (file.is_open())
    {
        while (getline(file, line))
        {
            row.clear();

            stringstream str(line);

            while (getline(str, word, ','))
                row.push_back(word);
            content.push_back(row);
        }
    }
    else{
        cout << "Could not open the file\n";
    }
    double latitud2;
    double longitud2;
    int num_counties;

    cout << "Enter the referrence point: " << endl;
    cout << "Enter the latitude: ";
    cin >> latitud2;

    cout << "Enter the longitude: ";
    cin >> longitud2;

    cout << "Enter the number of the closest city: ";
    cin >> num_counties;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    for(long i = 1; i<content.size(); i++){
        double latitud1 = stod(content[i][6]);
        double longitud1 = stod(content[i][7]);
        city_county_pair.state = content[i][5];
        city_county_pair.county = content[i][0];
        distance.push_back(CalcGPSDistance(latitud1, longitud1, latitud2, longitud2));
        county.push_back(city_county_pair);
    }
    map<double, state_county> mymap = map_sort(county, distance);
    county_state_print(mymap, num_counties);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference (sec) = " <<  (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) /1000000.0<< "s" <<std::endl;
    return 0;
}