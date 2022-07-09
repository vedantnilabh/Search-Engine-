// Owner - Leland Winston
// Edited by Leland and Vedant
//

#include "QueryProcessor.h"

void QueryProcessor::processQuery(const string& query, DSAVLTree<string, unordered_map<int,double>>& wordsT, DSAVLTree<string, unordered_set<int>>& personsT, DSAVLTree<string, unordered_set<int>>& orgsT, unordered_map<int, fileobj>& filesM) {
    std::chrono::time_point<std::chrono::system_clock> start, end; // to calculate query time
    start = std::chrono::system_clock::now();
    std::stringstream stream(query);
    string buffer;
    std::vector<string> terms;
    std::vector<string> keywords = {"PERSON", "ORG", "NOT", "OR", "AND"};

    //pushing items in query into a vector
    //stemming and trimming words that are not keywords
    while (getline(stream, buffer, ' ')) {
        if(std::find(keywords.begin(), keywords.end(), buffer) == keywords.end()){
            Porter2Stemmer::trim(buffer);
            Porter2Stemmer::stem(buffer);
        }
        terms.push_back(buffer);
//        cout << buffer << " ";
    }
    cout << endl;

    //if first index is not a AND or OR
    if(terms[0] != "AND" && terms[0] != "OR") {
        map<int, double> tempRes;
        for(int i = 0; i < terms.size(); i++) {
            if(std::find(keywords.begin(), keywords.end(), terms[i]) == keywords.end()) {
                if(wordsT.find(terms[i])) {
                    getFiles(tempRes, wordsT[terms[i]]);
                }
            }
            if(terms[i] == "NOT") {
                if(wordsT.find(terms[i + 1])) {
                    setDifference(tempRes, wordsT[terms[i + 1]]);
                }
                i++; // so we skip the next index
            }
            if(terms[i] == "PERSONS") {
                if(personsT.find(terms[i + 1])) {
                    SetIntersection(tempRes, personsT[terms[i + 1]]);
                }
                i++;
            }
            if(terms[i] == "ORG") {
                if(orgsT.find(terms[i + 1])) {
                    SetIntersection(tempRes, orgsT[terms[i + 1]]);
                }
                i++;
            }
        }
        // printing if tempRes is not empty
        if (tempRes.empty()) {
            cout << "Query is empty, going back to main menu" << endl;
            return;
        } else {
            set<pair<double, int>> printThis;
            ToSet(tempRes, printThis);
            end = std::chrono::system_clock::now();
            std::chrono::duration<double> queryTime = end - start;
            cout << "Query time: " << queryTime.count() << " SEC" << endl;
            PrintResults(printThis, filesM);
        }
    }

    if(terms[0] == "AND") {
        map<int, double> tempRes;
        if(wordsT.find(terms[1])) {
            getFiles(tempRes, wordsT[terms[1]]);
        } else {
            cout << "Query is empty, going back to main menu" << endl;
            return;
        }
        for(int i = 2; i < terms.size(); i++) {
            if(std::find(keywords.begin(), keywords.end(), terms[i]) == keywords.end()) {
                if(wordsT.find(terms[i])) {
                    SetIntersection(tempRes, wordsT[terms[i]]);
                } else {
                    cout << "Query is empty, going back to main menu" << endl;
                    return;
                }
            }
            if(terms[i] == "NOT") {
                if(wordsT.find(terms[i + 1])) {
                    setDifference(tempRes, wordsT[terms[i + 1]]);
                }
                i++;
            }
            if(terms[i] == "PERSONS") {
                if(personsT.find(terms[i + 1])) {
                    SetIntersection(tempRes, personsT[terms[i + 1]]);
                }
                i++;
            }
            if(terms[i] == "ORG") {
                if(orgsT.find(terms[i + 1])) {
                    SetIntersection(tempRes, orgsT[terms[i + 1]]);
                }
                i++;
            }
        }
        //printing if tempRes is not empty
        if (tempRes.empty()) {
            cout << "Query is empty, going back to main menu" << endl;
            return;
        } else {
            set<pair<double, int>> printThis;
            ToSet(tempRes, printThis);
            end = std::chrono::system_clock::now();
            std::chrono::duration<double> queryTime = end - start;
            cout << "Query time: " << queryTime.count() << " SEC" << endl;
            PrintResults(printThis, filesM);
        }
    }

    if(terms[0] == "OR") {
        map<int, double> tempRes;
        if(wordsT.find(terms[1])) {
            getFiles(tempRes, wordsT[terms[1]]);
        }
        for(int i = 2; i < terms.size(); i++) {
            if(std::find(keywords.begin(), keywords.end(), terms[i]) == keywords.end()) {
                if(wordsT.find(terms[i])) {
                    setUnion(tempRes, wordsT[terms[i]]);
                }
            }
            if(terms[i] == "NOT") {
                if(wordsT.find(terms[i + 1])) {
                    setDifference(tempRes, wordsT[terms[i + 1]]);
                }
                i++;
            }
            if(terms[i] == "PERSONS") {
                if(personsT.find(terms[i + 1])) {
                    SetIntersection(tempRes, personsT[terms[i + 1]]);
                }
                i++;
            }
            if(terms[i] == "ORG") {
                if(orgsT.find(terms[i + 1])) {
                    SetIntersection(tempRes, orgsT[terms[i + 1]]);
                }
                i++;
            }
        }
        //do whatever you need to put tempRes in order to print it in reverse order
        if (tempRes.empty()) {
            cout << "Query is empty, going back to main menu" << endl;
            return;
        } else {
            set<pair<double, int>> printThis;
            ToSet(tempRes, printThis);
            end = std::chrono::system_clock::now();
            std::chrono::duration<double> queryTime = end - start;
            cout << "Query time: " << queryTime.count() << " SEC" << endl;
            PrintResults(printThis, filesM);
        }
    }
}
// puts unordered map in a set of pairs of double, int
void QueryProcessor::getFiles(set<pair<double, int>> & results, unordered_map<int, double> & wordVals) {
    for(auto& vals: wordVals){
        results.insert({vals.second, vals.first});
    }
}
// gets the text from a file based on path
void QueryProcessor::getText(const string& path) {
    ifstream input(path);
    if(!input.is_open())
        std::cerr << "cannot open file" << endl;

    //Create a RapidJSON IStreamWrapper using the file input stream above.
    IStreamWrapper isw(input);

    //Create a RapidJSON Document object and use it to parse the IStreamWrapper object above.
    Document d;
    d.ParseStream(isw);
    auto text = d["text"].GetString();
    cout << text << endl;
    input.close();

}
// prints the results from the set of pairs - sorted by tf-idf
void QueryProcessor::PrintResults(set<pair<double, int>> &results, unordered_map<int, fileobj>& files) {
    int index = 1;
    for (auto res = results.crbegin(); res != results.crend(); ++res) {
        cout << "Relevancy: " << res->first << endl;
        cout << index << ". " << endl << "Title: " << files[res->second].title << endl << "Publisher: "
             << files[res->second].publisher << endl << "Publication Date: "
             << files[res->second].pubDate;
        cout << endl << endl;
        index++;
        if (index > 15) {
            break;
        }
    }
    bool x = true;
    while (x) {
        cout
                << "Enter the corresponding number to access the associated text or enter an integer > 15 or < 1 to exit"
                << endl;
        int num;
        cin >> num;
        if (num > 0 && num <= 15 && num <= results.size()) {
            int index = 1;
            for (auto res = results.crbegin(); res != results.crend(); ++res) {
                if (index == num) {
                    getText(files[res->second].docID);
                    cout << endl;
                }
                index++;
            }
        } else {
            cout << "Invalid input, going to main menu" << endl;
            x = false;
        }
    }

}
// puts the files and tf-idf's into a map
void QueryProcessor::getFiles(map<int, double>& results, unordered_map<int, double>& wordVals) {
    for(auto& vals: wordVals){
        results.insert({vals.first, vals.second});
    }
}
// set difference of map and unordered map
void QueryProcessor::setDifference(map<int, double> &results, unordered_map<int, double> &wordVals) {
    //map<int, double> temp;
    vector<int> temp;
    for(auto x : results) {
        for(auto y : wordVals){
            if(x.first == y.first) {
                temp.push_back(x.first);
            }
        }
    }
    for(int i = 0; i < temp.size(); i++) {
        if(!results.empty()) {
            results.erase(temp[i]);
        } else{
            return;
        }
    }
}
// set intersection of map and unordered map
void QueryProcessor::SetIntersection(map<int, double> & results, unordered_map<int, double> & wordVals) {
    if(results.empty()){
        return;
    }
    map<int, double> temp;
    for(auto x : results) {
        for(auto y : wordVals) {
            if(x.first == y.first) {
                temp[x.first] = x.second + y.second;
                break;
            }
        }
    }
    results = temp;
}
// set intersection of map and unordered set of ints - filekeys
void QueryProcessor::SetIntersection(map<int, double> &results, unordered_set<int> & wordVals) {
    if(results.empty()){
        return;
    }
    map<int, double> temp;
    for(auto x : results) {
        for(auto y : wordVals) {
            if(x.first == y) {
                temp.insert({x.first, x.second});
                break;
            }
        }
    }
    results = temp;
}
// set union - between map containing found results and unordered map for other word
void QueryProcessor::setUnion(map<int, double> &results, unordered_map<int, double> &wordVals) {
    if (results.empty()) {
        for(auto z : wordVals) {
            results.insert({z.first, z.second});
            return;
        }
    }
    for (auto x : results) {
        for(auto y : wordVals) {
            if(x.first == y.first) {
                results[x.first] = x.second + y.second;
            } else {
                results.insert({y.first, y.second});
            }
        }
    }
}
// converts map to set of pairs
void QueryProcessor::ToSet(map<int, double>& results, set<pair<double, int>>& ToPrint) {
    for(auto X: results){
        ToPrint.insert({X.second, X.first});
    }
}



