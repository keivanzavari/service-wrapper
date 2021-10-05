#include <ros/ros.h>
#include <sstream>
#include <string>

#include "std_msgs/String.h"
#include "sv_msgs/setString.h"

namespace wrapper {
template<typename Req, typename Res>
class Server {
 public:
  void initializePublisher() {
    constexpr int queue_size = 1;
    constexpr bool latch = true;
    publisher_ = node_handle_.advertise<std_msgs::String>(node_name_ + "/services", queue_size, latch);
  }

  Server(const std::string& service_name, std::function<bool(Req&, Res&)> callback) :
      node_handle_(), service_name_(service_name), callback_(callback), node_name_(ros::this_node::getName()) {
    createService();
    this->initializePublisher();
  }

  void createService() {
    service_server_ = node_handle_.advertiseService(service_name_, &Server::wrapperCallback, this);
  }

  bool wrapperCallback(Req& req, Res& res) {
    ROS_INFO_STREAM("received service call " << req);
    publishServiceCall(req);
    return callback_(req, res);
  }

  void publishServiceCall(const Req& req) {
    std::stringstream ss;
    ss << "Service call " << node_name_ << "/" << service_name_ << ", req: " << req;
    std_msgs::String msg;
    msg.data = ss.str();
    publisher_.publish(msg);
  }

 private:
  ros::NodeHandle node_handle_;
  ros::ServiceServer service_server_;
  ros::Publisher publisher_;
  std::function<bool(Req&, Res&)> callback_;
  std::string service_name_;
  std::string node_name_;
};
}  // namespace wrapper

int main(int argc, char** argv) {
  ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Info);

  ros::init(argc, argv, "test_node");
  std::string service_name("test_service");
  // ros::NodeHandle nh;
  auto callback = [](sv_msgs::setStringRequest& req, sv_msgs::setStringResponse& res) {
    ROS_INFO_STREAM("callback received " << req);

    res.success = true;
    return true;
  };
  wrapper::Server<sv_msgs::setStringRequest, sv_msgs::setStringResponse> service_wrapper(service_name, callback);

  ros::spin();

  return 0;
}
