cmake_minimum_required(VERSION 3.12)
project(configclient VERSION 0.0.1)

find_package(daq-cmake REQUIRED)

daq_setup_environment()

#find_package(logging REQUIRED)
#find_package(nlohmann_json REQUIRED)
find_package(ers REQUIRED)
find_package(Boost REQUIRED)

##############################################################################
# Main library
#target_include_directories(ConfigClient SYSTEM PRIVATE Boost)
include_directories(SYSTEM PRIVATE ${Boost_INCLUDE_DIR})
daq_add_library(ConfigClient.cpp ) #LINK_LIBRARIES ers})

##############################################################################
# Integration tests

daq_add_application(configclient_test_app ccTest.cpp TEST LINK_LIBRARIES configclient pthread)

##############################################################################
# No unit tests written

##############################################################################

daq_install()
