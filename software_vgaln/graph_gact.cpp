# include <string>
# include <iostream>
# include <fstream>
# include "graph_gact.h"
# include "load.h"

/*
#define _debug_dpcomput_ 
#define _debug_tb_
#define _debug_input_
#define _gactresult_       
#define _swresult_       
*/
//#define  _tmpData_ 
using namespace  std;
inline int max(int a ,int b ){
    return a > b ? a : b;
} 

//first=1  
// when alignment matrix size real needed is smaller than tile matrix ,overlap_size is 0;
//AlignTileRet alignTile(Graph graph ,std::string qry ,bool first, int overlap_size, int tile_size)
AlignTileRet alignTile(Graph graph, int r_dp_start, int r_dp_stop, std::string qry, bool first, int overlap_size, int tile_size)
{  
    int match_score = 1;
    int mismatch_score = -1;
    int insertion_score =  -1;
    int deletion_score = -1; 
    // tracback direction 
    const int  zero = 0 ;
    const int  up = 1 ;  
    const int  left = 2 ; 
    const int  k_dir= 3 ; 
    const int  self = 4; 
    
    //aligment compute
    int graphLen = r_dp_stop - r_dp_start+1 ;
    int qryLen = qry.length();
    int score[qry.length()+1][graphLen];
    int tb_dir[qry.length()+1][graphLen];     
    int score_max_k[qry.length()+1][graphLen] ;
    int max_from_k[qry.length()+1][graphLen] ;    

    int *pScore = score[0];
    int *pTbDir = tb_dir[0];
    int *pScoreK = score_max_k[0];
    int *pMaxK = max_from_k[0];
    int matrixSize = ( qryLen + 1 ) * graphLen;
    //initial subgraph
    Graph subgraph;
    subgraph.nodeCount= graphLen;
    Node tempNode;
    for ( int i = 0 ; i < graphLen ;i++)
    {   
        tempNode = graph.nodes[r_dp_start+i];
        subgraph.nodes.push_back(tempNode) ;    
    }
    
    for (int i = 0; i < qry.length()+1; i++)
    {
        for(int j=0 ; j <graphLen ; j++)
        {
            score[i][j]=0;  
            tb_dir[i][j]=0; 
            score_max_k[i][j]=0 ;
            max_from_k[i][j]=0 ;  
        }
    }
    cout<<"initial matrix finish"<<endl;
    
    // tb
    int maxval =0 ;
    int max_pos_qry = 0 ;
    int max_pos_ref = 0 ;    
    MatchPos maxpos = {0 , 0}; 

    AlignTileRet ret;
    ret.qryTBStr = "" ;
    ret.refTBStr = "" ;
    ret.maxpos= {0 , 0};
    ret.offset={0,0};
    ret.tbpos={0,0};
    ret.stopByScore = 0 ; //stop =1 and stop traceback because of score[i][j] = 0;  
    //ret.offset traceback stop pos
    ret.tbBottomscore = 0;
    ret.tbmaxscore = 0;
    ret.tbtopscore = 0;

  /*
  #ifdef _debug_input_
        cout<<" ";
        
        for(int i=0 ; i<graph.nodeCount ; i++)
        cout<<"     "<<graph.nodes[i].base;
      //  cout<<endl;
 
  #endif
  */
    //dp compution
    for (int i=0 ;i < qry.length()+1;i++)
    {
  #ifdef _debug_dpcomput_
        
        if(i==0)std::cout<<endl<<" "<<" ";
        else std::cout<<endl<<qry[i-1]<<" ";
  #endif         
        for (int j = 0; j < graphLen ;j++)
        {
            if(i!=0)
            {    
            int edge_k = 0;
           // int max_k = (graph.nodes)[r_dp_start+j].adjList.size();
         //   cout<<max_k;
            if(j != 0)
            {
            int max_k = (graph.nodes)[r_dp_start+j].adjList.size();
        //        cout<<max_k;

            //for (int k = 0; (k < max_k) && ((graph.nodes)[r_dp_start+j].adjList[k] < j) + r_dp_start ; k++) 
            for (int k = 0; (k < max_k) && ((graph.nodes)[r_dp_start+j].adjList[k] < j+ r_dp_start) ; k++) 
            {

                 // edge_k = graph[j][k];
                edge_k = graph.nodes[r_dp_start+j].adjList[k];
                max_from_k[i][j] = edge_k;
            //     cout<<edge_k;
               if (score_max_k[i][j] < score[i][edge_k-r_dp_start] ) 
               {   
                   score_max_k[i][j] = score[i][edge_k-r_dp_start] ; 
             //      cout<<score_max_k[i][j];
                   max_from_k[i][j] = edge_k;
               }
               else 
               { 
                   score_max_k[i][j] = score_max_k[i][j];
                   max_from_k[i][j] = max_from_k[i][j];
               }
            }
            }
            int value_zero = 0;           
            int value_insertion_k = score_max_k[i][j] + insertion_score;
            int value_del = score[i-1][j] + deletion_score ;
            int value_match_k = score_max_k[i-1][j] + (qry[i-1] == (graph.nodes)[r_dp_start+j].base ? match_score : mismatch_score);    
            int value_match_curr = (qry[i-1] ==  (graph.nodes)[r_dp_start+j].base ) ? match_score : mismatch_score;   
            
            // record score[i][j] and direction
            if (value_match_k >= value_insertion_k  && value_match_k >= value_del  
            && value_match_k >= value_zero         && value_match_k > value_match_curr)
            {
               score[i][j] = value_match_k;
               tb_dir[i][j] = k_dir;
            }
            else if (value_zero >= value_match_k  && value_zero >= value_insertion_k   
                 && value_zero >= value_del      && value_zero >= value_match_curr)
            {
                score[i][j] = value_zero;
                tb_dir[i][j] = zero;
            }     
            else if (value_insertion_k >= value_match_k  && value_insertion_k >= value_del  
                 && value_insertion_k >= value_zero     && value_insertion_k >= value_match_curr )
            {
                score[i][j] = value_insertion_k;
                tb_dir[i][j] = left ;
            }
            else if (value_del >= value_match_k && value_del >= value_insertion_k 
                 && value_del >= value_zero    && value_del >= value_match_curr)
            {
                score[i][j] = value_del;
                tb_dir[i][j] = up;
            }
            else
            /* (value_match_curr >= value_match_k       &&  value_match_curr >= value_insertion_k
               && value_match_curr >= value_del     && value_match_k >= value_zero)*/
            {
                score[i][j] = value_match_curr;
                tb_dir[i][j] = self;
            }

            } //endif   
            if (score[i][j] >= maxval)
            {
                maxval = score[i][j];
                max_pos_qry = i  ;
                max_pos_ref = j ;   //record max position
                // maxpos = {max_pos_qry , max_pos_ref};
            }
  #ifdef _debug_dpcomput_
        if(i==0)std::cout<<graph.nodes[j+r_dp_start].base<<"  "<<score[i][j] <<"("<<tb_dir[i][j]<<")"<<" "; 
        else   std::cout<<" "                 <<"  "<<score[i][j] <<"("<<tb_dir[i][j]<<")"<<" ";    
          
  #endif
        }   

    }
    cout<<"dp_compute finish"<<endl;

  #ifdef _debug_dpcomput_
    /*
    for (int i=0 ;i < qry.length()+1;i++) 
        if(i==0)std::cout<<endl<<" "<<" ";
        else std::cout<<endl<<qry[i-1]<<" ";
    */
    //score
    for (int i=0 ;i < qry.length()+1;i++)
    {
        for (int j = 0; j < graphLen ;j++)
        {   
        if(i==0)std::cout<<graph.nodes[j+r_dp_start].base<<score[i][j]<<" "; 
        else   std::cout<<" "                 <<score[i][j]<<" ";    
        }   
        cout<<endl;
    }     
    //tb_dir
    for (int i=0 ;i < qry.length()+1;i++)
    {
        for (int j = 0; j < graphLen ;j++)
        {   
        if(i==0)std::cout<<graph.nodes[j].base<<tb_dir[i][j]<<" "; 
        else   std::cout<<" "                 <<tb_dir[i][j]<<" ";    
        }   
        cout<<endl;
    }           
  #endif
  
    ret.maxpos = {max_pos_qry - 1 , max_pos_ref };  //give the offset
    
    int i,j;
  // traceback initial and   record somedata  
    if(first)
    {
        ret.tbpos = {max_pos_qry - 1, max_pos_ref };  //give the offsetret
        i = max_pos_qry;
        j = max_pos_ref;
        cout<<"max pos i = "<<i <<" maxval = "<<maxval<< qry <<endl;
        ret.tbmaxscore= maxval;  
    } 
    else{
        cout<<"****************************"<<endl;    
        ret.tbpos = {qryLen - 1, graphLen - 1 };  //give the offset   
        i = qry.length() ; //tb maxtix pos
        j = graphLen-1;
        ret.tbBottomscore = score[i][j];  

        cout<<"i offset "<< i <<" the bottom score = "<<score[i][j]<<"   qry[i-1] = "<<qry[i-1]<<" qry is  " << qry <<endl;
        
      //  ret.tbpos = {max_pos_qry - 1 , max_pos_ref };  //give the offsetret
      //  i = max_pos_qry;
      //  j = max_pos_ref;
    }  



    // bool ret.stopByScore = 0 ; //stop =1 and stop traceback because of score[i][j] = 0;  
    bool  stopByPOS = 0 ;  
    int  tbstop_q_pos = max_pos_qry;
    int  tbstop_r_pos = max_pos_ref;

   // ret.refTBStr = (graph.nodes)[j].base + ret.refTBStr; 
   // ret.qryTBStr = qry[i-1] + ret.qryTBStr;

    while( ret.stopByScore == 0 && i >= overlap_size && j >= overlap_size )
    {  
        //tb trace 
      {
        if (tb_dir[i][j] == k_dir )
        {
           // cout<<"max_from_k[i][j]"<<max_from_k[i][j]<<endl;
           // cout<<"graph[j]"<<(graph.nodes)[j+r_dp_start].base<<endl;
            ret.refTBStr = (graph.nodes)[j+r_dp_start].base + ret.refTBStr; 
            ret.qryTBStr = qry[i-1] + ret.qryTBStr;          
            j = max_from_k[i][j]-r_dp_start; // ref need to jump to hop_vertex

            i-- ;// qry--      
        }
        else if(tb_dir[i][j] == up )
        {    
            ret.refTBStr = '-' + ret.refTBStr; 
            ret.qryTBStr = qry[i-1] + ret.qryTBStr;
          //  cout<<ret.refTBStr<<endl;
            i-- ;// qry-- 
        } 
        else if(tb_dir[i][j] == left )
        {
            ret.refTBStr = (graph.nodes)[j+r_dp_start].base + ret.refTBStr; 
            ret.qryTBStr = '-' + ret.qryTBStr;
            j = max_from_k[i][j]-r_dp_start; // ref 需要跳转到hop vertex
        } 
        else if(tb_dir[i][j] == self)
        {
            ret.refTBStr = (graph.nodes)[j+r_dp_start].base + ret.refTBStr; 
            ret.qryTBStr = qry[i-1] + ret.qryTBStr;          
            ret.stopByScore = 1 ;
            
        }
        else ret.stopByScore = 1 ;
      }
       // tbstop_q_pos = i;//-1 for qury 0 is empty
       // tbstop_r_pos = j;
        stopByPOS = (i < overlap_size || j < overlap_size )? 1:0; 
        if (stopByPOS || ret.stopByScore) 
        {
            break;
        }
        
  #ifdef _debug_tb_
        cout<<"ret.refTBStr"<<ret.refTBStr<<endl;
        cout<<"ret.qryTBStr"<<ret.qryTBStr<<endl;

  #endif
    }
    cout<<"dp_tb finish"<<endl;
    cout<<"dp_tb finish stop at (i,j)" <<i<<","<< j << endl;
    ret.offset={i , j};  
    cout<<"stop qry[i-1] = "<<qry[i-1] <<endl;
    ret.tbtopscore=score[i][j];  
    
    string filedata = "dpData.txt";
    csvdata( pScore , pTbDir, pScoreK , pMaxK ,  matrixSize , qry , subgraph , filedata);
    return ret ; 
  // we also need to return maxpos and stopByScore
}


///////////////////////////////////////////////
/*
gact
*/////////////////////////////////////////////
GACTRet gact( Graph graph, std::string qry ,int tile ,int overlap)
{  
 //   std::string qry = qry_zero.substr(0,qry_zero.length());

  #ifdef _debug_input_
       // for(int i=0 ; i<=graph.nodeCount ; i++)
       // std::cout<<graph.nodes[i].base;
        cout <<"qry is"<< endl ;
        cout<<qry<<endl;
  #endif

    int tile_size = tile;
    int overlap_size = overlap;
    
    int r_in_len=graph.nodeCount;
    int q_in_len=qry.length();
    
    bool first = true ;
    GACTRet result; 
    //MatchPos optPos;
    result.qryResStr="";
    result.refResStr="";
    result.optmax={0,0};
    result.startpos={0,0};
    result.totalscore = 0;
    MatchPos tmpTbStartPos={0,0};
    MatchPos tmpTbEndPos={0,0};


  
    int r_dp_start = r_in_len - 1 ;
    int q_dp_start = q_in_len - 1 ;

    int r_dp_stop = 0 ;
    int q_dp_stop = 0  ;
    int sublen=qry.length();

    //int r_dp_stop = r_dp_start  ;
    //int q_dp_stop = q_dp_start  ;
  
    do 
    {
        if (r_dp_start > tile_size && q_dp_start > tile_size)
        {
            r_dp_stop = r_dp_start  ;
            q_dp_stop = q_dp_start  ;
            r_dp_start -=tile_size-1;
            q_dp_start -=tile_size-1;
            sublen = tile_size;
            cout<<"1 :q_dp_start = "<<q_dp_start<<endl;//////////// delete
        }
        else if(r_dp_start > tile_size && q_dp_start <= tile_size)
        {
            r_dp_stop = r_dp_start  ;
            q_dp_stop = q_dp_start  ;
            r_dp_start -=tile_size-1;
            q_dp_start = 0;
            sublen = q_dp_stop+1;
            overlap_size=0;
           cout<<"2:q_dp_start = "<<q_dp_start<<endl;//////////// delete

        }
        else
        {   
            r_dp_stop = r_dp_start  ;
            q_dp_stop = q_dp_start  ;
            r_dp_start = 0;
            q_dp_start = 0;
            sublen=q_dp_stop+1 ;
            overlap_size=0;
            cout<<"3:q_dp_start = "<<q_dp_start<<endl;//////////// delete

        }    
       // std::string subGraph  = graph.substr(r_dp_start,r_dp_stop);
       std::string subQry    = qry.substr(q_dp_start,sublen); 
       cout<<"subQry len is "<<subQry.length()<<endl;   
       //AlignTileRet ret = alignTile(subGraph ,subQry ,first, overlap_size, tile_size);
       AlignTileRet ret = alignTile(graph,r_dp_start,r_dp_stop ,subQry ,first, overlap_size, tile_size);   
  #ifdef _swresult_       
        cout<<"ret.refTBStr"<<ret.refTBStr<<endl;
        cout<<"ret.qryTBStr"<<ret.qryTBStr<<endl;
  #endif

        if(first)
        {
           result.optmax = {q_dp_start + ret.maxpos.qrypos,r_dp_start+ ret.maxpos.refpos};  // maxpos.qry  get by alignTile's dp compute part
           first = false ;
           cout<<"tile first"<<endl;
           result.tbscore.push_back(ret.tbmaxscore);

        }
        else 
            result.tbscore.push_back(ret.tbBottomscore);
        result.stopscore.push_back(ret.tbtopscore);
        result.qryResStr=ret.qryTBStr + result.qryResStr ;
        result.refResStr=ret.refTBStr + result.refResStr ;
        tmpTbStartPos={ret.tbpos.qrypos+q_dp_start ,ret.tbpos.refpos+r_dp_start};
        result.tbStartPos.push_back(tmpTbStartPos);      
        r_dp_start =  r_dp_start + ret.offset.refpos;
        q_dp_start =  q_dp_start + ret.offset.qrypos;
        tmpTbEndPos={q_dp_start,r_dp_start};
        result.tbEndPos.push_back(tmpTbEndPos);      
        for (auto &it : result.tbscore )
        {           
           // result.totalscore += it; 
        }
      //  cout<<" result.totalscore "<<endl;
     //   for (auto &it : result.stopscore )
      //       result.totalscore = result.totalscore - it;
        


  #ifdef _gactresult_      
      cout<<"result.qryResStr"<<result.qryResStr<<endl;
      cout<<"result.refResStr"<<result.refResStr<<endl;
      cout<<"(tmpTbEndPos_ref , tmpTbStartPos_ref) = "<<"("<<tmpTbEndPos.refpos<<","<<tmpTbStartPos.refpos<<")"<<"  score ="<<ret.tbmaxscore<<endl;
      cout<<"(tmpTbEndPos_qry , tmpTbStartPos_qry) = "<<"("<<tmpTbEndPos.qrypos<<","<<tmpTbStartPos.qrypos<<")"<<endl;
       
  #endif 

  
        if (ret.stopByScore){ 
           cout<<"stop by score :: alignment is  over!!!!!!!!!!!!!!!!" << endl;
           break; 
        } //跳出整个大循环 所有操作结束      
    }while(r_dp_start>0 && q_dp_start>0);
    result.startpos={q_dp_start-1,r_dp_start};
    return result;
}  

void csvdata(int *pScore ,int *pTbDir,int *pScoreK ,int *pMaxK , int matrixSize , string qry ,Graph graph , string fpath)
{ 
#ifdef  _tmpData_ 
   // string fpath = "dpData.txt" ;
    ofstream  fs ;
    fs.open(fpath,ios::app);
    cout<< "--------"<<endl;
    //print dp all matrix 
    {
    fs << "print dp all matrix"<<endl;
    //set  first row
    fs <<" ";
    for (int j = 0 ;j < graph.nodeCount ;j ++)
    {
        fs<<","<<graph.nodes[j].base<<","<<"dir";       
    }
    fs << endl;

    //
    for (int i = 0 ; i < qry.length()+1 ; i++ )
    {   
        if(i==0) fs<<" "<<",";
        else 
            fs <<qry[i-1];
        
        for (int j = 0 ;j < graph.nodeCount ;j ++)
        {
            if(i==0)
                fs<<"0"<<","<<"0"<<",";
            else  {
                int addrOffset = ( i - 1 ) * qry.length() +j;    
                fs << "," <<*(pScore+addrOffset)<<","<<*(pTbDir + addrOffset);
            }
        }
        fs << endl;
    }
       
    }
   
   // print dp score matrix  
    {
    fs << "print dp score"<<endl;
    //set  first row
    fs <<"score";
    for (int j = 0 ;j < graph.nodeCount   ;j ++)
    {
        fs<<","<<graph.nodes[j].base;       
    }
    fs << endl;

    //
    for (int i = 0 ; i < qry.length()+1 ; i++ )
    {   
        if(i==0) fs<<" "<<",";
        else 
            fs <<qry[i-1];
        
        for (int j = 0 ;j < graph.nodeCount ;j ++)
        {
            if(i==0)
                fs<<"0"<<",";
            else  {
                int addrOffset = ( i - 1 ) * qry.length() +j;    
                fs << "," <<*(pScore+addrOffset);
            }
        }
        fs << endl;
    }
    }

    // print dp tbdir matrix  
    {
    fs << "print dp tbdir"<<endl;
    //set  first row
    fs <<"tbdir";
    for (int j = 0 ;j < graph.nodeCount  ; j ++)
    {
        fs<<","<<graph.nodes[j].base;       
    }
    fs << endl;

    //
    for (int i = 0 ; i < qry.length()+1 ; i++ )
    {   
        if(i==0)fs<<" "<<",";
        else 
            fs <<qry[i-1];
        
        for (int j = 0 ;j < graph.nodeCount ;j ++)
        {
            if(i==0)
                fs<<"0"<<",";
            else  {
                int addrOffset = ( i - 1 ) * qry.length() +j;    
                fs << "," <<*(pTbDir+addrOffset);
            }
        }
        fs << endl;
    }
    }
     
    // print dp score_maxFromK matrix  
    {
    fs << "print dp score_maxFromK"<<endl;
    //set  first row
    fs <<"score_maxFromK";
    for (int j = 0 ;j < graph.nodeCount ;j ++)
    {
        fs<<","<<graph.nodes[j].base;       
    }
    fs << endl;

    //
    for (int i = 0 ; i < qry.length()+1 ; i++ )
    {   
        if(i==0)fs<<" "<<",";
        else 
            fs <<qry[i-1];
        
        for (int j = 0 ;j < graph.nodeCount ;j ++)
        {
            if(i==0)
                fs<<"0"<<",";
            else  {
                int addrOffset = ( i - 1 ) * qry.length() +j;    
                fs << "," <<*(pScoreK + addrOffset);
            }
        }
        fs << endl;
   }
   }
  
    // print dp dir_maxFromK matrix
    {   
    fs << "print dp dir_maxFromK"<<endl;
    //set  first row
    fs <<"dir_maxFromK";
    for (int j = 0 ;j < graph.nodeCount   ;j ++)
    {
        fs<<","<<graph.nodes[j].base;       
    }
    fs << endl;

    //
    for (int i = 0 ; i < qry.length()+1 ; i++ )
    {   
        if(i==0) fs <<" "<<",";
        else 
            fs <<qry[i-1];
        
        for (int j = 0 ;j < graph.nodeCount ;j ++)
        {
            if(i==0)
                fs<<"0"<<",";
            else  {
                int addrOffset = ( i - 1 ) * qry.length() +j;    
                fs << "," <<*(pScoreK + addrOffset);
            }
        }
        fs << endl;
    }   
    }

   fs.close();
  #endif
}
