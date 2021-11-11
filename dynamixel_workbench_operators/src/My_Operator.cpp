#include "dynamixel_workbench_operators/joint_operator.h"
#include <iostream>
#include <vector>
#include <string>

JointOperator::JointOperator()
  :node_handle_(""),
   priv_node_handle_("~"),
   is_loop_(false)
{
  std::string yaml_file = node_handle_.param<std::string>("trajectory_info", "");
  jnt_tra_msg_ = new trajectory_msgs::JointTrajectory;

  bool result = getTrajectoryInfo(yaml_file, jnt_tra_msg_);


  joint_trajectory_pub_ = node_handle_.advertise<trajectory_msgs::JointTrajectory>("joint_trajectory", 100);
  move_command_server_ = node_handle_.advertiseService("execution", &JointOperator::moveCommandMsgCallback, this);

  is_loop_ = priv_node_handle_.param<bool>("is_loop", "false");
}

JointOperator::~JointOperator()
{
}

bool JointOperator::getTrajectoryInfo(const std::string yaml_file, trajectory_msgs::JointTrajectory *jnt_tra_msg)
{

  std::vector<std::string> joint = {"joint1", "joint2"};
  for (uint8_t index = 0; index < joint.size(); index++)
  {
    jnt_tra_msg->joint_names.push_back(joint[index]);
  }

  std::vector<std::string> motion_name = {"motion1","motion2","motion3","motion4","motion5"};
  std::vector<std::vector<double>> motion = {{0.0, 0.0},{1.0, 1.0},{2.0, 2.0},{3.0, 3.0},{2.0, 2.0}};
  std::vector<double> time_from_start = {2.0, 3.0, 4.0, 5.0, 6.0};

  for (uint8_t index = 0; index < motion_name.size(); index++)
  {
    trajectory_msgs::JointTrajectoryPoint jnt_tra_point;

    for (uint8_t size = 0; size < joint.size(); size++)
    {
      if (joint.size() != 2)
      {
        ROS_ERROR("Please check motion step size. It must be equal to joint size");
        return 0;
      }

      jnt_tra_point.positions.push_back(motion[index][size]);

      ROS_INFO("motion_name : %s, step : %f", motion_name[index].c_str(), motion[index][size]);
    }

    jnt_tra_point.time_from_start.fromSec(time_from_start[index]);

    ROS_INFO("time_from_start : %f", time_from_start[index]);

    jnt_tra_msg->points.push_back(jnt_tra_point);
  }

  return true;
}

bool JointOperator::moveCommandMsgCallback(std_srvs::Trigger::Request &req,
                                           std_srvs::Trigger::Response &res)
{
  joint_trajectory_pub_.publish(*jnt_tra_msg_);

  res.success = true;
  res.message = "Success to publish joint trajectory";

  return true;
}

int main(int argc, char **argv)
{
  // Init ROS node
  ros::init(argc, argv, "joint_operator");
  JointOperator joint_operator;

  ROS_INFO("For now, you can use publish joint trajectory msgs by triggering service(/execution)");

  if (1)
  {
    while(1)
    {
      std_srvs::Trigger trig;
      joint_operator.moveCommandMsgCallback(trig.request, trig.response);
    }
  }

  return 0;
}
