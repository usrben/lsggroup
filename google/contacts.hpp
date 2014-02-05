#ifndef USRBEN_GOOGLE_CONTACTS_HPP
#define USRBEN_GOOGLE_CONTACTS_HPP

#include <map>
#include <string>
#include <list>
#include <memory>
#include "oauth2.hpp"

namespace usrben { namespace google
{
    class contacts
    {
    public:
        contacts(const std::shared_ptr<oauth2> & oauth, 
            const std::string & user_id);
        std::map<std::string, std::string> 
            get_email_names(const std::list<std::string> & email_list);
    private:
        std::shared_ptr<oauth2> oauth_;
        std::string user_id_;
    };
} // namespace google

} // namespace usrben

#endif // USRBEN_GOOGLE_CONTACTS_HPP
