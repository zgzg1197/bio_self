#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "load.h"
#include "graph_gact.h"
#include "printresult.h"
//#define _result_
//#define _otherfile_
//#define _readsdata_
//#define _vgstr_
#define _gactresult_



using namespace std;

int main(int argc, char **argv) {
    Graph g;
    Reads R;
    
    std::string qry = "AACATAACAAGACCCGTGTCTAACCCCACCCCAAATTATCTTTTTTATTCCCTATATTGTCCAATTTGGGGGCAATGAGCTTATAGCAATTTTGTAAGAA";
    //std::string qryinfo= "test data";
    int qryinfo = 0;
    if (argc > 2)
    {
        R = loadReads(argv[2]);
        g = loadFile(argv[1]);
    }
    else if (argc > 1 && argc <= 2 )
        g = loadFile(argv[1]);
    else {
        g = loadFile("/home/zengg/mycode/aln_graph/data/vgref.txt");
        R = loadReads("/home/zengg/mycode/aln_graph/data/6m_5K/x_2000.sim");
        std::cout <<"@ \n";
    }

    std::string seq;
    int seqnum;
    std::vector<Node> nodes = g.nodes;
    cout<<"alignment result is "<<endl;
    
    
    GACTRet result ; 
    int tile = 512;
    int overlap= 80;
    ofstream fs;
    if(argc > 3)
        fs.open(argv[3],ios::out);
    else
        fs.open("result1.txt",ios::out);
    fs.close();  
    
    string filedata;
    filedata = "dpData.txt";   
    fs.open(filedata,ios::out);
    fs.close();

    if(argc > 3)
            fs.open(argv[3]);
        else
            fs.open("result1.txt"); 
    fs<< "tile = "<<tile <<"  overlap = "<<overlap<<endl;
    fs.close();
    
    for(int i =0 ;i < R.qrynum ;i++){
   // for(int i =0 ;i < 1  ;i++){

        qry =  R.qry[i].seq ; 
        qryinfo = R.qry[i].name + 1; 
        result= gact(g, qry ,tile ,overlap);
        //cout<<"+++++++++++++++++++++++++++++++++++"<<endl;

  #ifdef _gactresult_      
       cout<<"result.qryResStr"<<result.qryResStr<<endl;
       cout<<"result.refResStr"<<result.refResStr<<endl;
  #endif 

        cout<<"align seq id "<<i+1<<endl;

        if(argc > 3)
            printresult(argv[3], result, qryinfo);
        else
            printresult("result1.txt", result,qryinfo);
    }

#ifdef _readsdata_
    std::cout<<"there are "<<R.qrynum<<"reads"<<std::endl;
    for(int i = 0; i < R.qrynum ; i++){
        std::cout<<" "<<R.qry[i].name<<std::endl;
        std::cout<<" "<<R.qry[i].seq<<std::endl;//<<R.qry[i].name
    }
#endif

#ifdef _vgstr_
    std::string refvg="$";
    for(int i =0 ;i < g.nodeCount ;i++){
       refvg=refvg+g.nodes[i].base;
      
    }
    std::ofstream fs;
    fs.open("refvg.fa");
    fs<<refvg<<endl;
    fs.close();

#endif
   
# ifdef _vgdata_
   for(auto &it : nodes)
        {
            std::cout<<it.base<<" " ;
            for (auto &it1 : it.adjList) {
            std::cout << it1 << " ";}
            std::cout <<std::endl;  
        }
#endif 
   


#ifdef _result_
    //std::vector<Node> nodes = g.nodes;
    cout<<"alignment result is "<<endl;
    cout<<" ref   " <<result.refResStr<<endl;
    cout<<" qry   " <<result.qryResStr<<endl;
    cout<<" ref  pos  " <<result.startpos.refpos<<","<<result.optmax.refpos<<endl;
    cout<<" qry  pos  " <<result.startpos.qrypos<<","<<result.optmax.qrypos<<endl;

#endif

#ifdef _other_
    std::string newstr="$";
    int len=3;
    cout<<qry.length()<<endl;
    cout<<endl;
    for(int i = 0; i < qry.length();i+=len)
    {
        newstr=newstr+qry.substr(i,i+3);
        cout<<i<<":"<<newstr<<endl;
    }
#endif

#ifdef _otherfile_

    std::ofstream fs;
    fs.open("reads2.fa",ios::app);

    for(int i =0 ;i < R.qrynum ;i++){
        fs<<">"<<R.qry[i].name<<endl;
        fs<<R.qry[i].seq<<endl;
    }
   fs.close();
#endif

 //   system("pause");
    return 0;
}