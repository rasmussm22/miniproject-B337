#include <iostream>
#include <ros/ros.h>
#include <turtlesim/Pose.h>

int getTurtle1Choice()
{

    int turtle1Choice = rand() % 3 + 1;
    return turtle1Choice;
}

int getTurtle2Choice()
{

    int turtle2Choice = rand() % 3 + 1;
    return turtle2Choice;
}

bool determineWinner(int getTurtle1Choice, int getTurtle2Choice)
{
    bool tie = false;

    const int rock = 1;
    const int paper = 2;
    const int scissor = 3;

    if (getTurtle1Choice == rock && getTurtle2Choice == paper)
    {
        std::cout << "turtle2 Wins! Paper sufficates rock!" << std::endl;
    }
    else if (getTurtle2Choice == rock && getTurtle1Choice == paper)
    {
        std::cout << "turtle1 Wins! Paper sufficates rock!" << std::endl;
    }
    else if (getTurtle1Choice == scissor && getTurtle2Choice == paper)
    {
        std::cout << "turtle1 Wins! Scissor sepirates paper leaving it in 2!" << std::endl;
    }
    else if (getTurtle2Choice == scissor && getTurtle1Choice == paper)
    {
        std::cout << "turtle2 Wins! Scissor sepirates paper leaving it in 2!" << std::endl;
    }
    else if (getTurtle1Choice == rock && getTurtle2Choice == scissor)
    {
        std::cout << /*insert turtle name function*/ "Wins! Rock crushes the scissors into a pulp!" << std::endl;
    }
    else if (getTurtle2Choice == rock && getTurtle1Choice == scissor)
    {
        std::cout << /*insert turtle name function*/ "Wins! Rock crushes the scissors into a pulp!" << std::endl;
    }
    else
    {
        std::cout << "Tie" << std::endl;
        tie = true;
    }

    return tie;
}

int main(int argc, char **argv)
{
    srand(time(NULL));

    std::cout << /*insert turtle name function*/ "and" /*insert turtlename function2*/ "have comenced battle" << std::endl;

    bool tie;

    do
    {
        tie = determineWinner(getTurtle1Choice(), getTurtle2Choice());
    } while (tie == true);

    return 0;
}
