#include <string>

//using namespace std;

enum class Role { System, User, Assistant };

// Message class
class Message {
public:
    // Default-constructs an empty System message with empty content.
    // Needed so Conversation can allocate raw array slots before
    // append() fills them in.
    Message();

    Message(Role role, std::string content);

    // Who sent the message
    Role role() const noexcept;
    // The message text
    const std::string& content() const noexcept;

private:
    Role        role_;
    std::string content_;
};