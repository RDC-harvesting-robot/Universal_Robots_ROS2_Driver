
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>

class JointStatesManager : public rclcpp::Node
{
public:
  JointStatesManager()
  : Node("joint_states_manager_node")
  {
    publisher_ = this->create_publisher<sensor_msgs::msg::JointState>("/joint_states", 10);

    right_subscriber_ = this->create_subscription<sensor_msgs::msg::JointState>(
      "/right_arm/joint_states", 10,
      [this](sensor_msgs::msg::JointState::SharedPtr msg) {
        right_state_ = msg;
        publish_merged();
      });

    left_subscriber_ = this->create_subscription<sensor_msgs::msg::JointState>(
      "/left_arm/joint_states", 10,
      [this](sensor_msgs::msg::JointState::SharedPtr msg) {
        left_state_ = msg;
        publish_merged();
      });
  }

private:
  void publish_merged()
  {
    if (!right_state_ || !left_state_) return;

    auto merged = std::make_shared<sensor_msgs::msg::JointState>();
    merged->header.stamp = this->get_clock()->now();

    merged->name.insert(merged->name.end(), right_state_->name.begin(), right_state_->name.end());
    merged->name.insert(merged->name.end(), left_state_->name.begin(), left_state_->name.end());

    merged->position.insert(merged->position.end(), right_state_->position.begin(), right_state_->position.end());
    merged->position.insert(merged->position.end(), left_state_->position.begin(), left_state_->position.end());

    merged->velocity.insert(merged->velocity.end(), right_state_->velocity.begin(), right_state_->velocity.end());
    merged->velocity.insert(merged->velocity.end(), left_state_->velocity.begin(), left_state_->velocity.end());

    merged->effort.insert(merged->effort.end(), right_state_->effort.begin(), right_state_->effort.end());
    merged->effort.insert(merged->effort.end(), left_state_->effort.begin(), left_state_->effort.end());

    publisher_->publish(*merged);
  }

  rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr publisher_;
  rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr right_subscriber_;
  rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr left_subscriber_;

  sensor_msgs::msg::JointState::SharedPtr right_state_;
  sensor_msgs::msg::JointState::SharedPtr left_state_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<JointStatesManager>());
  rclcpp::shutdown();
  return 0;
}

