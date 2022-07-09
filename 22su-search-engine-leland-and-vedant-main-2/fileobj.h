//
// Created by Vedant Nilabh on 6/22/22.
//

#ifndef INC_22SU_SEARCH_ENGINE_FILEOBJ_H
#define INC_22SU_SEARCH_ENGINE_FILEOBJ_H

#include <string>
using namespace std;
class fileobj {

public:
    string pubDate;
    string publisher;
    string title;
    string docID;
    fileobj();
    fileobj(string pubDate, string publisher, string title, string dociD);
};


#endif //INC_22SU_SEARCH_ENGINE_FILEOBJ_H
