#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Pose.h>
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
int t;
cout << "How many turtles are facing off: "; 
cin >> t; // Insert amount of turtles
cout << t << " turtles are facing off";
    return 0;
}
