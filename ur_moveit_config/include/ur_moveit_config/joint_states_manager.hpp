/**
 * @file joint_states_manager.hpp
 * @brief 両腕のJointStateを受信・統合し、ROS2でpublishするクラスの宣言
 */
#pragma once
#include <memory>
#include <mutex>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>

class JointStatesManager : public rclcpp::Node {
public:
  /**
   * @brief コンストラクタ
   * @details 両腕のJointStateを受信し、統合してROS2でpublishするノードを初期化します。
   */
  JointStatesManager();

private:
  /**
   * @brief 両腕のJointStateをマージして1つのJointStateにする
   * @return マージされたJointStateのポインタ
   */
  std::shared_ptr<sensor_msgs::msg::JointState> create_merged_joint_state();

  /**
   * @brief 定期的にマージされたJointStateをpublishする
   */
  void joint_states_publisher();

  // パブリッシャ
  rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr publisher_;
  // 両腕のJointStateを受信するサブスクライバ
  rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr right_subscriber_;
  rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr left_subscriber_;
  
  // 100ms周期でpublishするためのタイマー
  rclcpp::TimerBase::SharedPtr joint_states_pub_timer_;

  // 両腕のJointStateを保持するポインタ
  sensor_msgs::msg::JointState::SharedPtr right_state_;
  sensor_msgs::msg::JointState::SharedPtr left_state_;
  
  std::mutex mutex_;
};
