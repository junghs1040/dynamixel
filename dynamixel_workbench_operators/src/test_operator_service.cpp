JointOperator::JointOperator()
  :node_handle_(""),
   priv_node_handle_("~")
{
  std::string yaml_file = node_handle_.param<std::string>("trajectory_info", "");
  jnt_tra_msg_ = new trajectory_msgs::JointTrajectory;

  bool result = getTrajectoryInfo(yaml_file, jnt_tra_msg_);
  if (result == false)
  {
    ROS_ERROR("Please check YAML file");
    exit(0);
  }

  joint_trajectory_pub_ = node_handle_.advertise<trajectory_msgs::JointTrajectory>("joint_trajectory", 100);
  move_command_server_ = node_handle_.advertiseService("execution", &JointOperator::moveCommandMsgCallback, this);

}

JointOperator::~JointOperator()
{
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

  joint_trajectory_pub_ = node_handle_.advertise<trajectory_msgs::JointTrajectory>("joint_trajectory", 100);

  std_srvs::Trigger trig;
  joint_operator.moveCommandMsgCallback(trig.request, trig.response);
  

  ros::spin();

  return 0;
}
