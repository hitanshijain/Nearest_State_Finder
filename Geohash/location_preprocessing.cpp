#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "string"

using namespace std;

string base32_table="0123456789bcdefghjkmnpqrstuvwxyz";

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

int main()

{
    double latitude,longitude;
    string state;
    int code_digits=20;
	ifstream ifs;
    ofstream ofs;

	ifs.open("uscounties_database.csv", ios::in);
    ofs.open("uscounties_encoded.csv", ios::out|ios::trunc);

	if (!ifs.is_open()&&ofs.is_open())
	{
		cout << "Failed to open file";
		return 0;
	}

	vector<string> item;

	string temp;

	while (getline(ifs, temp))
	{
		item.push_back(temp);
	}

	for (auto it = item.begin()+1; it != item.end(); it++)
	{
		string str;

		istringstream istr(*it);

		getline(istr, str, ',');
		cout << str << endl;
        ofs<<str<<",";

        for(int i=0;i<3;i++){ //skip several rows
            getline(istr, str, ',');
        }

        getline(istr, str, ',');
        state= str;
        cout << state << endl;

        getline(istr, str, ',');

        getline(istr, str, ',');
        latitude= stod(str);
        cout << latitude << endl;

        getline(istr, str, ',');
        longitude=stod(str);
        cout << longitude << endl;

        string latitude_encoded= encode_latitude(latitude,code_digits);
        string longitude_encoded= encode_longitude(longitude,code_digits);
        string location_encoded= encode_location(latitude_encoded,longitude_encoded);
        string location_base32_encoded= base32_encode(location_encoded);
        cout<<location_base32_encoded<<endl<<endl;
        ofs<<location_base32_encoded<<",";
        ofs<<state<<",";
        ofs<<latitude<<",";
        ofs<<longitude<<endl;

	}
    ofs.close();

	return 0;
}