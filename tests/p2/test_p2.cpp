// tests/p2/test_p2.cpp
//
// YOUR test suite goes here. At least 12 assert-based test cases — see
// spec §5 for the required categories and the sample test for the
// expected level of rigor.
//
// This file is a stub so the project builds out of the box; replace the
// body of main() with your own tests.

//#include "core/message.h"
//#include "core/conversation.h"
#include "core/sentinel_scanner.h"
#include "harness/harness.h"
#include "model/model_client.h"
#include "model/replay_client.h"
#include "model/scripted_client.h"
#include <iostream>

#include <cassert>
#include <cstring>

int main() {
    // Test 1: Empty Conversation Bounds
    Conversation conv;
    try {
        const Message& m = conv.at(0);
        const Message* n = conv.begin();
        const Message* o = conv.end();
    } catch (const std::exception& e) {
        assert(strcmp(e.what(), "OOB"));
    }

    // Test 2: System Message Ordering
    const Message m1 = {Role::System, "This is a system message"};
    const Message m2 = {Role::User, "This is a test"};
    const Message m3 = {Role::Assistant, "This is a test 2"};
    const Message m4 = {Role::User, "This is a test 3"};

    conv.append(m1);
    conv.append(m2);
    conv.append(m3);
    conv.append(m4);
    assert(conv.begin()->role() == Role::System);

    // Test 3: Rule of Five (Copy)
    Conversation c1;
    c1.append(m1);
    c1.append(m2);
    c1.append(m3);
    Conversation c2(c1);
    assert(c1.begin() != c2.begin());

    // Test 4: Rule of Five (Move)
    Conversation c3;
    c3.append(m1);
    c3.append(m2);
    c3.append(m3);
    const Message* tmp = c3.begin();
    Conversation c4 = std::move(c3);
    assert(c3.begin() == nullptr);
    assert(c3.size() == 0);
    assert(c3.end() == nullptr);
    assert(tmp == c4.begin());

    // Test 5: Growth behavior
    const Message* prevData;
    Conversation c5;
    assert(c5.size() == 0);
    c5.append(m1);
    assert(c5.size() == 1);
    prevData = c5.begin();
    for (std::size_t i = 2; i < 20; i++) {
        c5.append(m2);
        assert(c5.size() == i);
        assert(c5.at(0).role() == m1.role());
        assert(c5.at(0).content() == m1.content());
        if ((i == 2) || (i == 4) || (i == 8) || (i == 16)) {
            prevData = c5.begin();
        }
        if ((i == 3) || (i == 5) || (i == 9) || (i == 17)) {
            assert(c5.begin() != prevData);
        }
    }

    // Test 6: Scanner (clean text)
    const std::string sentinel = "<|end_conversation|>";
    const std::string text = "Goodbye." + sentinel;
    SentinelScanner scanner1(sentinel);
    auto out1 = scanner1.feed(text);
    assert(out1.safe_text == "Goodbye.");
    assert(out1.sentinel_found);

    // Test 7: Scanner (Split Sentinel)
    for (std::size_t split = 0; split <= text.size(); ++split) {
        SentinelScanner scanner2(sentinel);
        auto out3 = scanner2.feed(text.substr(0, split));
        auto out4 = scanner2.feed(text.substr(split));
        assert((out3.sentinel_found || out4.sentinel_found) && "sentinel must be caught regardless of split point");
        assert(out3.safe_text + out4.safe_text == "Goodbye.");
    }

    // Test 8: Scanner (False Alarms)
    const std::string sentinel2 = "<|end_world|>";
    const std::string text2 = "Goodbye." + sentinel2;
    SentinelScanner scanner3(sentinel);
    auto out5 = scanner3.feed(text2);
    assert(out5.safe_text == "Goodbye.<|end_world|>");
    assert(!out5.sentinel_found);

    // Test 9: Scanner (Bounded Memory)
    const std::string partial_sentinel = "<|end_";
    std::string text3 = "";
    SentinelScanner scanner4(sentinel);
    for (int i = 0; i < 100; i++) {
        text3 = text3.append(partial_sentinel); // "<|end_<|end_<|end_..."
    }
    auto out6 = scanner4.feed(text3);
    assert(scanner4.pending_.size() <= sentinel.size()-1);

    return 0;
}
