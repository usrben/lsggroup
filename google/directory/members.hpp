#ifndef USRBEN_GOOGLE_DIRECTORY_MEMBERS_HPP
#define USRBEN_GOOGLE_DIRECTORY_MEMBERS_HPP

#include <string>
#include <memory>
#include <list>

#include "../oauth2.hpp"

namespace usrben { namespace google { namespace directory
{
    class members
    {
    public:
        members(std::shared_ptr<usrben::google::oauth2> oauth, const std::string & group_key);

        std::list<std::string> list_all();
         
    private:
        struct member
        {
            std::string kind;
            std::string etag;
            std::string id;
            std::string email;
            std::string role;
            std::string type;
        };

        std::string group_key_;
        std::shared_ptr<usrben::google::oauth2> oauth_;
    };
} // namespace directory

} // namespace google

} // namespace usrben

#endif // USRBEN_GOOGLE_DIRECTORY_MEMBERS_HPP

