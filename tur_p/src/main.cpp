#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Pose.h"

using  namespace std;
void Goal(turtlesim::Pose goal, double distance_tolerance);
double distance(double ox, double  oy, double  gx, double  gy);
void poseCallback(const turtlesim::Pose::ConstPtr & pose_message);

ros::Publisher pub;
ros::Subscriber sub;

turtlesim::Pose p;

double ux,uy,end_m;

int main(int argc, char **argv){
    turtlesim::Pose p;
    ros::init(argc,argv,"Turtle");
    ros::NodeHandle n;
    ros::Rate loop_rate(0.5);
    pub = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel",1000);
    sub = n.subscribe("/turtle1/pose",10,poseCallback);
    ROS_INFO("\nSTARTED PUBLISHER AND SUBSCRIBER");
    do{
    cout<<"enter goal x";
    cin>>ux;
    cout<<"enter goal y";
    cin>>uy;
    p.x =ux;
    p.y=uy;
    p.theta =0;
    Goal(p,0.1);
    cout<<"do you want to give goal again";
    cin>>end_m;
    loop_rate.sleep();
    
}while (end_m == 0);

ros::spin();

}

void poseCallback(const turtlesim::Pose::ConstPtr & pose_message){
	p.x=pose_message->x;
	p.y=pose_message->y;
	p.theta=pose_message->theta;
}

void Goal(turtlesim::Pose goal, double distance_tolerance){
    geometry_msgs::Twist t;
    ros::Rate loop(10);
    do{
    ROS_INFO("moving towards the goal");
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
    t.linear.x = 0;
    t.angular.z= 0;
    pub.publish(t);
    
}

double distance(double ox, double  oy, double  gx, double  gy){
    return sqrt(pow((gx-ox),2) + pow((gy-oy),2));
}

