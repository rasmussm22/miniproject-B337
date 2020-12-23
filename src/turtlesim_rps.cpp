#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Pose.h>
#include <turtlesim/Spawn.h>
#include <iostream>
#include <cstdlib>


using namespace std;
ros::NodeHandle * p_node_handle;

// Spawn_turtle snippet from kill turtle start
void spawn_turtle(int n)
{
    ros::ServiceClient spawn_client = 
    p_node_handle->serviceClient<turtlesim::Spawn>("/spawn");
    turtlesim::Spawn spawn_message;
    spawn_message.request.name = "turtle2";
    spawn_message.request.x = rand() % 11;
    spawn_message.request.y = rand() % 11;
    spawn_client.call(spawn_message);
}
// Snippet end

int main(int argc, char *argv[])
{
int t;
cout << "How many turtles are facing off: "; 
cin >> t; // Insert amount of turtles
cout << t << " turtles are facing off";
ros::init( argc, argv, "turtle_spawn" );
ros::NodeHandle node_handle;
p_node_handle =&node_handle;
ros::Duration(1.0).sleep();
spawn_turtle(t+1);

    return 0;
}
