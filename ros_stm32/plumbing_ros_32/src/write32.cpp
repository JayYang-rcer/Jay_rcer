#include "plumbing_ros_32/communica.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/String.h"
#include "plumbing_ros_32/r2_controller.h"

plumbing_ros_32::r2_controller controller;
ros::Time last_msg_time;

void doMsg(const geometry_msgs::Twist::ConstPtr &msg_p)
{
    unsigned char crtlFlag = 1;
    ROS_WRITE_TO_STM32(msg_p->linear.x,msg_p->linear.y,msg_p->angular.z,
                    controller.chassis_ctrl_flag,controller.next_bp_state,controller.next_fw_state);
    ROS_INFO("Send chassis cmd successful!\r\n");
}


void control_callback(const plumbing_ros_32::r2_controller::ConstPtr &msg_p)
{
    controller.chassis_ctrl_flag = msg_p->chassis_ctrl_flag;
    controller.next_bp_state = msg_p->next_bp_state;
    controller.next_fw_state = msg_p->next_fw_state;
    last_msg_time = ros::Time::now();   //获取更新上层控制命令的时间戳
}


void timer_callback(const ros::TimerEvent&)
{
    //超时1s，把上层控制命令置0
    if((ros::Time::now() - last_msg_time).toSec() >= 1.0f)
    {
        controller.next_bp_state = 0;
        controller.next_fw_state = 0;
        ROS_INFO("Reset the shangceng Controller state\r\n");
    }
}


int main(int argc, char * argv[])
{
    ros::init(argc, argv, "ros_write_to_stm32");
    setlocale(LC_ALL, "");
    ros::NodeHandle nh;
    ros::Rate loop_rate(50);
    ros::Subscriber chassis_sub = nh.subscribe<geometry_msgs::Twist>("/cmd_vel",10,doMsg);
    ros::Subscriber shangceng_sub = nh.subscribe<plumbing_ros_32::r2_controller>("/r2_control",10,control_callback);

    // 创建定时器,触发的时间间隔为0.5秒
    ros::Timer timer = nh.createTimer(ros::Duration(0.5), timer_callback);
    SerialInit();
    controller.next_bp_state = 0;
    controller.chassis_ctrl_flag = 0;
    controller.next_fw_state = 0;

    ros::spin();
    return 0;
}

