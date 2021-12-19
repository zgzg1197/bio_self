#include <iostream>
#include <fstream>
#include <string>
#include <vector>


#include "load.h"
#include "graph_gact.h"
#include "printresult.h"

using namespace std;
void printresult(char *file , GACTRet result,int qryinfo)//std::string
{   
    ofstream fs;
    fs.open(file,ios::app);
    fs<<qryinfo<<endl;

    fs<<" ref   " <<result.refResStr<<endl;
    fs<<" qry   " <<result.qryResStr<<endl;
    fs<<" total score   " <<result.totalscore<<"    every tb bottom score  : ";
    for (auto &it : result.tbscore )     
       fs<<it<<" ";   
    fs<<  "    every tb top score  : "; 
    for (auto &it : result.stopscore )     
       fs<<it<<" ";      
    fs<<endl; 

    fs<<" ref  pos  " <<result.startpos.refpos<<","<<result.optmax.refpos<<endl;
    fs<<" qry  pos  " <<result.startpos.qrypos<<","<<result.optmax.qrypos<<endl;
    
  //  fs<<" --------------------------------  " <<endl;
    fs.close();
   
}
