#include <boost/network/protocol/http/client.hpp>
#include <boost/property_tree/ptree.hpp>
#include "members.hpp"
#include "directory.hpp"
#include "../utility/json.hpp"

using namespace usrben::google;
using namespace usrben::google::directory;

members::members(std::shared_ptr<usrben::google::oauth2> oauth, const std::string & group_key) :
    oauth_(oauth),
    group_key_(group_key)
{
}

std::list<std::string> members::list_all()
{
    const std::string full_url = api_base_url + "/groups/" + group_key_ + "/members";

    namespace network = boost::network;
    typedef network::http::basic_client<network::http::tags::http_default_8bit_tcp_resolve, 1, 1> http_client;
    typedef network::http::basic_request<network::http::tags::http_default_8bit_tcp_resolve> http_request;
    using boost::property_tree::ptree;

    http_client client;
    auto request = http_request(full_url);
    std::string access_token;
    try
    {
        access_token = oauth_->get_access_token();
    }
    catch (utility::json_error & e)
    {
        access_token = oauth_->refresh_access_token();
    }

    request << network::header("Authorization", "Bearer " + oauth_->get_access_token());

    std::istringstream response_stream(network::http::body(client.get(request)));

    //std::cout << network::http::body(response);

    std::list<std::string> emails;

    ptree pt;
    utility::read_json_throw_error(response_stream, pt);

    for (auto m : pt.get_child("members"))
    {
        emails.push_back(m.second.get<std::string>("email"));
    }

    return emails;
}

