#include "cli/arguments.hpp"
#include <stdexcept>

namespace cli
{

Arguments::Arguments()
{

}

Arguments::~Arguments()
{

}

bool Arguments::contains(char id) const
{
    return values.end() != values.find(id);
}

std::string const & Arguments::get(char id) const
{
    auto it = values.find(id);
    if (values.end() == it)
    {
        throw  std::out_of_range("id not found : " + id);
    }

    return it->second;
}

void Arguments::set(char id, std::string const & value)
{
    values[id] = value;
}

}