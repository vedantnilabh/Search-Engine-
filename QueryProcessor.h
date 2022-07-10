// Owner - Leland Winston
// Edited by Leland and Vedant
//

#ifndef INC_22SU_SEARCH_ENGINE_QUERYPROCESSOR_H
#define INC_22SU_SEARCH_ENGINE_QUERYPROCESSOR_H
#include <chrono>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "DSAVLTree.h"
#include "fileobj.h"
#include <sstream>
#include <fstream>
#include <map>
#include <set>
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"
#include "porter2_stemmer.h"
using std::set;
using std::map;
using std::string;
using std::unordered_set;
using std::unordered_map;
using namespace rapidjson;

class QueryProcessor {
public:
    // function that processes queries, "inner interface" for querying
    void processQuery(const string&, DSAVLTree<string, unordered_map<int, double>>&, DSAVLTree<string, unordered_set<int>>&, DSAVLTree<string, unordered_set<int>>&, unordered_map<int, fileobj>&);
private:
    // private helpers
    void getFiles(map<int, double>& results, unordered_map<int, double>& wordVals);
    void getFiles(set<pair<double, int>>& results, unordered_map<int, double>& wordVals);
    void getText(const string& );
    void ToSet(map<int, double>& results, set<pair<double, int>>& ToPrint);
    void PrintResults(set<pair<double, int>>& results, unordered_map<int, fileobj>& filesM);

    // set operators
    void setDifference(map<int, double> & results, unordered_map<int, double> & wordVals);
    void SetIntersection(map<int, double> & results, unordered_map<int, double> & wordVals);
    void SetIntersection(map<int, double> & results, unordered_set<int> & wordVals);
    void setUnion(map<int, double> & results, unordered_map<int, double> & wordVals);

};


#endif //INC_22SU_SEARCH_ENGINE_QUERYPROCESSOR_H
