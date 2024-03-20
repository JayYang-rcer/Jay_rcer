# rc_ros技术篇

## 章节一 ROS_Control架构

参考网址：http://wiki.ros.org/ros_control

https://zhuanlan.zhihu.com/p/182417621

### 本架构有如下优点：

1. 调试界面简单直观

![](config/1-1.png)

2.能够在机械没出车的情况下完成代码的初调

3.可以根据不同的硬件开发不同的硬件接口，而且硬件驱动只要写一次，便可以通过简单的配置文件使用对应的硬件

4.代码的复用性极强，控制组的同学可以把更多的精力放在开发新技术上，而不是像单片机一样重新实现旧的功能。

### 我们使用的代码框架：

我们开发的ros代码是基于ros_control进行开发的，数据流图如下：

![](config/1-2.png)

ros_control包将来自机器人执行器编码器的关节状态数据和输入设定点作为输入。它使用通用的控制回路反馈机制，通常是PID控制器，来控制发送到执行器的输出，通常是作用力。对于没有联合位置、努力等一对一映射的物理机制，ros_control变得更加复杂，但这些场景是使用传输来解释的。

## 官方常用的Ros_controller：

- **joint_state_controller**
  
  - 比较特殊，不是用来发指令的，而是用来读关节位置，并且发布在 "/joint_states"话题上。"joint_state_controller/JointStateController"。

- **effort_controllers **驱动器接受力矩指令****
  
  - JointPositionController
    
    - "effort_controllers/JointPositionController". controller接受位置指令，Error=（期望位置-当前位置）。根据误差输出力指令（PID闭环)
  
  - JointVelocityController
    
    - "effort_controllers/JointVelocityController".controller接受速度指令，Error=（期望速度-当前速度）。根据误差输出力指令（PID闭环）。
  
  - JointEffortController
    
    - "effort_controllers/JointEffortController". 接受力矩指令，输出力指令（PID闭环在这里无效）。

- **velocity_controllers **驱动器接受速度指令****
  
  - JointPositionController
    
    - "velocity_controllers/JointPositionController". controller接受位置指令，Error=（期望位置-当前位置）。根据误差输出速度指令（PID闭环）。
  
  - JointVelocityController
    
    - "velocity_controllers/JointVelocityController".接受速度指令，输出速度指令（PID闭环在这里无效）。

- **position_controllers **驱动器接受位置指令****
  
  - JointPositionController
    
    - "position_controllers/JointPositionController". 接受位置指令，输出位置指令（PID闭环在这里无效）。

ros_control包提供了很多控制器（具体可以去看ROS_WIKI）。你可以根据自己的驱动器接受指令的类型来选择。也可以根据需求重写自己的控制器。

## 如何写自己的硬件抽象（HardwareInterface）

HardwareInterface就是ROS_Control与实体机器人之间沟通的桥梁，由上面的数据流图可以看出。而根据硬件抽象的具体功能又可以分为两类：

1. 向实体机器人发送指令--**Interfaces for Joint Actuators**

2. 从机器人的传感器读取机器人状态--**Interfaces for Joint Sensors**

### 1.**Interfaces for Joint Actuators：**

1.1 **EffortJointInterface**：用于接受力矩指令的驱动器。对应使用**effort_controllers**  
1.2 **VelocityJointInterface**：用于接受速度指令的驱动器。对应使用**velocity_controllers**1.3 **PositionJointInterface**：用于接受位置指令的驱动器。对应使用**position_controllers**

### 2.**Interfaces for Joint Sensors:**

2.1 **JointStateInterface**：当关节有 位置 / 速度 / 力 传感器时，用于读取传感器数据，对应使用 **joint_state_controlle**r
2.2 **ImuSensorInterface**：当有IMU传感器时使用，对应的controller为**imu_sensor_controller**

下面我们假设我们需要控制一个二维的机械臂。此时机器人拥有JointA ，JointB两个关节，每个关节都有位置传感器。机械臂支持力控和位置控制，即effort commands可以实现功能。我们只需要写一个头文件和Cpp文件就可以实现：

    MyRobot_hardware_interface.h:

```cpp
#include <hardware_interface/joint_state_interface.h>
#include <hardware_interface/joint_command_interface.h>
#include <hardware_interface/robot_hw.h>
#include <joint_limits_interface/joint_limits.h>
#include <joint_limits_interface/joint_limits_interface.h>
#include <controller_manager/controller_manager.h>
#include <boost/scoped_ptr.hpp>
#include <ros/ros.h>

class MyRobot: public hardware_interface::RobotHW
{
    public:
        MyRobot(ros::NodeHandle& nh);
        ~MyRobot();
        void init();
        void update(const ros::TimerEvent& e);//control loop
        void read();//读取传感器数据
        void write(ros::Duration elapsed_time);//发送关节指令
    protected:
        hardware_interface::JointStateInterface joint_state_interface_;
        hardware_interface::EffortJointInterface effort_joint_interface_;

        joint_limits_interface::JointLimits limits;
        joint_limits_interface::EffortJointSaturationInterface effortJointSaturationInterface;
        joint_limits_interface::PositionJointSaturationInterface positionJointSaturationInterface;
        //读取状态变量
        double joint_position_[2];
        double joint_velocity_[2];
        double joint_effort_[2];
        //指令变量
        double joint_effort_command_[2];
        double joint_position_command_[2];

        ros::NodeHandle nh_;
        ros::Timer my_control_loop_;//控制周期
        ros::Duration elapsed_time_;
        double loop_hz_;
        boost::shared_ptr<controller_manager::ControllerManager> controller_manager_;
}
```

    MyRobot_hardware_interface.cpp:

```cpp
#include <YOUR_PACKAGE_NAME/MYRobot_hardware_interface.h>
​
MyRobot::MyRobot(ros::NodeHandle& nh) : nh_(nh) {

// Declare all JointHandles, JointInterfaces and JointLimitInterfaces of the robot.
    init();

// Create the controller manager
    controller_manager_.reset(new controller_manager::ControllerManager(this, nh_));

//Set the frequency of the control loop.
    loop_hz_=10;
    ros::Duration update_freq = ros::Duration(1.0/loop_hz_);

//Run the control loop
    my_control_loop_ = nh_.createTimer(update_freq, &MyRobot::update, this);
}
​
MyRobot::~MyRobot() {
}
​
void MyRobot::init() {

// Create joint_state_interface for JointA
    hardware_interface::JointStateHandle jointStateHandleA("JointA", &joint_position_[0], &joint_velocity_[0], &joint_effort_[0]);
    joint_state_interface_.registerHandle(jointStateHandleA);
// Create effort joint interface as JointA accepts effort command.
    hardware_interface::JointHandle jointEffortHandleA(jointStateHandleA, &joint_effort_command_[0]);
    effort_joint_interface_.registerHandle(jointEffortHandleA); 
// Create Joint Limit interface for JointA
    joint_limits_interface::getJointLimits("JointA", nh_, limits);
    joint_limits_interface::EffortJointSaturationHandle jointLimitsHandleA(jointEffortHandleA, limits);
    effortJointSaturationInterface.registerHandle(jointLimitsHandleA);    

// Create joint_state_interface for JointB
    hardware_interface::JointStateHandle jointStateHandleB("JointB", &joint_position_[1], &joint_velocity_[1], &joint_effort_[1]);
    joint_state_interface_.registerHandle(jointStateHandleB);
// Create effort joint interface as JointB accepts effort command..
    hardware_interface::JointHandle jointEffortHandleB(jointStateHandleB, &joint_effort_command_[1]);
    effort_joint_interface_.registerHandle(jointEffortHandleB);
// Create Joint Limit interface for JointB
    joint_limits_interface::getJointLimits("JointB", nh_, limits);
    joint_limits_interface::EffortJointSaturationHandle jointLimitsHandleB(jointEffortHandleB, limits);
    effortJointSaturationInterface.registerHandle(jointLimitsHandleB);       

// Register all joints interfaces    
    registerInterface(&joint_state_interface_);
    registerInterface(&effort_joint_interface_);
    registerInterface(&position_joint_interface_);
    registerInterface(&effortJointSaturationInterface);
    registerInterface(&positionJointSaturationInterface);    
}
​
//This is the control loop
void MyRobot::update(const ros::TimerEvent& e) {
    elapsed_time_ = ros::Duration(e.current_real - e.last_real);
    read();
    controller_manager_->update(ros::Time::now(), elapsed_time_);
    write(elapsed_time_);
}
​
void MyRobot::read() {​
  // Write the protocol (I2C/CAN/ros_serial/ros_industrial)used to get the current joint position and/or velocity and/or effort       
  //from robot.
  // and fill JointStateHandle variables joint_position_[i], joint_velocity_[i] and joint_effort_[i]
}
​
void MyRobot::write(ros::Duration elapsed_time) {
  // Safety
  effortJointSaturationInterface.enforceLimits(elapsed_time);   // enforce limits for JointA and JointB
  positionJointSaturationInterface.enforceLimits(elapsed_time); // enforce limits for JointC

  // Write the protocol (I2C/CAN/ros_serial/ros_industrial)used to send the commands to the robot's actuators.
  // the output commands need to send are joint_effort_command_[0] for JointA, joint_effort_command_[1] for JointB and 
  //joint_position_command_ for JointC.
}
​
int main(int argc, char** argv)
{
    //Initialze the ROS node.
    ros::init(argc, argv, "MyRobot_hardware_inerface_node");
    ros::NodeHandle nh;

    //Separate Sinner thread for the Non-Real time callbacks such as service callbacks to load controllers
    ros::MultiThreadedspinner(2); 


    // Create the object of the robot hardware_interface class and spin the thread. 
    MyRobot ROBOT(nh);
    spinner.spin();

    return 0;
}
```

完成了上面这些工作后，我们需要写一些config.yaml文件去描述一下controller和Joint limits，由于我们有位置传感器，故文件如下：

config.yaml

```yaml
MyRobot:

    # Publish all joint states
    joints_update:
      type: joint_state_controller/JointStateController
      publish_rate: 50

    JointA_EffortController:                                               # Name of the controller
      type: effort_controllers/JointPositionController         # Since JointA uses effort interface this controller type is  used      
      joint: JointA                                                                # Name of the joint for which this controller belongs to.
      pid: {p: 100.0, i: 10.0, d: 1.0}                                    # PID values

    JointB_EffortController:                                             
      type: effort_controllers/JointPositionController        # Since JointB uses effort interface this controller type is  used   
      joint: JointB                                                                 
      pid: {p: 1.0, i: 1.0, d: 0.0}
```

joint_limit.yaml

```yaml
joint_limits:

    JointA:
      has_position_limits: true
      min_position: -1.57
      max_position: 1.57
      has_velocity_limits: true
      max_velocity: 1.5
      has_acceleration_limits: false
      max_acceleration: 0.0
      has_jerk_limits: false
      max_jerk: 0
      has_effort_limits: true
      max_effort: 255
​
    JointB:
      has_position_limits: true
      min_position: 0
      max_position: 3.14
      has_velocity_limits: true
      max_velocity: 1.5
      has_acceleration_limits: false
      max_acceleration: 0.0
      has_jerk_limits: false
      max_jerk: 0
      has_effort_limits: true
      max_effort: 255
```

最后一步，我们只要写一个launch文件即可

```yaml
<launch>

  <rosparam file="$(find YOUR_PACKAGE_NAME)/config/controllers.yaml" command="load"/>
  <rosparam file="$(find YOUR_PACKAGE_NAME)/config/joint_limits.yaml" command="load"/>
​
  <node name="MyRobotHardwareInterface" pkg="YOUR_PACKAGE_NAME" type="MyRobot_hardware_inerface_node" output="screen"/>
​
  <node name="robot_state_publisher" pkg="robot_state_publisher" type="state_publisher"/ >

  <node name="controller_spawner" pkg="controller_manager" type="spawner" respawn="false" output="screen"
        args="
            /MyRobot/joints_update
            /MyRobot/JointA_EffortController
            /MyRobot/JointB_EffortController
            /MyRobot/JointC_PositionController
        "/> 
</launch>
```

相信通过上面的说明，你对Ros_control会有更清楚的认识。接下来可以看我们的开源项目进一步了解该架构。

项目地址：https://github.com/gdut-robocon/rc_control

## 第二章：机器人行为树

参考网址：https://www.behaviortree.dev/docs/Intro

行为树，类似于我们之前学习过的状态机，就是在合适时间调用回调函数的机制，传递Tick信号。这本质上与状态机是极其类似的。

行为树在机器人开发方面也取得了不错的进展，适用于重复的，可以搭成模块化的指令和动作，因而常用于机器人导航。如：ROS2 - navigation

#### 下面来简单介绍一下行为树：

    行为树(BT)是一种在自主代理(如机器人或计算机游戏中的虚拟实体)中结构化不同任务之间切换的方法。

    BT 是创建模块化和反应式复杂系统的一种非常有效的方法。这些特性在许多应用中是至关重要的，这导致了 BT 从计算机游戏编程到 AI 和机器人技术的许多分支的传播。

    如果您已经熟悉有限状态机(FSM) ，那么您将很容易掌握大部分概念，但是，希望您会发现 BT 更具表现力，更容易推理。

    将树的节点看作一组构造块。这些块是用 C + + 实现的，并且是“可组合的”: 换句话说，它们可以“组装”来构建行为。

### 行为树的主要优势

- 它们本质上是分层的: 我们可以组合复杂的行为，包括将整棵树作为更大的树的子树。例如，行为“ FetchBeer”可能会重用树“ GraspObject”。

- 它们的图形表示具有语义意义: 更容易“阅读”BT 并理解相应的工作流。通过比较，FSM 中的状态转换在文本和图形表示上都更难理解。

**具体的学习可以参考上面给出的官方网址**，在这里简单介绍一下常用的节点类型：

    **行为节点（Action Node）**：
行为节点表示机器人执行的具体动作或任务，它们是行为树的叶子节点。行为节点通常对应于机器人可以执行的基本操作，例如移动、旋转、抓取物体等。执行行为节点时，机器人会执行相应的动作，并返回执行结果（成功、失败、运行中等）。行为节点是行为树的执行单元，它们负责实际执行机器人的行为。

   **条件节点（Condition Node）：**
条件节点用于评估机器人当前的状态或环境条件，并根据评估结果返回布尔值（真或假）。条件节点通常用于决策树中的分支节点，根据不同的条件结果，选择不同的分支路径。条件节点可以包括简单的传感器检测（例如检查是否有障碍物）或更复杂的逻辑判断（例如评估目标是否可达）。条件节点的评估结果用于控制树的执行流程。

    **控制节点（Control Node）：**
控制节点用于控制行为树的执行流程和控制逻辑。它们可以改变行为节点的执行顺序、控制分支的选择、设定循环和并行执行等。常见的控制节点包括序列节点（Sequence Node）、选择节点（Selector Node）、并行节点（Parallel Node）等。控制节点根据子节点的执行结果来决定下一步的执行路径，并具有灵活的控制机制，使得行为树可以实现复杂的决策逻辑和行为调度。

当然我们直接XML文件也是可以构建树的，但这里我们推荐使用Groot进行树的编写，这是一个能够图形化构建BT树的软件。

![](config/1-3.png)

![](config/1-4.png)

![](config/1-5.png)

如图，我们通过简单改写读取的xml文件或者按照官方的Groot开发调试软件(需按照官方提示规范化xml文件书写格式)。这能让我们在极短时间更改机器人运行逻辑并可以在其中采取许多官方提供的节点，确保程序在遇到异常时能进行重试，保证程序正常运行。

### Visual interface

    Here you can use a remote computer to easily see the situation of robot Such as:Smart Phone,Switch and so on

ref:https://medium.com/teamarimac/groot-with-ros-a8f7855f8e35

我们的项目地址：

    https://github.com/gdut-robocon/rc_decision
