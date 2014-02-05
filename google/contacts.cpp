#include <boost/property_tree/ptree.hpp>
#include <unordered_map>
#include "contacts.hpp"
#include "utility/http.hpp"

using namespace usrben::google;

contacts::contacts(const std::shared_ptr<oauth2> & oauth, 
        const std::string & user_id) :
    oauth_(oauth), user_id_(user_id)
{
}

std::map<std::string, std::string> 
    contacts::get_names(const std::list<std::string> & email_list)
{
    using boost::property_tree::ptree;

    const std::string url = "https://www.google.com/m8/feeds/contacts/" + 
        user_id_ + "/full?alt=json";
    
    ptree pt;
    utility::http::get_json_refresh_token(url, oauth_, pt);

    std::unordered_map<std::string, std::string> all_contacts;
    std::map<std::string, std::string> requested_contacts;
    for (auto contact : pt.get_child("feed.entry"))
    {
        auto emails = contact.second.get_child("gd$email");
        if (emails.begin() != emails.end())
        {
            all_contacts[contact.second.get_child("gd$email").begin()->second.get<std::string>("address")] =
                contact.second.get<std::string>("title.$t"); 
        }
    }

    for (auto email : email_list)
    {
        requested_contacts[email] = all_contacts[email];
    }

    return requested_contacts;
}

