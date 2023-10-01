#include "plumbing_ros_32/communica.h"

int main(int argc, char * argv[])
{
    ros::init(argc, argv, "ros_write_to_stm32");
    ros::NodeHandle nh;
    ros::Rate loop_rate(50);
    setlocale(LC_ALL, "");
    SerialInit();

    int16_t target_RPM1 = 200, target_RPM2 = 200;
    unsigned char crtlFlag = 1;

    while(ros::ok())
    {
        ROS_WRITE_TO_STM32(target_RPM1,target_RPM2,crtlFlag);
        ROS_INFO("发送成功 RPM1: %d, RPM2: %d",target_RPM1,target_RPM2);

        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}

