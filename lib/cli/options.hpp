#ifndef CLI_OPTIONS_HPP
#define CLI_OPTIONS_HPP

#include <string>
#include <vector>

#include "cli/argument.hpp"

extern "C" 
{
#include <getopt.h>
}

namespace cli
{

class Options
{
public:
    explicit Options(std::vector<Argument> const & args);
    ~Options();
    char const * shortOpts() const;
    option const * longOpts() const;
private:
    option * long_opts;
    std::string short_opts;
};

}


#endif