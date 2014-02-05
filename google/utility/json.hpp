#ifndef USRBEN_GOOGLE_UTILITY_JSON_HPP
#define USRBEN_GOOGLE_UTILITY_JSON_HPP

#include <stdexcept>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace usrben { namespace google { namespace utility { namespace json
{
    class json_error : public std::runtime_error
    {
    public:
        json_error(const std::string & message) : runtime_error(message)
        {
        }
    };

    inline void read_throw_error(std::istream & json_stream, 
        boost::property_tree::ptree & pt)
    {
        boost::property_tree::read_json(json_stream, pt);
//        boost::property_tree::write_json(std::cout, pt);

        if (pt.find("error") != pt.not_found())
        {
            std::ostringstream strout;
            boost::property_tree::write_json(strout, pt);
            throw json_error(strout.str());
        }
    }
} // namespace json

} // namespace utility

} // namespace google

} // namespace usrben

#endif
