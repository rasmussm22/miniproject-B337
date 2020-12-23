#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Pose.h>
#include <turtlesim/Spawn.h>
#include <iostream>
#include <cstdlib>


using namespace std;
ros::NodeHandle * p_node_handle;

// Spawn_turtle snippet from kill turtle start
void spawn_turtle(char n)
{
    ros::ServiceClient spawn_client = 
    p_node_handle->serviceClient<turtlesim::Spawn>("/spawn");
    turtlesim::Spawn spawn_message;
    spawn_message.request.name = "";
    spawn_message.request.x = rand() % 11;
    spawn_message.request.y = rand() % 11;
    spawn_client.call(spawn_message);
}
// Snippet end
char t;
char yeet=0;
int main(int argc, char *argv[]){

//char t;
//char yeet =0;

cout << "How many turtles are facing off: "; 
cin >> t; // Insert amount of turtles
cout << t << " turtles are facing off";

// don't know don't care
ros::init( argc, argv, "turtle_spawn" );
ros::NodeHandle node_handle;
p_node_handle =&node_handle;

// My cheeky way to get around 
// spawn turtle not working x amount of times
while(yeet <t){
spawn_turtle(1);
yeet++;
}
    return 0;
}
