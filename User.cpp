#include "User.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

User::User(){
    id = rand();
    bday = zip = -1;
    f_name = l_name = "Not Set"; 
} 
User::User(int id, int bday, int zip, string f_name, string l_name, vector<int> conn){
    this->id = id;
    this->bday = bday;
    this->zip = zip;
    this->f_name = f_name;
    this->l_name = l_name;
    this->conn = conn; 
} 
void User::add_friend(int f_id){
    conn.push_back(f_id);
}
void User::delete_friend(int f_id){
    auto it = find(conn.begin(),conn.end(),f_id);
    if(it != conn.end())
        conn.erase(it);
}
