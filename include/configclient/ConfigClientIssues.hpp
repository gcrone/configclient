#ifndef CONFIGCLIENTISSUES_HPP
#define CONFIGCLIENTISSUES_HPP
#include <string>

#include "ers/Issue.hpp"


namespace dunedaq{
  ERS_DECLARE_ISSUE(configclient,
                    EnvNotFound,
                    "Environment variable " << name << " not found",
                    ((std::string)name))
  ERS_DECLARE_ISSUE(configclient,
                    FailedPublish,
                    "Failed to publish configuration " << result,
                    ((std::string)result))
  ERS_DECLARE_ISSUE(configclient,
                    FailedRetract,
                    "Failed to retract configuration " << result,
                    ((std::string)name) ((std::string)result))
  ERS_DECLARE_ISSUE(configclient,
                    FailedLookup,
                    "Failed to lookup " << target << " " << result,
                    ((std::string)target) ((std::string)result))
}

#endif
