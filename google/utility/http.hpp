#ifndef USRBEN_GOOGLE_UTILITY_HTTP_HPP
#define USRBEN_GOOGLE_UTILITY_HTTP_HPP

#include <stdexcept>
#include <boost/network.hpp>
#include "json.hpp"

namespace usrben { namespace google { namespace utility { namespace http
{
    using namespace usrben::google::utility;

    const int status_success = 200;
    const int status_redirect = 300;
    const int status_client_error = 400;
    const int status_server_error = 500;

    class http_error : public std::runtime_error
    {
    public:
        http_error(int status_code, const std::string & message) : 
            runtime_error(message),
            status_code(status_code)
        {
        }
        const int status_code;
    };

    typedef boost::network::http::basic_client<boost::network::http::tags::http_default_8bit_tcp_resolve, 1, 1> http_client;
    typedef boost::network::http::basic_request<boost::network::http::tags::http_default_8bit_tcp_resolve> http_request;
    typedef boost::network::http::basic_response<boost::network::http::tags::http_default_8bit_tcp_resolve> http_response;

    inline void set_authorization_header(http_request & request,
            std::shared_ptr<usrben::google::oauth2> & oauth)
    {
        auto access_token = oauth->get_access_token();
        request << boost::network::remove_header("Authorization");
        request << boost::network::header("Authorization", "Bearer " + access_token);
    }

    inline http_response get_json(const std::string & url,
            std::shared_ptr<oauth2> & oauth, 
            const std::list<std::pair<std::string, std::string>> & headers)
    {
        http_client client;
        auto request = http_request(url);
        set_authorization_header(request, oauth);
        for (auto header : headers)
        {
            request << boost::network::header(header.first, header.second);
        }

        return client.get(request);
    }

    inline void get_json_refresh_token(const std::string & url, 
            std::shared_ptr<oauth2> & oauth,
            const std::list<std::pair<std::string, std::string>> & headers,
            boost::property_tree::ptree & pt)
    {
        auto response = get_json(url, oauth, headers);

        auto status_code = boost::network::http::status(response);
        if (status_code >= status_client_error &&
            status_code < status_server_error) 
        {
            oauth->refresh_access_token();
            response = get_json(url, oauth, headers);
        }
        else if (status_code < status_success ||
                 status_code >= status_redirect)
        {
            throw http_error(status_code, 
                    boost::network::http::status_message(response));
        }

        std::istringstream response_stream(boost::network::http::body(response));
        json::read_throw_error(response_stream, pt);
    }

} // http

} // utility

} // google

} // usrben

#endif // USRBEN_GOOGLE_UTILITY_HTTP_HPP

