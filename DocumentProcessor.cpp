
#include "DocumentProcessor.h"

void DocumentProcessor::generateStopWords(const char * stopFile, std::unordered_set<string> & sWords) {

    ifstream stopF(stopFile);

    if(!stopF.is_open()) {
        cout << "stopFile is not open" << endl;
    }

    while(!stopF.eof()) {
        string temp;
        getline(stopF, temp, '\n');

        sWords.insert(temp);
    }
    stopF.close();
}

void DocumentProcessor::indexFiles(const char *path, const std::unordered_set<string> &stopWords, DSAVLTree<string, unordered_map<int, double>> & wordIndex, unordered_map<int, fileobj>& fileIndex, DSAVLTree<string, unordered_set<int>> & PersonsIndex, DSAVLTree<string, unordered_set<int>> & OrgIndex, unordered_map<string, int>& words) {
    //recursive_director_iterator used to "access" folder at parameter -path-
    //we are using the recursive iterator so it will go into subfolders.
    auto it = std::filesystem::recursive_directory_iterator(path);


    int fileKey = 0;
    //loop over all the entries.
    for(const auto& entry : it) {


        //We only want to attempt to parse files that end with .json...
        if (entry.is_regular_file() && entry.path().extension().string() == ".json") {

            ifstream input(entry.path().c_str());

            if (!input.is_open())
                std::cerr << "cannot open file" << endl;

            //Create a RapidJSON IStreamWrapper using the file input stream above.
            IStreamWrapper isw(input);

            //Create a RapidJSON Document object and use it to parse the IStreamWrapper object above.
            Document d;
            d.ParseStream(isw);

            //Now that the document is parsed, we can access different elements the JSON using
            //familiar subscript notation.

            //This accesses the -title- element in the JSON. Since the value associated with title is a string (rather than
            // an array or something else), we call the GetString() function to return the actual title of the article
            // as a c-string.
            string docID(entry.path().c_str());

            auto val = d["title"].GetString();
            string title(val);
            //cout << "Title: " << title << endl;

            auto pub = d["published"].GetString();
            string published(pub);
            published = published.substr(0, 10);

            auto publicCstr = d["thread"]["site"].GetString();
            string publication(publicCstr);

            fileobj newFile(published, publication, title, docID);
            fileIndex[fileKey] = newFile;

            // getting persons array
            auto persons = d["entities"]["persons"].GetArray();

            // iterating over persons array and adding to index
            for (auto &p: persons) {
                string person = p["name"].GetString();
                PersonsIndex[person].insert(fileKey);
            }

            // getting organizations array, iterating, and adding to index
            auto organizations = d["entities"]["organizations"].GetArray();
            for (auto &o: organizations) {
                string organization = o["name"].GetString();
                OrgIndex[organization].insert(fileKey);
            }
            auto text = d["text"].GetString();
            string textStr(text);

            stringstream stream(textStr);
            string line;
            string buffer;
            while (getline(stream, line)) {
                stringstream lineStream(line);
                while (getline(lineStream, buffer, ' ')) {
                    string substr;
                    for (int i = 0; i < buffer.size(); i++) {
                        if (ispunct(buffer[i])) { // substr is the second part of the string after a punctuation mark
                            substr = buffer.substr(i + 1, buffer.size() - i + 1);
                            buffer = buffer.substr(0, i);
                            if (stopWords.count(substr) == 0) {
                                Porter2Stemmer::trim(substr);
                                Porter2Stemmer::stem(substr);
                            }
                        }
                    }
                    if (stopWords.count(buffer) == 0) { // not in stopwords
                        Porter2Stemmer::trim(buffer);
                        Porter2Stemmer::stem(buffer);
                        if (stopWords.count(buffer) == 0 && buffer.length() > 2) {
                            if (wordIndex[buffer].count(fileKey)) {
                                wordIndex[buffer][fileKey]++; // fileKey is in the list of files associated with the word, incrementing frequency
                            } else {
                                wordIndex[buffer][fileKey] = 1; // first time seeing word in the file, setting frequency to 1
                            }
                            if (words.count(buffer)) {
                                words[buffer]++; // this is for the frequency map we will use for tfidf, to access all the words in word index, and to output the 25 most common words
                            } else { ;
                                words[buffer] = 1;
                            }
                        }
                    }
                    // same logic as before with substr
                    if (stopWords.count(substr) == 0) {
                        if (stopWords.count(substr) == 0 && substr.length() > 2) {
                            if (wordIndex[substr].count(fileKey)) {
                                wordIndex[substr][fileKey]++;
                            } else {
                                wordIndex[substr][fileKey] = 1;
                            }
                            if (words.count(substr)) {
                                words[substr]++;
                            } else { ;
                                words[substr] = 1;
                            }
                        }
                    }
                }
            }



            fileKey++;
            input.close();

            if(fileKey % 10000 == 0) {
                cout << fileKey << endl;
            }
        }

    }
    cout << "PARSED " << fileKey << " FILES" << endl;
}

void DocumentProcessor::CalcIDF(DSAVLTree<string, unordered_map<int, double>> & wordIndex, unordered_map<string, int>& words, const int& numFiles) {
    // iterates over frequency map with words and total frequencies and looks at the unordered map associated with that word,
    // and computes the idf of each of the files to that specific word and multiples by tf to get tf-idf
    for(auto& word: words){
        for (auto &file: wordIndex[word.first]) {
            double idf = log(1 + numFiles / 1 + wordIndex[word.first].size());
            file.second *= idf;
        }
    }

}


