// Owners - Vedant and Leland
// Edited by Leland and Vedant
//

#include "math.h"
#include "SearchEngine.h"

void SearchEngine::menu() {
    cout << "Welcome to Leland and Vedant's def not ratchet search engine!" << endl;

    bool x = true;
    while (x) {
        cout << "Please select a menu option" << endl << endl;

        cout << "1. Generate inverted file index" << endl;
        cout << "2. Clear current inverted file index" << endl;
        cout << "3. Search a query in the existing index" << endl;
        cout << "4. Generate additional statistics" << endl;
        cout << "0. Exit not ratchet search engine" << endl;

        cout << "option (please enter an integer): ";
        string option;
        cin >> option;
        // populating indexes based on path entered by user
        if(option == "1") {
            string userPath;
            cout << "Enter a path to the folder: ";
            cin >> userPath;
            path = userPath;
            try {
                scan.generateStopWords(stopFile, stopWords);
                scan.indexFiles(path.c_str(), stopWords, wordsIndex, files, PersonsIndex, OrgIndex, words);
                scan.CalcIDF(wordsIndex, words, files.size());
            }   catch (filesystem::filesystem_error) {
                cout << "Invalid path entered, try a different path" << endl;
            }
            continue;
        }
        // clearing indexes
        if(option == "2") {
            wordsIndex.~DSAVLTree();
            PersonsIndex.~DSAVLTree();
            OrgIndex.~DSAVLTree();
            files.clear();
            words.clear();
            continue;
        }
        if(option == "3") {
            // querying
            if(wordsIndex.size == 0){
                cout << "No index found, generate inverted file index" << endl;
                continue;
            } else {
                string query;
                cout << "Enter your query: ";
                cin.ignore();
                getline(cin, query);
                queue.processQuery(query, wordsIndex, PersonsIndex, OrgIndex, files);
            }
            continue;
        }
        if(option == "4") {
            // additional statistics
            if (words.empty()) {
                cout << "need to create inverted file index" << endl;
                continue;
            }
            cout << "total number of articles indexed: " << files.size() << endl;
            cout << "total number of unique words indexed: " << words.size() << endl;
            cout << "unique persons " << PersonsIndex.size << endl;
            cout << "unique orgs " << OrgIndex.size << endl;
            set<pair<int, string>> SetPrint;
            for (auto X: words) {
                SetPrint.insert({X.second, X.first});
            }
            cout << "top 25 words by frequency" << endl;
            int index = 1;
            for (auto res = SetPrint.crbegin(); res != SetPrint.crend(); ++res) {
                    cout << index << ". " << res->second << " " << words[res->second] << endl;
                    index++;
                    if (index > 25) {
                        break;
                    }
                }
                continue;
            }

        if(option == "0") {
            cout << "Thanks for using the def not ratchet search engine, come again!" << endl;
            x = false;
            continue;
        }
        else {
            cout << "continuing back to menu." << endl;
        }
    }
}

