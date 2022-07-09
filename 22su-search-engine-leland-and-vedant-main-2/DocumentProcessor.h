//
// Owner - Vedant Nilabh
// Edited by Leland and Vedant
//

#ifndef INC_22SU_SEARCH_ENGINE_DOCUMENTPROCESSOR_H
#define INC_22SU_SEARCH_ENGINE_DOCUMENTPROCESSOR_H

//more includes
#include <iostream>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <unordered_set>
#include <unordered_map>


//include's from the project
#include "DSAVLTree.h"
#include "fileobj.h"

//RapidJSON headers we need for our parsing.
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"
#include "porter2_stemmer.h"

using namespace rapidjson;
using std::cout;
using std::pair;
using std::set;
using std::endl;
using std::cin;
using std::ifstream;
using std::setw;
using std::left;
using std::string;
using std::stringstream;
using std::vector;
using std::unordered_set;
using std::unordered_map;

class DocumentProcessor {
public:
    DocumentProcessor() = default;

    void generateStopWords(const char* stopFile, std::unordered_set<string> & stopWords); // populates set with stopwords
    void indexFiles(const char* path, const std::unordered_set<string>& stopWords, DSAVLTree<string, unordered_map<int, double>> & wordIndex, unordered_map<int, fileobj>& fileIndex, DSAVLTree<string, unordered_set<int>> & PersonsIndex, DSAVLTree<string, unordered_set<int>> & OrgIndex, unordered_map<string, int>& words); // creates all the indexes
    void CalcIDF(DSAVLTree<string, unordered_map<int, double>> & wordIndex, unordered_map<string, int>& words, const int& numFiles); // calculates tfidf and stores it in place of frequencies
};


#endif //INC_22SU_SEARCH_ENGINE_DOCUMENTPROCESSOR_H
