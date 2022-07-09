// Owners - Vedant and Leland
// Edited by Leland and Vedant
//


#ifndef INC_22SU_SEARCH_ENGINE_SEARCHENGINE_H
#define INC_22SU_SEARCH_ENGINE_SEARCHENGINE_H

#include <iostream>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <set>

#include "DSAVLTree.h"
#include "DocumentProcessor.h"
#include "QueryProcessor.h"
#include "fileobj.h"

using std::unordered_map;
using std::unordered_set;
using std::set;

using namespace std;

class SearchEngine {
private:
    // private data members
    char stopFile[100] = "../stopWords.txt";
    std::unordered_set<string> stopWords;
    string path;
    DSAVLTree<string, unordered_map<int, double>> wordsIndex;
    DSAVLTree<string, unordered_set<int>> PersonsIndex;
    DSAVLTree<string, unordered_set<int>> OrgIndex;
    unordered_map<string, int> words;
    unordered_map<int, fileobj> files;
    DocumentProcessor scan;

    QueryProcessor queue;

public:
    SearchEngine() = default;

    void menu();  //gives user chance to clear trees, make trees, search a tree (query), main interface of program



};


#endif //INC_22SU_SEARCH_ENGINE_SEARCHENGINE_H
