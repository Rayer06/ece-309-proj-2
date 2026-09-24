#include <string>

using namespace std;

enum class Role { System, User, Assistant };

// Message class
class Message {
public:
    // Default-constructs an empty System message with empty content.
    // Needed so Conversation can allocate raw array slots before
    // append() fills them in.
    Message() {
        role_ = Role::System;
        content_ = "";
    }

    Message(Role role, std::string content) {
        role_ = role;
        content_ = content;
    }

    // Who sent the message
    Role role() const noexcept {
        return role_;
    }
    // The message text
    const std::string& content() const noexcept {
        return content_;
    }

private:
    Role        role_;
    std::string content_;
};

/*class Message {
public:
    // Default-constructs an empty System message with empty content.
    // Needed so Conversation can allocate raw array slots before
    // append() fills them in.
    Message();

    Message(Role role, std::string content);

    Role               role()    const noexcept;  // Who sent this message.
    const std::string& content() const noexcept;  // The message text.

private:
    Role        role_;
    std::string content_;
};
*/