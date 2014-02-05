#ifndef USRBEN_GOOGLE_DIRECTORY_MEMBERS_HPP
#define USRBEN_GOOGLE_DIRECTORY_MEMBERS_HPP

#include <string>
#include <memory>
#include <list>

#include "oauth2.hpp"

namespace usrben { namespace google
{
    class directory_members
    {
    public:
        directory_members(
            const std::shared_ptr<usrben::google::oauth2> & oauth, 
            const std::string & group_key);

        std::list<std::string> list_all();
         
    private:
        std::string group_key_;
        std::shared_ptr<usrben::google::oauth2> oauth_;
    };
} // namespace google

} // namespace usrben

#endif // USRBEN_GOOGLE_DIRECTORY_MEMBERS_HPP

