#include <string>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;       // from <boost/beast/http.hpp>
namespace net = boost::asio;        // from <boost/asio.hpp>

class ConfigClient {
public:
  ConfigClient(const std::string& name, const std::string& server,
               const std::string& port);
  ~ConfigClient();

  void publish(const std::string& config, const std::string& resources="");
  void retract();
  std::string getAppConfig(const std::string& appName);
  std::string getResourceApp(const std::string& resource);
  const std::string& name() const {return m_name;};
private:
  std::string get(const std::string& target);

  std::string m_name;
  net::io_context m_ioContext;
  beast::tcp_stream m_stream;
  beast::flat_buffer m_buffer;
};
