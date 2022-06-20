#ifndef CONFIGCLIENT_HPP
#define CONFIGCLIENT_HPP

#include <string>

#include <boost/beast/core.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/basic_resolver.hpp>

namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace net = boost::asio;        // from <boost/asio.hpp>

namespace dunedaq {
namespace configclient {
class ConfigClient {
public:
  ConfigClient(const std::string& server,
               const std::string& port);
  ~ConfigClient();

  void publish(const std::string& name, const std::string& config, const std::string& resources="");
  void retract(const std::string& name);
  std::string getAppConfig(const std::string& appName);
  std::string getResourceApp(const std::string& resource);

private:
  std::string get(const std::string& target);

  net::io_context m_ioContext;
  net::ip::basic_resolver<net::ip::tcp>::results_type m_addr;
  beast::tcp_stream m_stream;
  beast::flat_buffer m_buffer;
};
} //configclient
} //dunedaq
#endif
