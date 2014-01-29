#ifndef USRBEN_GOOGLE_OAUTH2_HPP
#define USRBEN_GOOGLE_OAUTH2_HPP

#include <string>
#include <memory>
#include <boost/filesystem.hpp>

namespace usrben { namespace google 
{
    struct oauth2_access_token
    {
        std::string access_token;
        std::string refresh_token;
        int expires_in;
        std::string token_type;
    };

    std::istream & operator>>(std::istream & access_token_stream, oauth2_access_token & access_token);
    std::ostream & operator<<(std::ostream & access_token_stream, const oauth2_access_token & access_token);

    class oauth2
    {
    public:
        oauth2(const std::string & client_id, 
                const std::string & client_secret, const std::string & scope, 
                const std::string & access_token_filename);
     
        std::string get_access_token();
        std::string refresh_access_token();

    private:
        static const std::string authentication_base_url;
        static const std::string access_token_base_url;
        static const std::string browser_command;

        std::string client_id_;
        std::string client_secret_;
        std::string scope_;
        boost::filesystem::path access_token_path_;
        oauth2_access_token access_token_;

        std::string get_authorization_code();
        std::string get_access_token_json(const std::string & authorization_code);
        std::string refresh_access_token_json();
        void open_url(const std::string & url);
        std::string post_token(const std::string &request_body);
        void process_access_token_json(const std::string & access_token_json);
    };
};
};

#endif // USRBEN_GOOGLE_OAUTH2_HPP

