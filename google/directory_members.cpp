#include <boost/property_tree/ptree.hpp>
#include "directory_members.hpp"
#include "utility/http.hpp"

using namespace usrben::google;

directory_members::directory_members(
        const std::shared_ptr<usrben::google::oauth2> & oauth, 
        const std::string & group_key) :
    oauth_(oauth),
    group_key_(group_key)
{
}

std::list<std::string> directory_members::list_all()
{
    using boost::property_tree::ptree;

    const std::string url = 
        "https://www.googleapis.com/admin/directory/v1/groups/" + 
        group_key_ + "/members";

    ptree pt;
    utility::http::get_json_refresh_token(url, oauth_, pt);

    std::list<std::string> emails;
    for (auto m : pt.get_child("members"))
    {
        emails.push_back(m.second.get<std::string>("email"));
    }

    return emails;
}

