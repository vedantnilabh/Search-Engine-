/**
 * Summer 2022 Search Engine Template
 * includes some example code related to 2341 final project for summer 2022
 */
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <unordered_map>

//RapidJSON headers we need for our parsing.
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"
#include "porter2_stemmer.h"
#include "DSAVLTree.h"
#include "SearchEngine.h"

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
using std::getline;

int main() {

    SearchEngine drive;
    drive.menu();




}