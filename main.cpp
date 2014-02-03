#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include "google/oauth2.hpp"
#include "google/directory/members.hpp"

using namespace usrben::google;
namespace po = boost::program_options;
namespace filesystem = boost::filesystem;

struct options
{
    options() : 
        client_id("client-id", ""),
        client_secret("client-secret", ""),
        group_key("group-key", "")
    {
    }

    std::pair<std::string, std::string> client_id;
    std::pair<std::string, std::string> client_secret;
    std::pair<std::string, std::string> group_key;
};

void get_options(int argc, char *argv[], const std::string & config_file_name, options & ops);
template <typename T>
T get_option(const po::variables_map & vm, const std::string & option_name);

int main(int argc, char *argv[])
{
    const std::string scope = "https://www.googleapis.com/auth/admin.directory.group.readonly";
    const std::string config_file_name = "lsggroup.cfg";

    try
    {
        options ops;
        get_options(argc, argv, config_file_name, ops);

        auto oauth2_ptr = std::make_shared<oauth2>(ops.client_id.second, ops.client_secret.second, scope, "oauth2.txt");

        auto directory_members = directory::members(oauth2_ptr, ops.group_key.second);

        auto emails = directory_members.list_all();

        for (auto email : emails)
        {
            std::cout << email << "\n";
        }
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return 0;
}

void get_options(int argc, char *argv[], const std::string & config_file_name, options & ops)
{
    //Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "Display help message")
        (ops.client_id.first.c_str(), po::value<std::string>(), "Google API client ID")
        (ops.client_secret.first.c_str(), po::value<std::string>(), "Google API client secret")
        (ops.group_key.first.c_str(), po::value<std::string>(), "Group ID (e.g., mygroup@example.com)");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);

    if (filesystem::exists(config_file_name))
    {
        po::store(po::parse_config_file<char>("lsggroup.cfg", desc), vm);
    }

    po::notify(vm);
   
    if (vm.count("help"))
    {
        std::cout << desc << "\n";
        std::exit(0);
    }

    ops.client_id.second = get_option<std::string>(vm, ops.client_id.first);
    ops.client_secret.second = get_option<std::string>(vm, ops.client_secret.first);
    ops.group_key.second = get_option<std::string>(vm, ops.group_key.first);
}

template <typename T>
T get_option(const po::variables_map & vm, const std::string & option_name)
{
    if (vm.count(option_name))
    {
        return vm[option_name].as<T>();
    }
    else
    {
        std::cerr << "Missing required option: " << option_name << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

