#ifndef __GRAPH_GACT_H__
#define __GRAPH_GACT_H__

#include <iostream>
#include <string.h>
#include "load.h"


typedef struct {
    int qrypos;
    int refpos;
}MatchPos;

typedef struct {
    MatchPos offset;  //tb stop offset
    MatchPos maxpos;
    //some debug data 
    MatchPos tbpos;
    int tbmaxscore;
    int tbBottomscore;// 
    int tbtopscore;
    std::string refTBStr;
    std::string qryTBStr;
    bool stopByScore;
} AlignTileRet;  //ret

typedef struct {
    MatchPos optmax;
    MatchPos startpos;  //final ali start pos
    //some debug data 
    std::vector<MatchPos> tbStartPos;
    std::vector<MatchPos> tbEndPos;

    std::vector<int> tbscore;
    std::vector<int> stopscore;

    int  totalscore;
    std::string refResStr;
    std::string qryResStr;
} GACTRet;  //result


//typedef MatchPos GACTRet;
AlignTileRet alignTile(Graph graph, int r_dp_start, int r_dp_stop, std::string qry, bool first, int overlap_size, int tile_size);
GACTRet gact( Graph graph, std::string qry,int tile,int overlap);
void csvdata(int *pScore ,int *pTbDir,int *pScoreK ,int *pMaxK , int matrixSize , std::string qry ,Graph graph ,std::string filep );
#endif // __GRAPH_GACT_H__
