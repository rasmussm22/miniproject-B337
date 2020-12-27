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
#include <turtlesim/SetPen.h>

geometry_msgs::Twist cmd_vel_message1;
geometry_msgs::Twist cmd_vel_message2;
float velocity_gain = 0.1;
float angular_gain = 0.7;
float battle_distance = 1;
turtlesim::Pose turtle1_Pose;
turtlesim::Pose turtle2_Pose;
ros::NodeHandle *p_node_handle;
int t1 = 1;
int t2 = 1;

void kill_turtle(std::string n)
{
    //std::stringstream ss;
    //ss << "turtle" << n;
    turtlesim::Kill kill_message;
    kill_message.request.name = n;
    ros::ServiceClient kill_client = p_node_handle->serviceClient<turtlesim::Kill>("/kill");
    kill_client.call(kill_message);
}

void t1Callback(const std_msgs::Int32::ConstPtr &msg)
{
    t1 = msg->data;

    std::cout << "I heard from t1Callback " << msg->data << std::endl;

}


void t2Callback(const std_msgs::Int32::ConstPtr &msg)
{
    t2 = msg->data;

    std::cout << "I heard from t2Callback " << msg->data << std::endl;

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

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "get_close");

    ros::NodeHandle n;
    p_node_handle = &n;
    
    srand(time(NULL));

    ros::ServiceClient spawn_client = n.serviceClient<turtlesim::Spawn>("/spawn");
    turtlesim::Spawn spawn_message;
    spawn_message.request.name = "turtle2";
    spawn_message.request.x = rand() % 11;
    spawn_message.request.y = rand() % 11;
    spawn_client.call(spawn_message);

    ros::ServiceClient pen_client = n.serviceClient<turtlesim::SetPen>("/turtle1/set_pen");
    ros::ServiceClient pen_client2 = n.serviceClient<turtlesim::SetPen>("/turtle2/set_pen");
    

     turtlesim::SetPen pen_srv;
     pen_srv.request.off = true;
     pen_client.call(pen_srv);
     pen_client2.call(pen_srv);


    
    ros::Publisher cmd_vel_pub1 = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1);
    ros::Publisher cmd_vel_pub2 = n.advertise<geometry_msgs::Twist>("/turtle2/cmd_vel", 1);

    ros::Subscriber pose1_subscriber = n.subscribe("/turtle1/pose", 100, &turtle1Callback);
    ros::Subscriber pose2_subscriber = n.subscribe("/turtle2/pose", 100, &turtle2Callback);

    ros::Subscriber t1_subscriber = n.subscribe("t1", 1, &t1Callback);
    ros::Subscriber t2_subscriber = n.subscribe("t2", 1, &t2Callback);

    ros::Rate loop_rate(10);
    while (ros::ok())
    {
        cmd_vel_pub1.publish(cmd_vel_message1);
        cmd_vel_pub2.publish(cmd_vel_message2);
        loop_rate.sleep();

        if (t1 == 0)
        {
        kill_turtle("turtle1");
        t1=1;
        }
        else if (t2 == 0)
        {
        kill_turtle("turtle2");
        t2=1;
        }

        ros::spinOnce();
    }

    return 0;
}
