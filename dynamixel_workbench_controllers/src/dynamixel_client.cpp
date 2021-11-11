#include "dynamixel_workbench_controllers/dynamixel_workbench_controllers.h"
#include <cstdlib>
int main(int argc, char **argv)
{
  ros::init(argc, argv, "dynamixel_client");

  if (argc != 3)
  {
    ROS_INFO("cmd: rosrun dynamixel_client arg0,arg1,arg2");
    ROS_INFO("arg0: unit8_ , arg1: string , arg2 : int32");
    return -1;
  }

  ros::NodeHandle priv_node_handle_("~");
  ros::ServiceClient dynamixel_command_client_ = priv_node_handle_.serviceClient<dynamixel_workbench_msgs::DynamixelCommand>("dynamixel_client");
  // 다이나믹셀 클라이언트 선언

  dynamixel_workbench_msgs::DynamixelCommand srv;
 
  srv.request.command = argv[1];
  srv.request.id = atoi(argv[2]);
  srv.request.addr_name =argv[3];
  srv.request.value = atoi(argv[4]);
  
  if (dynamixel_command_client_.call(srv))
  {
    ROS_INFO("send srv");
  }
  else
  {
    ROS_ERROR("Failed to call service!");
    return -1;
  }
  return 0;

}
