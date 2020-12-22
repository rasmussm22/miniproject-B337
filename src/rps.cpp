#include <iostream>
#include <ros/ros.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>  //random generator i think?//
#include <time.h>   //For the random generator//
#include <unistd.h> ///For the sleep//
#include <std_msgs/Int32.h>


int main(int argc, char **argv)
{
    ros::init(argc, argv, "rps");
    ros::NodeHandle n;

    ros::Publisher t1_pub = n.advertise<std_msgs::Int32>("t1", 1);
    ros::Publisher t2_pub = n.advertise<std_msgs::Int32>("t2", 1);

    

    
    std::srand(time(NULL));

    int Turtle1_NumberOfWins = 0;
    int Turtle2_NumberOfWins = 0;
    std::string turtle1;
    std::string turtle2;
    int t1 = 0;
    int t2 = 0;
    bool determine_Looser;

    

    while (Turtle1_NumberOfWins < 3 && Turtle2_NumberOfWins < 3)
    {

        int turtle1_control = rand() % 3 + 1;
        int turtle2_control = rand() % 3 + 1;

        if (turtle1_control == 1)
        {
            turtle1 = "rock";
        }
        else if (turtle1_control == 2)
        {
            turtle1 = "paper";
        }
        else if (turtle1_control == 3)
        {
            turtle1 = "scissor";
        }

        if (turtle2_control == 1)
        {
            turtle2 = "rock";
        }
        else if (turtle2_control == 2)
        {
            turtle2 = "paper";
        }
        else if (turtle2_control == 3)
        {
            turtle2 = "scissor";
        }

        std::cout << "Turtle1 chooses: " << turtle1 << std::endl;
        std::cout << "Turtle2 chooses: " << turtle2 << std::endl;

        ///////Outputting outcome of battle, and counting score///////////

        if (turtle1 == "rock" && turtle2 == "scissor")
        {
            Turtle1_NumberOfWins++;
            std::cout << "Turtle 1 won! Turtle1 chose rock while turtle2 chose scissor" << std::endl;
            std::cout << "Turtle1 gets one point" << std::endl;
        }
        else if (turtle1 == "scissor" && turtle2 == "paper")
        {
            Turtle1_NumberOfWins++;
            std::cout << "Turtle 1 won! Turtle1 chose scissor while turtle2 chose paper" << std::endl;
            std::cout << "Turtle1 gets one point" << std::endl;
        }
        else if (turtle1 == "paper" && turtle2 == "rock")
        {
            Turtle1_NumberOfWins++;
            std::cout << "Turtle 1 won! Turtle1 chose paper while turtle2 chose rock" << std::endl;
            std::cout << "Turtle1 gets one point" << std::endl;
        }
        else if (turtle1 == "rock" && turtle2 == "paper")
        {
            Turtle2_NumberOfWins++;
            std::cout << "Turtle 2 won! Turtle1 chose rock while turtle2 chose paper" << std::endl;
            std::cout << "Turtle2 gets one point" << std::endl;
        }
        else if (turtle1 == "scissor" && turtle2 == "rock")
        {
            Turtle2_NumberOfWins++;
            std::cout << "Turtle 2 won! Turtle1 chose scissor while turtle2 chose rock" << std::endl;
            std::cout << "Turtle2 gets one point" << std::endl;
        }
        else if (turtle1 == "paper" && turtle2 == "scissor")
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

        sleep(2);
    }

    if (Turtle1_NumberOfWins = 3)
    {
        std::cout << "Turtle1 is the champion!" << std::endl;
        determine_Looser = false;
    }
    else if (Turtle2_NumberOfWins = 3)
    {
        std::cout << "Turtle2 is the champion!" << std::endl;
        determine_Looser = true;
    }



    if (determine_Looser = true)
    {
        t1--;
    }
    else if (determine_Looser = false)
    {
        t2--;
    }

    std_msgs::Int32 msg1;
    std_msgs::Int32 msg2;

    msg1.data = t1;
    msg2.data = t2;

    t1_pub.publish(msg1);
    t2_pub.publish(msg2);

    return 0;
}