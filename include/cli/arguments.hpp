#ifndef CLI_ARGUMENTS_HPP
#define CLI_ARGUMENTS_HPP

#include <string>
#include <unordered_map>
namespace cli
{

class Arguments
{
public:
    Arguments();
    ~ Arguments();
    bool contains(char id) const;
    std::string const & get(char id) const;
    void set(char id, std::string const & value);
private:
    std::unordered_map<char, std::string> values;    
};

}

#endif
