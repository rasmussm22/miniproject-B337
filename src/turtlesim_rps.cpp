#include <iostream>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Pose.h>
#include <turtlesim/Spawn.h>
#include <cstdlib>
#include <turtlesim/Kill.h>
#include <std_msgs/Int32.h>

geometry_msgs::Twist cmd_vel_message1;
geometry_msgs::Twist cmd_vel_message2;
float velocity_gain = 0.03;
float angular_gain = 0.7;
float battle_distance = 1;
turtlesim::Pose turtle1_Pose;
turtlesim::Pose turtle2_Pose;
ros::NodeHandle *p_node_handle;
int t1 = 0;
int t2 = 0;

void kill_turtle(int n)
{
    std::stringstream ss;
    ss << "turtle" << n;
    turtlesim::Kill kill_message;
    kill_message.request.name = ss.str();
    ros::ServiceClient kill_client = p_node_handle->serviceClient<turtlesim::Kill>("/kill");
    kill_client.call(kill_message);
}

void t1Callback(const std_msgs::Int32::ConstPtr &msg)
{
    t1 = msg->data;

    if (t1 == -1)
    {
        kill_turtle(1);
    }
}