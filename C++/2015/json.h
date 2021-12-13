#ifndef JSON
#define JSON

#include <string>
#include <unordered_map>
#include <vector>

struct JSON_node {
    virtual ~JSON_node() noexcept = default;
};

struct JSON_num : public JSON_node {
    double value;
};

struct JSON_string : public JSON_node {
    std::string value;
};

struct JSON_array : public JSON_node {
    std::vector<JSON_node*> members;

    JSON_array() = default;
    JSON_array(const JSON_array&) = delete;
    JSON_array& operator=(const JSON_array&) = delete;
    JSON_array(JSON_array&&) noexcept = default;
    JSON_array& operator=(JSON_array&& rhs) noexcept {
        if (this != &rhs) {
            this->~JSON_array();
            members = std::move(rhs.members);
        }
        return *this;
    }

    ~JSON_array() noexcept {
        for (auto n : members)
            delete n;
    }
};

struct JSON_object : public JSON_node {
    std::unordered_map<std::string, JSON_node*> fields;

    JSON_object() = default;
    JSON_object(const JSON_object&) = delete;
    JSON_object& operator=(const JSON_object&) = delete;
    JSON_object(JSON_object&&) noexcept = default;
    JSON_object& operator=(JSON_object&& rhs) noexcept {
        if (this != &rhs) {
            this->~JSON_object();
            fields = std::move(rhs.fields);
        }
        return *this;
    }

    ~JSON_object() noexcept {
        for (auto n : fields)
            delete n.second;
    }
};

#endif // JSON
