#ifndef USRBEN_GOOGLE_UTILITY_JSON_HPP
#define USRBEN_GOOGLE_UTILITY_JSON_HPP

#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace usrben { namespace google { namespace utility
{
    namespace property_tree = boost::property_tree;

    class json_error : public std::runtime_error
    {
    public:
        json_error(const std::string & message) : runtime_error(message)
        {
        }
    };

    inline void read_json_throw_error(std::istream & json_stream, property_tree::ptree & pt)
    {
        property_tree::read_json(json_stream, pt);

        if (pt.find("error") != pt.not_found())
        {
            std::ostringstream strout;
            property_tree::write_json(strout, pt);
            throw json_error(strout.str());
        }
    }
}; // namespace utility

}; // namespace google

}; // namespace usrben

#endif
