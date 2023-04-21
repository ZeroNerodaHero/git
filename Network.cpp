#include "Network.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
using namespace std;

Network::Network(){}; 
void Network::add_user(User u){
    conn.push_back(u);
}
int Network::write_friends(string fout){
    ofstream out(fout);
    if(out.fail()){return -1;}
    out << num_users() << endl; 
    for(auto it : conn){
        out << it.getId() << endl;
        out << "\t" << it.getFname() << " " << it.getLname() << endl 
            << "\t" << it.getBday() << endl
            << "\t" << it.getZip() << endl;
        out << "\t";
        for(auto f : it.getConn()){
            out << f << " ";
        } out << endl;
    }
    return 0;
}
int Network::readFriends(string fin){
    //overwrite everything?
    conn.clear();

    ifstream in(fin);
    if(in.fail()){return -1;}

    string tmp;
    getline(in,tmp);
    int nUser = stoi(tmp);

    for(int i = 0; i < nUser; i++){
        string id,byear,zip;
        string name, fstring;
        
        getline(in,id);
        getline(in,name);
        getline(in,byear);
        getline(in,zip);
        getline(in,fstring);

        stringstream sn(name);
        string fname,lname;
        sn >> fname >> lname;
        stringstream sf(fstring);

        vector<int> v;
        int tmp;
        while(sf >> tmp){
            v.push_back(tmp);
        }
        add_user(User(
            stoi(id),stoi(byear),stoi(zip),fname,lname,v
        ));
    }
    return 0;
}
int Network::add_connection(string s1, string s2){
    int u1 = get_id(s1);
    int u2 = get_id(s2);
    if(u1 == -1 || u2 == -1 || u1 == u2) return -1;
    //mutual friendship so i only need to check once
    bool found = 0;
    for(auto it : conn[u1].getConn()){
        if(it == u2){
            found = 1;
            break;
        }
    }
    if(found) return -1;

    conn[u1].add_friend(u2);
    conn[u2].add_friend(u1);
    return 0;
}
int Network::remove_connection(string s1, string s2){
    int u1 = get_id(s1);
    int u2 = get_id(s2);
    if(u1 == -1 || u2 == -1) return -1;
    //mutual friendship so i only need to check once
    bool found = 0;
    for(auto it : conn[u1].getConn()){
        if(it == u2){
            found = 1;
            break;
        }
    }
    if(!found) return -1;

    conn[u1].delete_friend(u2);
    conn[u2].delete_friend(u1);
    return 0;
}
int Network::get_id(string name){
    for(auto it : conn){
        if(it.getFname()+" "+it.getLname() == name){
            return it.getId();
        }
    }
    return -1;
}
int Network::num_users(){return conn.size();}

User Network::get_user(int id){
    for(auto it : conn){
        if(it.getId()== id){
            return it;
        }
    }
    return User();
}

/*
Note:
My implementation of shortest_path is basically to store a queue of vectors.
Essentially instead of using just integers, I push a whole vector in. I don't actually use
the varaibles from step two because by using a vector I have encapsualted not only the
next users by putting them at the back of the vector and also stored the whole sequence.
*/
vector<int> Network::shortest_path(int from, int to){
    if(from == to) return vector<int>(1,from);
    queue<vector<int> > q;
    q.push(vector<int>(1,from));
    bool v[num_users()];
    memset(v,0,num_users());
    v[from] = 1;
    while(!q.empty()){
        vector<int> tmp = q.front();
        q.pop();
        User cUser = get_user(tmp.back());
        for(auto it:cUser.getConn()){
            if(v[it]) continue;
            v[it] = 1;
            vector<int> nextIt = tmp;
            nextIt.push_back(it);
            if(it == to) return nextIt;
            q.push(nextIt);
        }
    }
    return vector<int>(1,-1);
}
vector<vector<int> > Network::groups(){
    bool v[num_users()];
    memset(v,0,num_users());

    vector<vector<int> > ans;
    for(int i =0 ; i< num_users(); i++){
        if(v[i]) continue;
        v[i] = 1;
        queue<int > q;
        q.push(i);
        vector<int> s;
        s.push_back(i);

        while(!q.empty()){
            int tmp = q.front();
            q.pop();
            User cUser = get_user(tmp);
            for(auto it:cUser.getConn()){
                if(v[it]) continue;
                v[it] = 1;
                s.push_back(it);
                q.push(it);
            }
        }
        ans.push_back(s);
    }
    return ans;
}
vector<int> Network::suggest_friends(int who, int& score){
    unordered_map<int,int> m;
    m[who] = -1;
    int mx = -1;
    vector<int> ret;

    User source =  get_user(who);
    for(auto it:source.getConn()) m[it] = -1;
    for(auto it:source.getConn()){
        for(auto it2:get_user(it).getConn()){
            if(m.find(it2) != m.end()){
                if(m[it2] == -1) continue;
                m[it2]++;
            } else{
                m[it2] = 1;
            }
            if(mx <= m[it2]){
                if(mx != m[it2]) ret.clear();
                mx = m[it2];
                ret.push_back(it2);
            }
        }
    }
    score = mx;
    return ret;
}
vector<int> Network::distance_user(int from, int& to, int distance){
    stack<pair<vector<int>,unordered_set<int> > > s;
    s.push(make_pair(vector<int>(1,from),unordered_set<int>()));
    s.top().second.insert(from);
    
    while(!s.empty()){
        vector<int> tp = s.top().first; 
        if(tp.size() == distance+1){
            to = tp.back();
            return tp;
        }
        unordered_set<int> v = s.top().second;
//cout << (v.find(tp.back()) == v.end() ? "not set" :"set") <<endl;
        s.pop(); 
        User source = get_user(tp.back());
        
        for(auto it : source.getConn()){
            if(v.find(it) != v.end()) continue;
            tp.push_back(it);
            v.insert(it);
            s.push(make_pair(tp,v));
            tp.pop_back();
            v.erase(it);
        }
    }
    to = -1;
    return vector<int>(); 
}
