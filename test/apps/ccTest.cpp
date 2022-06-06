#include <iostream>

#include "ConfigClient.hpp"
#include "ConfigClientIssues.hpp"

using namespace dunedaq::configclient;

int main(int argc, char* argv[]) {
  ConfigClient client("ccTest", "localhost", "5000");

  std::string myConf="{\"host\": \"192.168.1.102\", \"port\": 9999}";
  std::string res="xp1, xp2";

  std::cout << "Publishing my conf" << std::endl;
  client.publish(myConf,res);

  std::string resource="xp2";
  if (argc>1) {
    resource=std::string(argv[1]);
  }
  std::cout << "Looking up resource " << resource << std::endl;
  std::string app;
  try {
    app=client.getResourceApp(resource);
    std::cout << "app=" << app << std::endl;
  }
  catch(FailedLookup& ex) {
    std::cout << "Lookup of " << resource << " failed. " << ex << std::endl;
  }

  if (app!="") {
    std::cout << "Looking up config for app " << app << std::endl;
    try {
      std::string conf=client.getAppConfig(app);
      std::cout << "conf=" << conf << std::endl;
    }
    catch(...) {
      std::cout << "Lookup of config for " << app << " failed" << std::endl;
    }
  }

  std::cout << "Retracting configuration for " << client.name() << std::endl;
  client.retract();

  std::cout << "Retracting configuration for " << client.name() 
            << " again" << std::endl;
  try {
    client.retract();
  }
  catch (FailedRetract& ex) {
    std::cout << "caught exception: " << ex <<std::endl;
  }
  std::cout << "Looking up config for app " << client.name() << std::endl;
  try {
    std::string conf=client.getAppConfig(client.name());
    std::cout << "conf=" << conf << std::endl;
  }
  catch(FailedLookup& ex) {
    std::cout << "Lookup of config for " << client.name() << " failed" << std::endl;
    std::cout << "caught exception: " << ex <<std::endl;
  }

  return 0;
}
