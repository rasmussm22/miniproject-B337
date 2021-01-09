#include <iostream>//C++ library for terminal output and input. 
#include <ros/ros.h>//ROS header file including the library, including ros in the code.
#include <geometry_msgs/Twist.h> //Twist header file found in the geometry_msgs library, used to send velocity messages to the turtlesim.
#include <turtlesim/Pose.h> //Pose header in the turltesim file, used to send the position and heading angle of the turtle in turtlesim.
#include <turtlesim/Spawn.h> //Spawn header file in the turtlesim file, to spawn a new turtle.
//C Standard library, remember that we didnt need it if asked, a standard library for C (not C++)
#include <turtlesim/Kill.h> //Kill header in the turtlesim file, to kill a turtle.
#include <std_msgs/Int32.h>//Int32 header in the std_msgs file, used for publishing and subscribing purposes 
// Stringstream (sstring) library, remember that we didnt need it if asked, used to add a char to a string
// string library, We didnt neeeeeed it, used for strings.
#include <turtlesim/TeleportAbsolute.h> //teleport function header in the turtlesim file, used to teleport the turtles.
#include <turtlesim/SetPen.h> // managing the pen in the turtlesim 

geometry_msgs::Twist cmd_vel_message1; 
geometry_msgs::Twist cmd_vel_message2;
//creating 2 variables of type Twist, an message type that contains the velocities of the turtles. 
float velocity_gain = 0.1;
float angular_gain = 0.7;
//velocity for the turtles,
float battle_distance = 1;
//how far the turtles are from each other when they commence the battle
//all floats here are global variables.
turtlesim::Pose turtle1_Pose;
turtlesim::Pose turtle2_Pose;
//position of the turtles, a decleration of the variables of message type that contains the pose of the turtle.
ros::NodeHandle *p_node_handle;
//pointer node handle, so it can be used outside of the main int, we do this to point at the nodehandle so it can be used outside of the main function (globally).
int t1 = 1;
int t2 = 1;
//starting lifes for the turtles (variables). 

void kill_turtle(std::string n)
{
    turtlesim::Kill kill_message;
    //creating a message of type kill
    kill_message.request.name = n;
    //assign the name of the turtle to be killed to the variable of type kill, object of type "request" with the sub object "name".
    //n has the name of the turtle we want to demolish.
    ros::ServiceClient kill_client = p_node_handle->serviceClient<turtlesim::Kill>("/kill");
    //creating a service client of type kill and naming it "/kill"
    //a service client is created with the name kill_client and is initialized with
    //a service client node handle of package name turtlesim::Kill, with a service name defined as /kill
    kill_client.call(kill_message);
    //calling the Service client with the name of kill_client with the object kill_message, murders the turtle
}
//kill command for t//node_handle is basically saying to the node that the code initialazing when you declare the node_handle.ant parameter of type std_msgs::Int32::ConstPointer which is pointing to the message.   

void t1Callback(const std_msgs::Int32::ConstPtr &msg)
{
    t1 = msg->data;
    //assigning the data recieved from t1s life/score puplisher to a varible of type int. 
}

void t2Callback(const std_msgs::Int32::ConstPtr &msg)
{
    t2 = msg->data;
}
//same as above

void turtle1Callback(const turtlesim::Pose::ConstPtr &msg)
{
    turtle1_Pose = *msg;
    //assigning the pose message, * to get the real coordinates not in computer language.

    double x_distance = turtle2_Pose.x - msg->x;
    double y_distance = turtle2_Pose.y - msg->y;
    //calculating a diffirence between x and y
    //initializing 2 varibles of type double 
    double distance = hypot(x_distance, y_distance);
    //hypot function is to return the hypoteneus between the distances.

    if (distance <= battle_distance)
    {
        float velocity_gain = 0.0;
        float angular_gain = 0.0;
        // to stop the turtles

        cmd_vel_message1.linear.x = velocity_gain;
        cmd_vel_message1.angular.z = angular_gain;
        //really stopping the turtle, sending data that they are stopped
    }
    else if (distance > battle_distance)
    {

        double heading = atan2(y_distance, x_distance);
        //atan2 = arctan and the 2 is so it can affect the x and y distance
        //to calculate the heading angle of the turtle. 
        double angular_difference = heading - msg->theta;
        //theta is the orientation of the turtle
        if (angular_difference > M_PI)
        {
            angular_difference = angular_difference - (2 * M_PI);
        }
        //because KARL
        else if (angular_difference < (-M_PI))
        {
            angular_difference = angular_difference + (2 * M_PI);
        }
        cmd_vel_message1.linear.x = velocity_gain * distance;
        cmd_vel_message1.angular.z = angular_gain * angular_difference;
        //assigning the variables so they can be puplished
        //slows down the turtle when they come closer to each other
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
    //intilating ros and creating a rosnode as get_close

    ros::NodeHandle n;
    //node_handle is basically saying to the node that the code initialazing when you declare the node_handle.
    p_node_handle = &n;
    //assigning a refreance to the p_node_handle. 
    srand(time(NULL));
    //seeding the random generator to the time of your computer


    ros::ServiceClient spawn_client = n.serviceClient<turtlesim::Spawn>("/spawn");
    //inisilizing an object of type service client of the package turtlesim::spawn and named /spawn. (reaserch a bit more in spare time) 
    turtlesim::Spawn spawn_message;
    //declaring a message type.
    spawn_message.request.name = "turtle2";
    spawn_message.request.x = rand() % 11;
    spawn_message.request.y = rand() % 11;
    //assigns a random value from 0 - 10. 
    spawn_client.call(spawn_message);
    //spawning the second turtle

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
     //teleporting the first turtle to a random place so both turtles are in a random place
    ros::Publisher cmd_vel_pub1 = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1);
    ros::Publisher cmd_vel_pub2 = n.advertise<geometry_msgs::Twist>("/turtle2/cmd_vel", 1);
     //creating publishers that publishes the pose of the turtles

    ros::Subscriber pose1_subscriber = n.subscribe("/turtle1/pose", 100, &turtle1Callback);
    ros::Subscriber pose2_subscriber = n.subscribe("/turtle2/pose", 100, &turtle2Callback);
    //finding out the pose of the turtles, 100 referce to the que

    ros::Subscriber t1_subscriber = n.subscribe("t1", 1, &t1Callback);
    ros::Subscriber t2_subscriber = n.subscribe("t2", 1, &t2Callback);
    //subscribing to the resault of the rps node 

    ros::Rate loop_rate(10);
    //a 10 hz delay before continuing, for safety
    //ros duration could be used
    while (ros::ok())
    //ros::ok is to cheack if ros is running, nodehandle check
    {
        cmd_vel_pub1.publish(cmd_vel_message1);
        cmd_vel_pub2.publish(cmd_vel_message2);
        //publish the messages
        loop_rate.sleep();
        //using the delay from before, could be removed

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
        //killing the turtle that looses than making the life 1 again so the terminal doesn't get filled with messages, passing a string to the function
        ros::spinOnce();
        //Handles all the subscribers and checks if they are actually are getting messages. 
        //asks ROS to execute all of the pending callbacks from all if the node's subscriptions and then return control back to us.  
    }
    

    return 0;
}
