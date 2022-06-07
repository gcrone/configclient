#include <cstdlib>
#include <iostream>

#include "ConfigClient.hpp"
#include "ConfigClientIssues.hpp"

#include "logging/Logging.hpp"
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>

using tcp = net::ip::tcp;           // from <boost/asio/ip/tcp.hpp>
namespace http = beast::http;       // from <boost/beast/http.hpp>

using namespace dunedaq::configclient;

ConfigClient::ConfigClient(const std::string& name, const std::string& server,
                           const std::string& port)
  : m_name(name), m_stream(m_ioContext) {

  tcp::resolver resolver(m_ioContext);
  auto const addr=resolver.resolve(server,port);
  m_stream.connect(addr);
}

ConfigClient::~ConfigClient(){
  beast::error_code ec;
  m_stream.socket().shutdown(tcp::socket::shutdown_both, ec);
}

void ConfigClient::publish(const std::string& config,
                           const std::string& resources) {
  std::string target="/publish";
  http::request<http::string_body> req{http::verb::post, target, 11};
  req.set(http::field::content_type,"application/x-www-form-urlencoded");

  req.body()="app="+m_name+"&conf="+config+"&resources="+resources;
  req.prepare_payload();
  http::write(m_stream, req);

  http::response<http::string_body> response;
  http::read(m_stream, m_buffer, response);
  if (response.result_int()!=200) {
    throw(FailedPublish(ERS_HERE,std::string(response.reason())));
  }
}

void ConfigClient::retract() {
  std::string target="/retract";
  http::request<http::string_body> req{http::verb::post, target, 11};
  req.set(http::field::content_type,"application/x-www-form-urlencoded");
  req.body()="app="+m_name;
  req.prepare_payload();
  http::write(m_stream, req);
  http::response<http::string_body> response;
  http::read(m_stream, m_buffer, response);
  if (response.result_int()!=200) {
    throw(FailedRetract(ERS_HERE,m_name,std::string(response.reason())));
  }
}

std::string ConfigClient::get(const std::string& target) {
  http::request<http::string_body> req{http::verb::get, target, 11};
  http::write(m_stream, req);

  http::response<http::string_body> response;
try{
  http::read(m_stream, m_buffer, response);
}
catch(std::exception& e) {
 std::cerr<< "caught exception " << e.what() << std::endl;
}
  TLOG_DEBUG(25) << "get " << target << " response: " << response;

  if (response.result_int() != 200) {
    throw(FailedLookup(ERS_HERE, target, std::string(response.reason())));
  }
  return response.body();
}

std::string ConfigClient::getAppConfig(const std::string& appName) {
  std::string target="/getapp/"+appName;
  return get(target);
}


std::string ConfigClient::getResourceApp(const std::string& resource) {
  std::string target="/getresource/"+resource;
  return get(target);
}
