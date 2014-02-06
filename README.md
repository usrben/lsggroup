Summary
=======
lsggroup is a simple tool with the following features:
* Retrieve a list of email addresses from the specifed group in a Google Apps
  domain.
* Optionally associated names with email addresses by retrieving contacts
  from the specified Google account
* Optionally format output as CSV

OS support
==========
Currently, lsggroup only runs on Linux distributions that have 'gnome-open'
installed (used for opening a web-browser).

Build
=====
1. Install Boost (tested against v1.55.0)
2. Define BOOST_ROOT environment variable
3. Install cpp-netlib (tested against v0.10.1) to $BOOST_ROOT 
4. mkdir build (in repository root)
5. cd build
6. cmake ..
7. make

Details
=======

Google Cloud Console
--------------------
lsggroup requires you to create a project using the Google Cloud Console:
https://cloud.google.com/console/project

The following APIs have to be enabled for the project
* Admin SDK
* Contacts SDK

The project's *Client ID* and *Client secret* must be provided when running
lsggroup (see options below).


Command-line options
--------------------
The following command-line options are supported (available through --help)

    --help                Print help message 
    --client-id arg       Google API client ID (REQUIRED)
    --client-secret arg   Google API client secret (REQUIRED)
    --group-key arg       Google Apps Group ID (e.g., mygroup@mydomain.com) (REQUIRED)
    --user-id arg         User ID (e.g., john.doe@gmail.com) (OPTIONAL)
    --csv                 Enable CSV formatting (OPTIONAL)

Config file
-----------
All of the command-line options can also be saved in a file named
'lsggroup.cfg' in the same directory as the executable. The config file
contains key-value pairs. Note that command-line options take precedence
if the option appears on both the command-line and the config file.

Example config file contents:

    client-id = MY-CLIENT-ID
    client-secret = MY-CLIENT-SECRET
    group-key = MY-GROUP-KEY
    user-id = MY-USER-ID
    csv =

Running
-------
When running lsggroup for the first time, your default browser will be opened
twice, each time prompting you to give access to a Google account.  
The first time, give access to your Google Apps domain account and copy the
resulting code into the terminal beside "Access code: " and hit Enter.  
Repeat this a second time for the Google account where you want to retrieve
the names associated with the email addresses.  
*NOTE*: If you do not provide 'user-id' option, you will not be prompted
        a second time and only a list of email addresses will be printed out.

