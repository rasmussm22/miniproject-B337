#include <iostream>
#include <ros/ros.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>  
#include <time.h>   
#include <unistd.h> 
#include <std_msgs/Int32.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Pose.h>
#include <turtlesim/Spawn.h>
#include <cstdlib>
#include <turtlesim/Kill.h>


turtlesim::Pose turtle1_Pose;
turtlesim::Pose turtle2_Pose;
int rock = 1;
int paper = 2;
int scissor = 3;

void turtle1Callback(const turtlesim::Pose::ConstPtr &msg)
{
    turtle1_Pose = *msg;
}

void turtle2Callback(const turtlesim::Pose::ConstPtr &msg)
{
    turtle2_Pose = *msg;
}


int getTurtle_choice()
{
    int turtle_choice = rand() % 3 + 1;
    return turtle_choice;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "rps");
    ros::NodeHandle n;

    ros::Rate test(1);

    ros::Subscriber pose1_subscriber = n.subscribe("/turtle1/pose", 100, &turtle1Callback);
    ros::Subscriber pose2_subscriber = n.subscribe("/turtle2/pose", 100, &turtle2Callback);

    ros::Publisher t1_pub = n.advertise<std_msgs::Int32>("t1", 1);
    ros::Publisher t2_pub = n.advertise<std_msgs::Int32>("t2", 1);


    double distance = 0;
    
    test.sleep();

    do
    {


    ros::spinOnce();

    double x_distance = turtle1_Pose.x - turtle2_Pose.x;
    double y_distance = turtle1_Pose.y - turtle2_Pose.y;
    distance = hypot(x_distance, y_distance);


    } while (distance > 1);
    

    std::srand(time(NULL));

    int Turtle1_NumberOfWins = 0;
    int Turtle2_NumberOfWins = 0;
    int turtle1_choice;
    int turtle2_choice;
    int t1 = 1;
    int t2 = 1;

    while (Turtle1_NumberOfWins < 3 && Turtle2_NumberOfWins < 3)
    {

        turtle1_choice = getTurtle_choice();
        turtle2_choice = getTurtle_choice();

        ///////Outputting outcome of battle, and counting score///////////

        if (turtle1_choice == rock && turtle2_choice == scissor)
        {
            Turtle1_NumberOfWins++;
            std::cout << "Turtle 1 won! Turtle1 chose rock while turtle2 chose scissor" << std::endl;
            std::cout << "Turtle1 gets one point" << std::endl;
        }
        else if (turtle1_choice == scissor && turtle2_choice == paper)
        {
            Turtle1_NumberOfWins++;
            std::cout << "Turtle 1 won! Turtle1 chose scissor while turtle2 chose paper" << std::endl;
            std::cout << "Turtle1 gets one point" << std::endl;
        }
        else if (turtle1_choice == paper && turtle2_choice == rock)
        {
            Turtle1_NumberOfWins++;
            std::cout << "Turtle 1 won! Turtle1 chose paper while turtle2 chose rock" << std::endl;
            std::cout << "Turtle1 gets one point" << std::endl;
        }
        else if (turtle1_choice == rock && turtle2_choice == paper)
        {
            Turtle2_NumberOfWins++;
            std::cout << "Turtle 2 won! Turtle1 chose rock while turtle2 chose paper" << std::endl;
            std::cout << "Turtle2 gets one point" << std::endl;
        }
        else if (turtle1_choice == scissor && turtle2_choice == rock)
        {
            Turtle2_NumberOfWins++;
            std::cout << "Turtle 2 won! Turtle1 chose scissor while turtle2 chose rock" << std::endl;
            std::cout << "Turtle2 gets one point" << std::endl;
        }
        else if (turtle1_choice == paper && turtle2_choice == scissor)
        {
            Turtle2_NumberOfWins++;
            std::cout << "Turtle 2 won! Turtle1 chose paper while turtle2 chose scissor" << std::endl;
            std::cout << "Turtle2 gets one point" << std::endl;
        }
        else
        {
            std::cout << "Its a tie! The turtles picked the same thing " << std::endl;
            std::cout << "No points are given" << std::endl;
        }

        std::cout << "Score: Turtle1 = " << Turtle1_NumberOfWins << ", Turtle2 = " << Turtle2_NumberOfWins << std::endl;

        sleep(1.5);
    }

    if (Turtle1_NumberOfWins == 3)
    {
        std::cout << "Turtle1 is the champion!" << std::endl;
        t2--;
    }
    else if (Turtle2_NumberOfWins == 3)
    {
        std::cout << "Turtle2 is the champion!" << std::endl;
        t1--;
    }

    std_msgs::Int32 msg1;
    std_msgs::Int32 msg2;

    msg1.data = t1;
    msg2.data = t2;

    t1_pub.publish(msg1);
    t2_pub.publish(msg2);

    ros::spinOnce();
    

    return 0;
}