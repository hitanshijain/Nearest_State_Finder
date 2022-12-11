#include "iostream"
#include "string"
#include <iomanip>
#include "vector"
#include <fstream>
#include <sstream>
#include <chrono>
#include "map"
#include "math.h"
#include <string.h>
#include <algorithm>
#include <chrono>



using namespace std;

const double PI = 3.14159265358979323846;
const double EARTH_RADIUS = 6378.137;
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

string base32_table="0123456789bcdefghjkmnpqrstuvwxyz";

vector<vector<string> > data_loader(string filename){
    vector<string> data_line;
    vector<vector<string> > data;
    ifstream ifs;
    ifs.open(filename, ios::in);
    if (!ifs.is_open())
    {
        cout << "Failed to open file";
        exit(-1);
    }
    vector<string> item;
    string temp;
    while (getline(ifs, temp))
    {
        item.push_back(temp);
    }
    for (auto & it : item){
        string str;
        istringstream istr(it);
        getline(istr, str, ',');
        data_line.push_back(str);
        getline(istr, str, ',');
        data_line.push_back(str);
        getline(istr, str, ',');
        data_line.push_back(str);
        getline(istr, str, ',');
        data_line.push_back(str);
        getline(istr, str, ',');
        data_line.push_back(str);
        data.push_back(data_line);
        data_line.clear();
    }
    return data;

}

string encode_latitude(double latitude,int code_digits){
    double lat_min=-90,lat_max=90,lat_mid=0;
    string latitude_encoded;
    for(int i=0;i<code_digits;i++){
        if(latitude<lat_mid){
            latitude_encoded+="0";
            lat_max=lat_mid;
            lat_mid=(lat_min+lat_max)/2;
        } else{
            latitude_encoded+="1";
            lat_min=lat_mid;
            lat_mid=(lat_min+lat_max)/2;
        }
    }
    return latitude_encoded;
}
string encode_longitude(double longitude,int code_digits){
    double lat_min=-180,long_max=180,long_mid=0;
    string latitude_encoded;
    for(int i=0;i<code_digits;i++){
        if(longitude<long_mid){
            latitude_encoded+="0";
            long_max=long_mid;
            long_mid=(lat_min+long_max)/2;
        } else{
            latitude_encoded+="1";
            lat_min=long_mid;
            long_mid=(lat_min+long_max)/2;
        }
    }
    return latitude_encoded;
}

string encode_location(string latitude_encoded, string longitude_encoded){
    int len=latitude_encoded.length(),long_len=longitude_encoded.length();
    string location_encoded;
    if(len!=long_len){
        return "Error";
    }
    for(int i=0;i<len;i++){
        location_encoded+=longitude_encoded[i];
        location_encoded+=latitude_encoded[i];
    }
    return location_encoded;
}

string base32_encode(string location_encoded){

    int bin_len=location_encoded.length();
    int base32_len=bin_len/5;
    int tmp_dec;
    string tmp_str, location_base32_encoded;

    for(int i=0;i<base32_len;i++){
        tmp_str=location_encoded.substr(i*5,5);
        tmp_dec=stoi(tmp_str, nullptr,2);
        location_base32_encoded=location_base32_encoded+base32_table[tmp_dec];
    }
    return location_base32_encoded;

}

bool startsWith(const string &str, const string &prefix) {
    return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
}


double rad(double d)
{
    return d * PI /180.0;
}

double distance(double lat1,double lng1,double lat2,double lng2) //https://blog.csdn.net/weixin_41519463/article/details/88999339
{

    double a;
    double b;
    double radLat1 = rad(lat1);
    double radLat2 = rad(lat2);
    a = radLat1 - radLat2;
    b = rad(lng1) - rad(lng2);
    double s = 2 * asin(sqrt(pow(sin(a/2),2) + cos(radLat1)*cos(radLat2)*pow(sin(b/2),2)));
    s = s * EARTH_RADIUS;
    s = s * 1000;
    return s;
}

vector<vector<string> > search_counties(vector<vector<string> > data, string your_location, int target_num){
    vector<vector<string> > temp,result;
    string prefix;
    int prefix_len=5;
    while(temp.size()<target_num){
        temp.clear();
        prefix_len--;
        prefix=your_location.substr(0,prefix_len);
        for(int i=0;i<data.size();i++){
            if(startsWith(data[i][1],prefix)){
                temp.push_back(data[i]);
            }
        }
    }

    int real_target_num=target_num*5;
    if(temp.size()<real_target_num){
        return temp;
    }

    return temp;
}

void search_outside_NA(double latitude_input,double longitude_input, int number_input){
    cout<<"\nR-B Tree method is applied\n"<<endl;
    vector<vector<string>> content;
    vector<state_county> county;
    vector<double> distance;
    
    state_county city_county_pair;

    string fname;
    string line, word;
    vector<string> row;

    // cout << "Enter the file name: ";
    // cin >> fname;

    fname="./data/uscounties_database.csv";

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
    double latitud2=latitude_input;
    double longitud2=longitude_input;
    int num_counties=number_input;

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
    return;
}


int main(){
    int code_digits=20, target_num=10; //make sure this is the same as data's code digits
    double your_latitude,your_longitude;
    chrono::time_point<chrono::steady_clock> start_point, stop_point;
    string lat_encoded,long_encoded,location_encoded,your_location;
    vector<vector<string> > result;
    cout<<"loading data..."<<endl;
    vector<vector<string> > uscounties=data_loader("./data/uscounties_encoded.csv");
    cout<<"Input your latitude[-90,90]: "<<endl;
   cin>>your_latitude;
    // If you want to assign a fixed locaion for test, uncomment the line below
    // your_latitude=13.485090252383909;
    cout<<"Input your longitude[-180,180]: "<<endl;
   cin>>your_longitude;
    // If you want to assign a fixed locaion for test, uncomment the line below
    // your_longitude=144.8004090355531;
    if(abs(your_latitude)>90|| abs(your_longitude)>180){
        cout<<"Data out of range!"<<endl;
        exit(-1);
    }
    lat_encoded= encode_latitude(your_latitude,code_digits);
    long_encoded= encode_longitude(your_longitude,code_digits);
    location_encoded= encode_location(lat_encoded,long_encoded);
    your_location= base32_encode(location_encoded);
    cout<<"Your location: "<<"["<<your_latitude<<","<<your_longitude<<"]"<<endl;

    // choose method accoring to the latitude and longitude input
    if((your_longitude<=180&&your_longitude>=-50)||(your_latitude<23)){
        search_outside_NA(your_latitude,your_longitude,10);
        return 0;
    }
    cout<<"\nGeohash method is applied\n"<<endl;
    cout<<"Geohash code: "<<your_location<<endl;
    start_point = chrono::steady_clock::now();
    result=search_counties(uscounties,your_location,target_num);
    double distance_tmp;
    bool insert_flag=false;
    vector<vector<string> > result_sorted;
    for(auto & i : result){
        insert_flag= false;
        distance_tmp= distance(your_latitude,your_longitude,stod(i[3]),stod(i[4]));
        i.insert(i.begin(), to_string(distance_tmp));
        if(result_sorted.empty()){
            result_sorted.push_back(i);
        } else{
            for(int j=0;j<result_sorted.size();j++){
                if(stod(i[0])<stod(result_sorted[j][0])){
                    result_sorted.insert(result_sorted.begin()+j,i);
                    insert_flag=true;
                    break;
                }
            }
            if(!insert_flag){
                result_sorted.push_back(i);
            }
        }
    }
    stop_point = chrono::steady_clock::now();
    map<string,int> state_vote;

    cout<<target_num<<" counties found: "<<endl;
    cout<<setw(20)<<left<<"County"<<"State"<<"\t"<<"Distance(m)"<<endl;
    int count=0;
    for(auto & i : result_sorted){
        cout<<setw(20)<<left<<i[1]<<i[3]<<"\t\t"<<i[0]<<endl;
        state_vote[i[3]]+=1;
        if(count++>target_num-2){
            break;
        }
    }
    int max=0;
    string state_majority;
    for (auto it = state_vote.begin(); it != state_vote.end(); it++){
        if(it->second>max){
            state_majority=it->first;
            max=it->second;
        }
    }
    cout<<"State majority: "<<state_majority<<endl;
    chrono::duration<double> difference_in_time = stop_point - start_point;
    auto difference_in_seconds = double(difference_in_time.count());
    cout<<"Time: "<<difference_in_seconds<<endl;

}