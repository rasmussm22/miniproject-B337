#include <iostream>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Pose.h>
#include <turtlesim/Spawn.h>
#include <cstdlib>
#include <turtlesim/Kill.h>
#include <std_msgs/Int32.h>
#include <sstream>
#include <string>
#include <turtlesim/TeleportAbsolute.h>
#include <turtlesim/SetPen.h>

geometry_msgs::Twist cmd_vel_message1;
geometry_msgs::Twist cmd_vel_message2;
float velocity_gain = 0.1;
float angular_gain = 0.7;
//velocity for the turtles
float battle_distance = 1;
//how far the turtles are from each other when they commence the battle
turtlesim::Pose turtle1_Pose;
turtlesim::Pose turtle2_Pose;
//position of the turtles
ros::NodeHandle *p_node_handle;
//pointer node handle, so it can be used outside of the main int
int t1 = 1;
int t2 = 1;
//starting lifes for the turtles

void kill_turtle(std::string n)
{
    turtlesim::Kill kill_message;
    kill_message.request.name = n;
    ros::ServiceClient kill_client = p_node_handle->serviceClient<turtlesim::Kill>("/kill");
    kill_client.call(kill_message);
}
//kill command for the turtles

void t1Callback(const std_msgs::Int32::ConstPtr &msg)
{
    t1 = msg->data;
    //assigning the data to the t1 and t2 
}

void t2Callback(const std_msgs::Int32::ConstPtr &msg)
{
    t2 = msg->data;
}

void turtle1Callback(const turtlesim::Pose::ConstPtr &msg)
{
    turtle1_Pose = *msg;

    double x_distance = turtle2_Pose.x - msg->x;
    double y_distance = turtle2_Pose.y - msg->y;
    double distance = hypot(x_distance, y_distance);

    if (distance <= battle_distance)
    {
        float velocity_gain = 0.0;
        float angular_gain = 0.0;

        cmd_vel_message1.linear.x = velocity_gain;
        cmd_vel_message1.angular.z = angular_gain;
    }
    else if (distance > battle_distance)
    {

        double heading = atan2(y_distance, x_distance);
        double angular_difference = heading - msg->theta;
        if (angular_difference > M_PI)
        {
            angular_difference = angular_difference - (2 * M_PI);
        }
        else if (angular_difference < (-M_PI))
        {
            angular_difference = angular_difference + (2 * M_PI);
        }
        cmd_vel_message1.linear.x = velocity_gain * distance;
        cmd_vel_message1.angular.z = angular_gain * angular_difference;
    }
}

void turtle2Callback(const turtlesim::Pose::ConstPtr &msg)
{
    turtle2_Pose = *msg;

    double x_distance = turtle1_Pose.x - msg->x;
    double y_distance = turtle1_Pose.y - msg->y;
    double distance = hypot(x_distance, y_distance);

    if (distance <= battle_distance)
    {
        float velocity_gain = 0.0;
        float angular_gain = 0.0;

        cmd_vel_message2.linear.x = velocity_gain;
        cmd_vel_message2.angular.z = angular_gain;
    }
    else if (distance > battle_distance)
    {
        double heading = atan2(y_distance, x_distance);
        double angular_difference = heading - msg->theta;
        if (angular_difference > M_PI)
        {
            angular_difference = angular_difference - (2 * M_PI);
        }
        else if (angular_difference < (-M_PI))
        {
            angular_difference = angular_difference + (2 * M_PI);
        }
        cmd_vel_message2.linear.x = velocity_gain * distance;
        cmd_vel_message2.angular.z = angular_gain * angular_difference;
    }
}
//movement and also knowing when it is battle range

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "get_close");
    //intilating ros

    ros::NodeHandle n;
    p_node_handle = &n;

    srand(time(NULL));

    ros::ServiceClient spawn_client = n.serviceClient<turtlesim::Spawn>("/spawn");
    turtlesim::Spawn spawn_message;
    spawn_message.request.name = "turtle2";
    spawn_message.request.x = rand() % 11;
    spawn_message.request.y = rand() % 11;
    spawn_client.call(spawn_message);
    //spawnign the second turtle

    ros::ServiceClient pen_client = n.serviceClient<turtlesim::SetPen>("/turtle1/set_pen");
    ros::ServiceClient pen_client2 = n.serviceClient<turtlesim::SetPen>("/turtle2/set_pen");

    turtlesim::SetPen pen_srv;
    pen_srv.request.off = true;
    pen_client.call(pen_srv);
    pen_client2.call(pen_srv);
    //erasing the pen line 

    ros::ServiceClient teleport_client = n.serviceClient<turtlesim::TeleportAbsolute>("/turtle1/teleport_absolute");
    turtlesim::TeleportAbsolute srv;
    srv.request.x = rand() % 11;
    srv.request.y = rand() % 11;
    teleport_client.call(srv);
    ros::Publisher cmd_vel_pub1 = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1);
    ros::Publisher cmd_vel_pub2 = n.advertise<geometry_msgs::Twist>("/turtle2/cmd_vel", 1);
    //teleporting the first turtle to a random place so both turtles are in a random place

    ros::Subscriber pose1_subscriber = n.subscribe("/turtle1/pose", 100, &turtle1Callback);
    ros::Subscriber pose2_subscriber = n.subscribe("/turtle2/pose", 100, &turtle2Callback);
    //finding out the position of the turtles

    ros::Subscriber t1_subscriber = n.subscribe("t1", 1, &t1Callback);
    ros::Subscriber t2_subscriber = n.subscribe("t2", 1, &t2Callback);
    //subscribing to the lives of the turtles

    ros::Rate loop_rate(10);
    while (ros::ok())
    {
        cmd_vel_pub1.publish(cmd_vel_message1);
        cmd_vel_pub2.publish(cmd_vel_message2);
        loop_rate.sleep();

        if (t1 == 0)
        {
            kill_turtle("turtle1");
            t1 = 1;
        }
        else if (t2 == 0)
        {
            kill_turtle("turtle2");
            t2 = 1;
        }
        //killing the turtle that looses than making the life 1 again so the terminal doesn't get filled with messages
        ros::spinOnce();
    }

    return 0;
}
