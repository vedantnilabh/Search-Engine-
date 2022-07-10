//
// Created by Vedant Nilabh on 6/22/22.
//

#include "fileobj.h"

fileobj::fileobj() {

}

fileobj::fileobj(string pubDate, string publisher, string title, string docID) {
    this->pubDate = pubDate;
    this->publisher = publisher;
    this->title = title;
    this->docID = docID;
}

