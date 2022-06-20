#include "ConfigClient.hpp"
#include "ConfigClientIssues.hpp"
#include "logging/Logging.hpp"

#include "boost/program_options.hpp"
#include "nlohmann/json.hpp"

#include <iostream>
#include <fstream>

using namespace dunedaq::configclient;
using nlohmann::json;

int main(int argc, char* argv[]) {

  dunedaq::logging::Logging::setup();

  std::string name("ccTest");
  std::string server("localhost");
  std::string port("5000");
  std::string file;
  std::string resource="xp2";
  namespace po = boost::program_options;
  po::options_description desc("Simple test program for ConfigClient class");
  desc.add_options()
    ("file,f", po::value<std::string>(&file), "file to publish as our configuration")
    ("name,n", po::value<std::string>(&name), "name to publish our config under")
    ("port,p", po::value<std::string>(&port), "port to connect to on configuration server")
    ("resource,r", po::value<std::string>(&resource), "name of resource to lookup")
    ("server,s", po::value<std::string>(&server), "Configuration server to connect to")
    ;
  try {
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
  }
  catch (std::exception& ex) {
    std::cerr << "Error parsing command line " << ex.what() << std::endl;
    std::cerr << desc <<std::endl;
    return 0;
  }


  ConfigClient client(server, port);
  std::string myConf;
  if (file!="") {
    std::ifstream infile(file);
    if (infile.is_open()) {
      for (std::string ln; infile >> ln;) {
        myConf+=ln;
      }
    }
    else {
      std::cerr << "Failed to open " << file << std::endl;
      return 0;
    }
  }
  else {
    myConf="{\"host\": \"192.168.1.102\", \"port\": 9999}";
  }
  std::string res="xp1, xp2";
  std::cout << "Publishing my conf as " << name << std::endl;
  client.publish(name,myConf,res);

  std::cout << "Looking up resource " << resource << std::endl;
  std::string app;
  try {
    app=client.getResourceApp(resource);
    std::cout << "Resource belongs to app " << app << std::endl;
  }
  catch(FailedLookup& ex) {
    std::cout << "Lookup of " << resource << " failed. " << ex << std::endl;
  }

  if (app!="") {
    std::cout << "Looking up config for app " << app << std::endl;
    try {
      std::string conf=client.getAppConfig(app);
//      std::cout << "conf=" << conf << std::endl;
      auto jsconf=json::parse(conf);
      for (auto& el : jsconf.items()) {
        std::cout << el.key() << " = " << el.value() << std:: endl;
      }
    }
    catch(FailedLookup& ex) {
      std::cout << "Lookup of config for " << app << " failed" << std::endl;
      std::cout << "caught exception: " << ex <<std::endl;
    }
  }


  if (app!=name) {
    std::cout << "Looking up config for app " << name << std::endl;
    try {
      std::string conf=client.getAppConfig(name);
      auto jsconf=json::parse(conf);
      for (auto& el : jsconf.items()) {
        std::cout << el.key() << " = " << el.value() << std:: endl;
      }
    }
    catch(FailedLookup& ex) {
      std::cout << "Lookup of config for " << name << " failed" << std::endl;
      std::cout << "caught exception: " << ex <<std::endl;
    }
  }


  std::cout << "Retracting configuration for " << name << std::endl;
  try {
    client.retract(name);
  }
  catch (FailedRetract& ex) {
    std::cout << "Failed to retract configuration. Caught exception " << ex << std::endl;
  }

  std::cout << "Retracting configuration for " << name
            << " again" << std::endl;
  try {
    client.retract(name);
  }
  catch (FailedRetract& ex) {
    std::cout << "OK, that failed as expected, caught exception: " << ex <<std::endl;
  }

  std::cout << "Looking up config for app " << name << std::endl;
  try {
    std::string conf=client.getAppConfig(name);
    std::cout << "Woops, that shouldn't have worked, conf=" << conf << std::endl;
  }
  catch(FailedLookup& ex) {
    std::cout << "OK, lookup of config for " << name << " failed" << std::endl;
  }

  return 0;
}
