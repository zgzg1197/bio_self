#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "load.h"

inline bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

Graph loadFile(char *file) {
    Graph g = {0, std::vector<Node>()};
   
    std::fstream fs;
    fs.open(file);

    int nodeCount;
    fs >> nodeCount;
    int i= 0;
    std::string str;
    while (i < nodeCount)
    {
        while(true){
          fs >> str;  
          if (!isDigit(str[0])||(i==nodeCount)) break;  
          }
  //      std::istringstream sstr(str.c_str());
       // char base;
      //  sstr >> base;
        Node temp;
        temp.base = str[0];
        g.nodes.push_back(temp);
        i++;  
    }
    fs.close();
    
    fs.open(file);
    fs >> nodeCount;
    int j=0 ;
    std::string str1;

    while (j <= nodeCount)
    {  
        while (true) {
            fs >> str1;
            if (!isDigit(str1[0])||(j>nodeCount))
                break;
            std::istringstream sstr(str1.c_str());
            int pos;
            sstr >> pos;
            g.nodes[j-1].adjList.push_back(pos-1);
        }
        j++;  
    }
    fs.close();
    g.nodes.erase(g.nodes.begin());
   /// g.nodes.erase(g.nodes.end());
    g.nodeCount=nodeCount-1;
    return g;
}



Reads loadReads(char *file) {
    Reads R = {0, std::vector<qrytype>()};
   
    std::fstream fs;
    fs.open(file);
    if(!fs.is_open())
    {
        std::cout<<"reads file is fail"<<std::endl;
    }
    
  
    std::string str = "";
    qrytype tmp;
    int lineCount = 0; //

    /*
        while(!fs.eof())
    {
        getline(fs,str);
        tmp.name = lineCount;       
        tmp.seq = str;
        R.qry.push_back(tmp);            
        lineCount ++;
    }
    R.qrynum = lineCount -1;  
    */
    
    while(!fs.eof())
    {
        getline(fs,str);
        if ((lineCount % 2)==0){
          tmp.name = lineCount/2 ;
        }
        else  {
          tmp.seq = str;
          R.qry.push_back(tmp);    
        }
        lineCount ++;
    }
    R.qrynum = lineCount/2;  
    std::cout<<"read reads file and count reads num is "<<R.qrynum<<"\n";
    fs.close();
    return R;
}
