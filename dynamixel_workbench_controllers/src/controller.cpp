#include "ros/ros.h"
#include "dynamixel_workbench_msgs/DynamixelCommand.h"
#include <cstdlib>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "controller");

  ros::NodeHandle n;
  ros::ServiceClient client = n.serviceClient<dynamixel_workbench_msgs::DynamixelCommand>("/dynamixel_workbench/dynamixel_command");

  dynamixel_workbench_msgs::DynamixelCommand srv;

  std::string item_command = "";
  std::string item_addr = "Goal_Position";
  int ID = 1;
  int value = 0;

  srv.request.command = item_command;
  srv.request.id = ID; //atoll(argv[1]);
  srv.request.addr_name = item_addr;
  srv.request.value = value; //atoll(argv[2]);

  if (client.call(srv))
  {
    ROS_INFO("send ID and Position Value : %ld, %ld", (uint8_t)srv.request.id, (int32_t)srv.request.value);
    ROS_INFO("receive result : %ld", (bool)srv.response.comm_result);
  }
  else
  {
    ROS_ERROR("Failed to call dynamixel_command");
    return 1;
  }
  return 0;
}
