#include "custom_factors.h"

using namespace std;

int offset = 2;

vector<int> get_custom_factor(vector<int>& icfl,int word_size){
    int j,z;
    vector<int> temp,res;

    res.push_back(0);
    //Il primo indice sarà sempre 0, possiamo iniziare da i=1.
    for(int i=1;i<icfl.size();i++){
        for(j=1;i>0 && icfl[i]-(offset*j) > icfl[i-1];j++)
            temp.push_back(icfl[i]-(offset*j));
        
        for(z=temp.size()-1;z>=0;z--)
            res.push_back(temp[z]);
        res.push_back(icfl[i]);
        temp.clear();
    }

    for(j=1;word_size-(offset*j) > icfl[icfl.size()-1];j++)
        temp.push_back(word_size-(offset*j));
    
    for(z=temp.size()-1;z>=0;z--)
        res.push_back(temp[z]);
    temp.clear();

    //printVec(res);
    //for(int i=0;i<word_size;i++){
    //    cout<<i<<" "<<check_if_custom_index(icfl,word_size,i)<<"\n";
    //}


    return res;
}

bool check_if_custom_index(vector<int>& icfl,int word_size,int index){

    for(int i=1;i<icfl.size();i++){
        //cout<<"("<<icfl[i-1]<<","<<icfl[i]<<","<<index<<")";
        if(index>=icfl[i-1] && index<icfl[i]){
            if((icfl[i] - index) <= offset) return false;
        }
    }

    //cout<<"word_size: "<<word_size<<" index: "<<index<<" offset: "<<offset<<"\n";
    if((word_size - index) <= offset) return false;

    return true;


}

bool check_if_normal_index(vector<int>& icfl,int word_size,int index){
    return !check_if_custom_index(icfl,word_size,index);
}