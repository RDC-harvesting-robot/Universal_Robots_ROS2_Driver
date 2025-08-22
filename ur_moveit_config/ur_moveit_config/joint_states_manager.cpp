
#include <ur_moveit_config/joint_states_manager.hpp>


JointStatesManager::JointStatesManager()
    : Node("joint_states_manager_node") {
  publisher_ = this->create_publisher<sensor_msgs::msg::JointState>("/joint_states", 10);

  right_subscriber_ = this->create_subscription<sensor_msgs::msg::JointState>(
      "/right_arm/joint_states", 10,
      [this](sensor_msgs::msg::JointState::SharedPtr msg) {
        std::lock_guard<std::mutex> lock(mutex_);
        right_state_ = msg;
      });

  left_subscriber_ = this->create_subscription<sensor_msgs::msg::JointState>(
      "/left_arm/joint_states", 10,
      [this](sensor_msgs::msg::JointState::SharedPtr msg) {
        std::lock_guard<std::mutex> lock(mutex_);
        left_state_ = msg;
      });

  joint_states_pub_timer_ = this->create_wall_timer(
      std::chrono::milliseconds(100),
      std::bind(&JointStatesManager::joint_states_publisher, this));
}




std::shared_ptr<sensor_msgs::msg::JointState> JointStatesManager::create_merged_joint_state() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (!right_state_ || !left_state_)
    return nullptr;

  auto merged_state = std::make_shared<sensor_msgs::msg::JointState>();
  merged_state->header.stamp = this->get_clock()->now();

  merged_state->name.reserve(right_state_->name.size() + left_state_->name.size());
  merged_state->name.insert(merged_state->name.end(), right_state_->name.begin(), right_state_->name.end());
  merged_state->name.insert(merged_state->name.end(), left_state_->name.begin(), left_state_->name.end());

  merged_state->position.reserve(right_state_->position.size() + left_state_->position.size());
  merged_state->position.insert(merged_state->position.end(), right_state_->position.begin(), right_state_->position.end());
  merged_state->position.insert(merged_state->position.end(), left_state_->position.begin(), left_state_->position.end());

  merged_state->velocity.reserve(right_state_->velocity.size() + left_state_->velocity.size());
  merged_state->velocity.insert(merged_state->velocity.end(), right_state_->velocity.begin(), right_state_->velocity.end());
  merged_state->velocity.insert(merged_state->velocity.end(), left_state_->velocity.begin(), left_state_->velocity.end());

  merged_state->effort.reserve(right_state_->effort.size() + left_state_->effort.size());
  merged_state->effort.insert(merged_state->effort.end(), right_state_->effort.begin(), right_state_->effort.end());
  merged_state->effort.insert(merged_state->effort.end(), left_state_->effort.begin(), left_state_->effort.end());

  return merged_state;
}


void JointStatesManager::joint_states_publisher() {
  auto merged_state = create_merged_joint_state();
  if (merged_state) {
    publisher_->publish(*merged_state);
  }
  else {
    RCLCPP_WARN(this->get_logger(), "Joint Statesがまだ受信されていません");
  }
}


int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<JointStatesManager>());
  rclcpp::shutdown();
  return 0;
}

