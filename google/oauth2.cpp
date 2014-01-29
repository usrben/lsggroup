#include <boost/algorithm/string/replace.hpp>  
#include <boost/network/protocol/http/client.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <iostream>
#include <sstream>
#include <cstdint>
#include <stdexcept>

#include "oauth2.hpp"
#include "utility/json.hpp"

using usrben::google::oauth2;
using namespace usrben::google;

const std::string oauth2::authentication_base_url = "https://accounts.google.com/o/oauth2/auth";
const std::string oauth2::access_token_base_url = "https://accounts.google.com/o/oauth2/token";
const std::string oauth2::browser_command = "gnome-open";

oauth2::oauth2(const std::string & client_id, 
        const std::string & client_secret, const std::string & scope,
        const std::string & access_token_filename) : 
    client_id_(client_id), client_secret_(client_secret), scope_(scope), 
    access_token_path_(boost::filesystem::path(access_token_filename))
{
}

std::string oauth2::get_access_token()
{
    namespace filesystem = boost::filesystem;

    if (access_token_.access_token.empty())
    {
        // Check if access token file exists
        if (filesystem::exists(access_token_path_) &&            
            filesystem::file_size(access_token_path_) > 0)            
        {            
            filesystem::ifstream access_token_stream(access_token_path_);
            access_token_stream >> access_token_;
        }            
        else // Otherwise, perform OAuth2 handshake           
        {            
            auto auth_code = get_authorization_code();            
            auto access_token_json = get_access_token_json(auth_code);
            
            process_access_token_json(access_token_json);
        }            
    }
    
    return access_token_.access_token;
}

void oauth2::process_access_token_json(const std::string & access_token_json)
{
    std::istringstream access_token_stream(access_token_json);
    access_token_stream >> access_token_;

    // Save JSON token to file            
    boost::filesystem::ofstream fout(access_token_path_);            
    fout << access_token_;            
}

std::string oauth2::refresh_access_token()
{
    if (access_token_.refresh_token.empty())
    {
        throw std::runtime_error("Refresh token not found. Call get_access_token() first");
    }

    auto access_token_json = refresh_access_token_json();

    process_access_token_json(access_token_json);

    return access_token_.access_token;
}

std::string oauth2::get_authorization_code()
{
    std::string url_full = authentication_base_url + 
        "?scope=" + boost::algorithm::replace_all_copy(scope_, " ", "%20") +
        "&redirect_uri=urn:ietf:wg:oauth:2.0:oob" +
        "&response_type=code" +
        "&client_id=" + client_id_;

    open_url(url_full);

    std::string code = "";
    std::cout << "Access code: ";
    std::cin >> code;

    return code;
}

std::string oauth2::get_access_token_json(const std::string & authorization_code)
{
    std::string request_body = "code=" + authorization_code +
        "&client_id=" + client_id_ +
        "&client_secret=" + client_secret_ +
        "&redirect_uri=urn:ietf:wg:oauth:2.0:oob" +
        "&grant_type=authorization_code";

    return post_token(request_body);
}

std::string oauth2::refresh_access_token_json()
{
    std::string request_body = "client_id=" + client_id_ +
        "&client_secret=" + client_secret_ +
        "&refresh_token=" + access_token_.refresh_token +
        "&grant_type=refresh_token";

    return post_token(request_body);
}

void oauth2::open_url(const std::string & url)
{
    std::string url_shell_escaped = boost::algorithm::replace_all_copy(url, "&", "\\&");
    system((browser_command + " " + url_shell_escaped + " > /dev/null").c_str());
}

std::string oauth2::post_token(const std::string &request_body)
{
    typedef boost::network::http::basic_client<boost::network::http::tags::http_default_8bit_tcp_resolve, 1, 1> http_client;
    typedef boost::network::http::basic_request<boost::network::http::tags::http_default_8bit_tcp_resolve> http_request;

    http_client client;
    auto request = http_request(access_token_base_url);
    auto response = client.post(request, request_body, std::string("application/x-www-form-urlencoded"));

    return response.body();
}

std::istream& usrben::google::operator>>(std::istream & access_token_stream, oauth2_access_token & access_token)
{
    boost::property_tree::ptree pt;
    utility::read_json_throw_error(access_token_stream, pt);

    access_token.access_token = pt.get<std::string>("access_token"),
    access_token.expires_in = pt.get<int>("expires_in"),
    access_token.token_type = pt.get<std::string>("token_type");

    if (pt.find("refresh_token") != pt.not_found())
    {
        access_token.refresh_token = pt.get<std::string>("refresh_token");
    }

    return access_token_stream;
}

std::ostream & usrben::google::operator<<(std::ostream & access_token_stream, const oauth2_access_token & access_token)
{
    namespace property_tree = boost::property_tree;

    property_tree::ptree pt;

    pt.put<std::string>("access_token", access_token.access_token);
    pt.put<int>("expires_in", access_token.expires_in);
    pt.put<std::string>("token_type", access_token.token_type);
    pt.put<std::string>("refresh_token", access_token.refresh_token);

    property_tree::write_json(access_token_stream, pt);

    return access_token_stream;
}

