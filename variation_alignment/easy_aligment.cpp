#include <iostream>;
#include <string>;

inline int64_t max (int64_t a, int_64 b){
    return a>b?a:b;
}

AlignTileRet alignTile(std::string ref ,std::string qry ,bool first ,int maxTBlen)
{
   int dp[ref.len()][qry.len()];
   int tb[ref.len()][qry.len()];
   int maxval=0;
   
   MatchPos maxpos={0,0};
  
//edit_param 
    int match=1;
    int mismatch=1;
    int ins=1;
    int del=1;
    
    AlignTileRet ret;
    ret.refTBstr =  "" ;
    ret.qryTBstr =  "" ;

    for (int i = 0; i <qry.len(); i++)
    {
       for (int j=0; j< ref.len(); j++)
       {
           if(int i ==0)
       }
    }  
  
}
     