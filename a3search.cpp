#include <iostream>
#include <dirent.h>
#include <fstream>
#include <string>
#include <stdio.h>
#include <vector>
#include <map>
#include <sstream>
//#include <cstring>
#include <cctype>
#include <algorithm>
#include <set>
#include <regex>
#include "stmr.h"
#include <time.h>
using namespace std;




void createIndex(char* searchfolder,char* indexfolder){
    DIR *dir=NULL;
    struct dirent *ent=NULL;
    //time_t first,second,third;
    //first=time(NULL);
    set<string>stopwordset;
    string stopword;
    string stopwordpath="stopwords.cpp";
    ifstream stopwordstream;
    stopwordstream.open(stopwordpath);
    if(stopwordstream.is_open()){
        //
        while(stopwordstream>>stopword){
            stopwordset.insert(stopword);
            //cout<<stopword<<endl;
        }
    }

    string folder=searchfolder;
    string index_folder=indexfolder;
    dir = opendir(indexfolder);
    //map
    if (dir==NULL)
    {
    system(("mkdir -p " +index_folder).c_str());
    dir =opendir(searchfolder);
    int fileId=1;
    //int time=1;
    map<string,map<int,int> > wordindex;
    if (dir!=NULL){
        //cout<<"here"<<endl;
        while((ent = readdir(dir)) != NULL){
            if(string(ent->d_name).compare(".") == 0 || string(ent->d_name).compare("..") == 0 ){
                continue;
            }else{
                //cout<<fileId<<endl;
                //first=time(NULL);
                char *fileName=ent->d_name;
                string fileNameString= string(fileName);
                string filePath = folder + "/" + fileNameString;
                string line;
                ifstream infile;
                string fmt="";
                regex r("[^a-zA-Z]");
                infile.open(filePath);
                if(infile.is_open()){
                    while(infile>>line){
                        transform(line.begin(), line.end(), line.begin(), ::tolower);
                        line=regex_replace(line, r, fmt);
                        if (stopwordset.count(line)==0){
                            if(line.length() > 2 &&line.size()<256){
                                char word[256];
                                strcpy(word,line.c_str());
                                int end = stem(word, 0, strlen(word) - 1);
                                word[end + 1] = 0;
                                if (wordindex.count(word)>0){
                                    map<int,int>& file=wordindex[word];
                                    file[fileId]++;
                                }
                                else {
                                    map<int,int>& file=wordindex[word];
                                    file[fileId]=1;
                                }
                            }
                        }
                    }
                }
                fileId++;
                }
            }
        }
    closedir(dir);
    
    dir =opendir(indexfolder);
    ofstream outFile(index_folder+"/"+"words_index.idx");
    map<string,map<int,int> >::const_iterator map_it = wordindex.begin();
        while(map_it!= wordindex.end()){
                outFile << map_it->first<<" ";
                map<int,int> & a_ddd = wordindex[map_it->first];    
                for (map <int,int>::iterator it_int=a_ddd.begin();it_int!=a_ddd.end();it_int++){
                outFile <<it_int->first<<" "<<it_int->second<<" ";
                }
                outFile<<endl;
                map_it++;
        }
        outFile.close();
    closedir(dir);
    //third=time(NULL);
    //cout<<(third-second)<<endl;
	}	
}

void createFileIndex(char* searchfolder,char* indexfolder){
    DIR *dir=NULL;
    struct dirent *ent=NULL;
    string folder=searchfolder;
    string index_folder=indexfolder;
    dir = opendir(indexfolder);
    //map
    if (dir==NULL)
    {
    system(("mkdir -p " +index_folder).c_str());
    //cout<<"make"<<endl;
    }
    //closedir(dir);
    dir =opendir(searchfolder); 
    map<int,string> fileIndex;
    int i=0;
    if (dir!=NULL){
        while((ent = readdir(dir)) != NULL){
            if(string(ent->d_name).compare(".") == 0 || string(ent->d_name).compare("..") == 0){
                continue;
            }
            else{
                char *fileName=ent->d_name;
                i++;
                fileIndex.insert(make_pair(i,fileName));
            }
        }
    }
    closedir(dir);
    dir =opendir(indexfolder);
    ofstream outFile(index_folder+"/"+"file_index.idx");
    map<int, string>::const_iterator map_it = fileIndex.begin();
        while(map_it!= fileIndex.end())
        {
                outFile << map_it->first<<" "<<map_it->second<<endl;
                map_it++;
        }
        outFile.close();
    closedir(dir);
}

map<int,map <string,int> > readwordIndex(set<string> keyword,char*indexfolder){
    DIR *dir=NULL;
    struct dirent *ent=NULL;
    dir=opendir(indexfolder);
    string word;
    string index_folder=indexfolder;
    map<int,map<string,int> > wordindex;
    string line;
    stringstream ss;
    int key;
    int occ;
    //int ti=0;
    //int d=0;
    if (dir!=NULL){
        while((ent = readdir(dir)) != NULL){
            ifstream infile;
            infile.open(index_folder+"/"+"words_index.idx");
            if(infile.is_open()){
                //cout<<ti<<endl;
                //ti++;
                while(getline(infile,line)){
                    //ss<<line;
                    stringstream input(line);
                    input>>word;
                    if (keyword.count(word)>0){
                        //cout<<line<<endl;
                        while(input>>key>>occ){
                            //cout<<key<<" "<<occ<<endl;
                            wordindex[key][word]=occ;
                        }
                    }
                }
            }
         infile.close();
         break;
        }
    }
    //cout<<wordindex.size()<<endl;
    /*map<int,map<string,int> >::const_iterator map_it=wordindex.begin();
    if(map_it!= wordindex.end()){
        cout << map_it->first<< endl;
    }*/
    return wordindex;
}


map<int,string> readFileIndex(char*indexfolder){
    DIR *dir=NULL;
    struct dirent *ent=NULL;
    dir=opendir(indexfolder);
    int key;
    string fileName;
    string index_folder=indexfolder;
    map<int,string>fileIndex;
    if (dir!=NULL){
        while((ent = readdir(dir)) != NULL){
            ifstream infile;
            infile.open(index_folder+"/"+"file_index.idx");
            //cout<<"1"<<endl;
            if(infile.is_open()){
                //cout<<"here"<<endl;
                while(infile>>key>>fileName){
                    fileIndex[key]=fileName;
                }
            }
         infile.close();
        }
    }
    return fileIndex;
}/*
    map <int, string>::iterator map_it;
    if(map_it!= fileIndex.end()) {
       // cout << fileIndex.find(6)->first<<" "<<fileIndex.find(6)->second<< endl;
    }


}*/

//get keyterms
set<string>searchkey(int n,set<string>s){
    //int keynumber=n;
    std::set<string> v;
    set<string>::iterator it;
    for(it=s.begin();it!=s.end();it++){
        string key=*it;
        transform(key.begin(),key.end(),key.begin(),::tolower);
        char word[1];
        strcpy(word,key.c_str());
        int end = stem(word, 0, strlen(word) - 1);
        word[end + 1] = 0;
        //cout<<word<<endl;
        v.insert(word);
    }
    return v;
}
// Sorter used to sort first according to occurrences, secord lexicagraphically
bool filesorter(std::pair<float, string> a, std::pair<float, string> b){
    if (a.first > b.first){
        return true;
    } else if (a.first < b.first) {
        return false;
    } else {
        int comparison = strcmp(a.second.c_str(), b.second.c_str());
        return comparison<0;
    }
}

// Print all the matching files
void printResults(std::vector<std::pair<float, string> > vector){
    std::sort(vector.begin(), vector.end(), filesorter);
    std::vector<std::pair<float, string> >::iterator it;
    if (vector.size()==0){
        cout<<endl;
    }
    else{
        for (it = vector.begin(); it != vector.end(); it++){
            std::cout <<(*it).second << std::endl;
        }
    }
}

std::vector<pair<float,string> > performSearch(set<string> keyword,int wordsnumber,map<int,map<string,int> > wordindex,map<int,string>fileIndex) {	
		unsigned int keynumber=wordsnumber;
        vector<pair<float,string> >result;
        float occ=0;
        //float occ_float=0
        //cout<<"here"<<endl;
        map<int,int>occurrences;
        for (map<int,map<string,int> >::iterator map_it=wordindex.begin();map_it!=wordindex.end();map_it++){
                //outFile << map_it->first<<" ";
                //cout<<"here"<<endl;
                map<string,int> & a_ddd = wordindex[map_it->first];
                if  (a_ddd.size()==keynumber){
                    //cout<<"there"<<endl;
                    for (map <string,int>::iterator it_int=a_ddd.begin();it_int!=a_ddd.end();it_int++){
                          occ+=it_int->second;
                    }
                occurrences[map_it->first]=occ;
                //cout<<occ<<endl;
                occ=0;
                }

        }
        for(map<int,int>::iterator it=occurrences.begin();it!=occurrences.end();it++){
            //cout<<it->first<<fileIndex[it->first]<<endl;
            ::pair<float,string>pair(occurrences[it->first],fileIndex[it->first]);
            result.push_back(pair);
        }
		return result;
}

int main(int argc,char **argv){
	string searchfolder=argv[1];
	string idxfolder=argv[2];
	int concept_num=0;
	//cout<<searchfolder<<endl;
	if (string(argv[3])=="-c"){
		stringstream(argv[4]) >> concept_num;
        cout<<concept_num<<endl;
		argv +=2;
		argc -=2;
	}
    createIndex(argv[1],argv[2]);
    createFileIndex(argv[1],argv[2]);

    map<int,string >fileIndex;
    map<int,map<string,int> >wordIndex;
    fileIndex=readFileIndex(argv[2]);

    set<string> keyword;
    //cout<<"1"<<endl;
    for(int qq=3;qq<argc;qq++){
        string key=string(argv[qq])
        transform(key.begin(),key.end(),key.begin(),::tolower);
        char word[1];
        strcpy(word,key.c_str());
        int end = stem(word, 0, strlen(word) - 1);
        word[end + 1] = 0;
        //cout<<word<<endl;
        keyword.insert(word);
    }
    //keyword=searchkey(argc-3,keyword);
    //cout<<keyword.size()<<endl;
    wordIndex=readwordIndex(keyword,argv[2]);

    vector<pair<float, string> > result;
    result=performSearch(keyword,argc-3,wordIndex,fileIndex);
    printResults(result);
}


