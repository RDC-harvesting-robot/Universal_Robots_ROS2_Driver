
#pragma once
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <memory>

class JointStatesManager : public rclcpp::Node
{
public:
  JointStatesManager();

private:
  void publish_merged();

  rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr publisher_;
  rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr right_subscriber_;
  rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr left_subscriber_;

  sensor_msgs::msg::JointState::SharedPtr right_state_;
  sensor_msgs::msg::JointState::SharedPtr left_state_;
};
