#include "iostream"
#include "string"
#include <iomanip>
#include "vector"
#include <fstream>
#include <sstream>
#include <chrono>
#include "map"
#include "math.h"

#define pi 3.1415926535897932384626433832795
#define EARTH_RADIUS 6378.137

using namespace std;

string base32_table="0123456789bcdefghjkmnpqrstuvwxyz";

vector<vector<string>> data_loader(string filename){
    vector<string> data_line;
    vector<vector<string>> data;
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
    return d * pi /180.0;
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

vector<vector<string>> search_counties(vector<vector<string>> data, string your_location, int target_num){
    vector<vector<string>> temp,result;
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

//    int index=base32_table.find(your_location[prefix_len]),extension=0;
//    do{
//        string tmp1,tmp2;
//        if(index+extension<31){
//            tmp1=prefix+base32_table[index+extension];
//            for(int i=0;i<temp.size();i++){
//                if(startsWith(temp[i][1],tmp1)){
//                    result.push_back(temp[i]);
//                }
//            }
//        }
//
//        if(index-extension>=0&&extension!=0){
//            tmp2=prefix+base32_table[index-extension];
//            for(int i=0;i<temp.size();i++){
//                if(startsWith(temp[i][1],tmp2)){
//                    result.push_back(temp[i]);
//                }
//            }
//        }
//        extension++;
//        if(extension>31){
//            cout<<"Error"<<endl;
//            exit(-1);
//        }
//
//    }while(result.size()<real_target_num);

    return temp;
}


int main(){
    int code_digits=20, target_num=10; //make sure this is the same as data's code digits
    double your_latitude,your_longitude;
    chrono::time_point<chrono::steady_clock> start_point, stop_point;
    string lat_encoded,long_encoded,location_encoded,your_location;
    vector<vector<string>> result;
    cout<<"loading data..."<<endl;
    vector<vector<string>> uscounties=data_loader("./uscounties_encoded.csv");
    cout<<"Input your latitude[-90,90]: "<<endl;
//    cin>>your_latitude;
    your_latitude=13.485090252383909;
    cout<<"Input your longitude[-180,180]: "<<endl;
//    cin>>your_longitude;
    your_longitude=144.8004090355531;
    if(abs(your_latitude)>90|| abs(your_longitude)>180){
        cout<<"Data out of range!"<<endl;
        exit(-1);
    }
    lat_encoded= encode_latitude(your_latitude,code_digits);
    long_encoded= encode_longitude(your_longitude,code_digits);
    location_encoded= encode_location(lat_encoded,long_encoded);
    your_location= base32_encode(location_encoded);
    cout<<"Your location: "<<"["<<your_latitude<<","<<your_longitude<<"]"<<endl;
    cout<<"Geohash code: "<<your_location<<endl;
    start_point = chrono::steady_clock::now();
    result=search_counties(uscounties,your_location,target_num);
    double distance_tmp;
    bool insert_flag=false;
    vector<vector<string>> result_sorted;
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

//    cout<<result_sorted.size()<<" counties found: "<<endl;
//    for(auto & i : result_sorted){
//        cout<<setw(20)<<left<<i[1]<<i[3]<<" "<<i[0]<<endl;
//    }
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