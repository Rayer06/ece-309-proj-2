#include "core/sentinel_scanner.h"

#include <string>
//using namespace std;


SentinelScanner::SentinelScanner(std::string sentinel) {
    sentinel_ = sentinel;
    pending_ = "";
}

// Feed the next chunk. Returns text guaranteed NOT to be part of
// the sentinel (safe to print immediately) and whether the
// sentinel has now been fully seen.
SentinelScanner::Out SentinelScanner::feed(std::string_view chunk) {
    pending_.append(chunk); // adds entire chunk to string
    if (pending_.size() > sentinel_.size()-1) {
        std::size_t charsToRemove = pending_.size() - (sentinel_.size() - 1);
        pending_.erase(0, charsToRemove); // if too long, remove characters
    }

    bool sentinel_start = false;
    std::size_t last_match = 0;
    int where_match = 0;

    Out put; // create new output container
    put.safe_text = "";
    put.sentinel_found = false;

    // string to search
    std::string tmp = "";
    tmp = tmp.append(pending_);
    tmp = tmp.append(chunk);

    // traverse pending_ + chunk string until start of sentinel found, then
    // put remaining characters into sentinel_so_far_

    // search for all


    // new logic:
    // search string for "<", "<|", "<|e", etc in sequence
    // if no sequence returns match, all text is safe text
    // if every iteration returns match, sentinel found, return only text excluded in sentinel
    // if only some iterations return match:
    // if last matched iteration hit the end of the string, sentinel not found,
    // but this text excluded from safe text
    // if last matched iteration does not hit end of string, sentinel not found,
    // all text is safe text

    for (std::size_t i = 0; i < sentinel_.size(); i++) {
        if (tmp.find(sentinel_.substr(0, i+1))) {
            where_match = tmp.find(sentinel_.substr(0, i+1));
            sentinel_start = true;
            last_match = i;
        }
        if (sentinel_start == false) {
            put.safe_text.append(tmp);
            put.sentinel_found = false;
        }
        if ((sentinel_start == true) && (last_match == tmp.size()-1)) {
            put.safe_text.append(tmp.substr(0, where_match));
            put.sentinel_found = true;
        }
        if ((sentinel_start == true) && (last_match != tmp.size()-1)) {
            if (where_match + i == tmp.size()) {
                put.safe_text.append(tmp.substr(0, where_match));
            } else {
                put.safe_text.append(tmp);
            }
            put.sentinel_found = false;
        }
    }
    return put;
}
/*
    for (int i = 0; i < tmp.size(); i++)
        if (sentinel_so_far_.empty()) { // regular string mode
            if (tmp[i] != '<') {
                put.safe_text.append(1, tmp[i]);
            } else {
                sentinel_so_far_.append(1, tmp[i]);
            }




            // sentinel match search mode
            if (tmp[i] == sentinel_[sentinel_so_far_.size()]) { // if continues matching
                sentinel_so_far_.append(1, tmp[i]);
            } else { // if match breaks eg "<|end_w"
                put.safe_text.append(sentinel_so_far_);
                sentinel_so_far_.clear();
                put.safe_text.append(1, tmp[i]);
            }
            // if fully matched
            if (tmp.find(sentinel_)) {

            }

        }

    return put;

}
*/
// Call once, after the stream ends, to release any text still
// being held back.
SentinelScanner::Out SentinelScanner::flush() {
    Out put;
    put.safe_text = pending_;
    put.sentinel_found = false;
    return put;
}