#include "plumbing_ros_32/communica.h"
#include "ros/ros.h"

int main(int argc, char * argv[])
{
    ros::init(argc, argv, "read_from_stm32");

    ros::NodeHandle nh; 
    ros::Rate loop_rate(50);
    SerialInit();

    int16_t RPM1=0,RPM2=0,RPM3=0,RPM4=0;
    float RealAngle_1=0,RealAngle_2=0,RealAngle_3=0,RealAngle_4=0;
    unsigned char crtl=0;

    ROS_INFO("open ok\n");
    while(ros::ok() || ROS_WRITE_TO_STM32)
    {
        ROS_READ_FROM_STM32(RealAngle_1,RealAngle_3,RealAngle_4,crtl);

        ROS_INFO("RPM1 = %d, angle1 = %f", RPM1, RealAngle_1);

        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}

