#include "plumbing_ros_32/communica.h"
#include "ros/ros.h"

int main(int argc, char * argv[])
{
    ros::init(argc, argv, "read_from_stm32");

    ros::NodeHandle nh; 
    ros::Rate loop_rate(50);
    SerialInit();

    int16_t RPM1=0,RPM2=0;
    unsigned char crtl=0;
    float test_float = 0;

    ROS_INFO("open ok\n");
    while(ros::ok() || ROS_WRITE_TO_STM32)
    {
        // ROS_READ_FROM_STM32(RPM1,RPM2,crtl);
        ROS_READ_FROM_STM32_float(RPM1,RPM2,test_float,crtl);

        // ROS_INFO("RPM1 = %d, RPM2 = %d",RPM1,RPM2);
        ROS_INFO("test_float is %f",test_float);

        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}

