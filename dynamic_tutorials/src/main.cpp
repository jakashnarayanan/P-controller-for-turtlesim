#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Pose.h"
#include <dynamic_reconfigure/server.h>
#include <dynamic_tutorials/TutorialsConfig.h>
using  namespace std;

void Goal(turtlesim::Pose goal, double distance_tolerance);
double distance(double ox, double  oy, double  gx, double  gy);
void poseCallback(const turtlesim::Pose::ConstPtr & pose_message);
void callback(dynamic_tutorials::TutorialsConfig &config, uint32_t level);
ros::Publisher pub;
ros::Subscriber sub;
turtlesim::Pose p;
double ux,uy,end_m;

void poseCallback(const turtlesim::Pose::ConstPtr & pose_message){
	p.x=pose_message->x;
	p.y=pose_message->y;
	p.theta=pose_message->theta;
}

void Goal(turtlesim::Pose goal, double distance_tolerance){
    geometry_msgs::Twist t;
    ros::Rate loop(10);
    ROS_INFO("moving towards the goal");
    do{
    t.linear.x = 0.5*distance(p.x, p.y, goal.x, goal.y);
    t.linear.y =0;
    t.linear.z = 0;
    t.angular.x=0;
    t.angular.y=0;
    t.angular.z= 4*(atan2(goal.y - p.y, goal.x - p.x)-p.theta);
    pub.publish(t);
    ros::spinOnce();
    loop.sleep();
    }while(distance(p.x, p.y, goal.x, goal.y)>distance_tolerance);
    ROS_INFO("Reached goal");
    t.linear.x = 0;
    t.angular.z= 0;
    pub.publish(t);
    
}

double distance(double ox, double  oy, double  gx, double  gy){
    return sqrt(pow((gx-ox),2) + pow((gy-oy),2));
}

void callback(dynamic_tutorials::TutorialsConfig &config, uint32_t level) {
    turtlesim::Pose msg_out;
   msg_out.x = config.goal_x;
   msg_out.y = config.goal_y;
   Goal(msg_out,0.1);

}

int main(int argc, char **argv){
    turtlesim::Pose p;
    ros::init(argc,argv,"Turtle");
    ros::NodeHandle n;
    ros::Rate loop_rate(0.5);
    pub = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel",1000);
    sub = n.subscribe("/turtle1/pose",10,poseCallback);
    dynamic_reconfigure::Server<dynamic_tutorials::TutorialsConfig> server;
    dynamic_reconfigure::Server<dynamic_tutorials::TutorialsConfig>::CallbackType f;
    f = boost::bind(&callback, _1, _2);
    server.setCallback(f);
    ROS_INFO("\nSTARTED PUBLISHER AND SUBSCRIBER");
    Goal(p,0.1);
    ros::spin();

}