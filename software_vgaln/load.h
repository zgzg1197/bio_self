#ifndef __LOAD_H__
#define __LOAD_H__

#include <vector>
//read vg as graph struct  
typedef struct {
    char base;
    std::vector<int> adjList;
} Node;

typedef struct {
    int nodeCount;
    std::vector<Node> nodes;
} Graph;    


//read reads as read
typedef struct {
   // std::string name;
    int name;
    std::string seq;
}qrytype;

typedef struct {
    int qrynum;
    std::vector<qrytype> qry;
} Reads; 



Graph loadFile(char *file);
Reads loadReads(char *file);



typedef struct {
    int rid;
    std::string read;
} reads;    


#endif // __LOAD_H__