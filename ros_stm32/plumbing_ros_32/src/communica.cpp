#include "plumbing_ros_32/communica.h"

using namespace std;
using namespace boost::asio;

const unsigned char serial_header[2] = {0x55,0xaa};
const unsigned char serial_ender[2] = {0x0d,0x0a};

union Recieve_Union0
{
    short data;
    unsigned char tmp_array[2];
}Stm32ToRos_RPM1,Stm32ToRos_RPM2,Stm32ToRos_RPM3,Stm32ToRos_RPM4;

union Recieve_Union1
{
    float data;
    unsigned char tmp_array[4];
}ROBOT_POS_X,ROBOT_POS_Y,ROBOT_POS_YAW;


union Send_Union0
{
    float data;
    unsigned char tmp_array[4];
}RosToStm32_Chassis_X,RosToStm32_Chassis_Y,RosToStm32_Chassis_W;



boost::asio::io_service boost_io_service;
boost::asio::serial_port boost_serial_point = boost::asio::serial_port(boost_io_service,"/dev/ttyUSB0");

void SerialInit(void)
{
    boost_serial_point.set_option(serial_port::baud_rate(115200));
    boost_serial_point.set_option(serial_port::flow_control(serial_port::flow_control::none));
    boost_serial_point.set_option(serial_port::parity(serial_port::parity::none));
    boost_serial_point.set_option(serial_port::stop_bits(serial_port::stop_bits::one));
    boost_serial_point.set_option(serial_port::character_size(8));
}


bool ROS_READ_FROM_STM32(float &p_robot_x, float &p_robot_y, float &p_robot_yaw, unsigned char &crtlFlag)
{
    unsigned char length;   //数据长度
    unsigned char recieve_buf[19] = {0};    
    unsigned char check_value;  //校验位

    try //串口接受数据
    {
        boost::asio::streambuf response;
        boost::system::error_code err;
        boost::asio::read_until(boost_serial_point,response,"\r\n",err);
        copy(istream_iterator<unsigned char>(istream(&response) >> noskipws), istream_iterator<unsigned char>(),recieve_buf);
    }
    catch(const std::exception& err)
    {
        std::cerr << err.what() << '\n';
    }

    //监察信息头
    for (int i = 0; i < 2; i++)
    {
        if(recieve_buf[i] != serial_header[i])
        {
            std::cerr << "read_header_error" << std::endl;
            return false;
        }
    }
    
    //数据长度
    length = recieve_buf[2];
    
    //检查信息校验值
    check_value = serial_get_crc8_value(recieve_buf,length + 3);    //buf[16]
    if(check_value != recieve_buf[3 + length])
    {
        std::cerr << "check_value_error" << std::endl;
        return false;
    }

    for (int i = 0; i < 4; i++)
    {
        ROBOT_POS_X.tmp_array[i] = recieve_buf[i + 3];      //buf[3]~buf[6]
        ROBOT_POS_Y.tmp_array[i] = recieve_buf[i + 7];      //buf[7]~buf[10]
        ROBOT_POS_YAW.tmp_array[i] = recieve_buf[i + 11];   //buf[11]~buf[14]
    }
    

    //读取控制标志位
    crtlFlag = recieve_buf[length+3-1];     //buf[15] length=13

    //读取电机角度
    p_robot_x = ROBOT_POS_X.data; 
    p_robot_y = ROBOT_POS_Y.data;
    p_robot_yaw = ROBOT_POS_YAW.data;

    return true;
}



void ROS_WRITE_TO_STM32(float chassis_x, float chassis_y, float chassis_w, unsigned char chassis_control_flag, 
                        unsigned char BP_ctrl_state, unsigned char FW_ctrl_state)
{
    // 协议数据缓存数组
	unsigned char Buf[21] = {0};
	int i, Length = 0;

    //底盘命令赋值
    RosToStm32_Chassis_X.data = chassis_x;
    RosToStm32_Chassis_Y.data = chassis_y;
    RosToStm32_Chassis_W.data = chassis_w;

    //设置消息头
    for (i = 0; i < 2; i++)
    {
        Buf[i] = serial_header[i];
    }

    Length = 15; //4*3 + 1 = 13
	Buf[2] = Length;    
    for (i = 0; i < 4; i++) //数据填充
    {
        Buf[i+3] = RosToStm32_Chassis_X.tmp_array[i];
        Buf[i+7] = RosToStm32_Chassis_Y.tmp_array[i];
        Buf[i+11] = RosToStm32_Chassis_W.tmp_array[i];
    }

    // 预留控制指令
	Buf[3 + Length - 3] = chassis_control_flag;             //buf[15]   
    Buf[3 + Length - 2] = BP_ctrl_state;                    //buf[16]
    Buf[3 + Length - 1] = FW_ctrl_state;                    //buf[17]
      
    // 设置校验值、消息尾
    Buf[3 + Length] = serial_get_crc8_value(Buf, 3 + Length);	//buf[18]
    Buf[3 + Length + 1] = serial_ender[0];                      //buf[19]
    Buf[3 + Length + 2] = serial_ender[1];                      //buf[20]

    //串口发送数据
    boost::asio::write(boost_serial_point,boost::asio::buffer(Buf));
}




//数据冗余检查
unsigned char serial_get_crc8_value(unsigned char *data, unsigned char len)
{
	unsigned char crc = 0;
	unsigned char i;
	while(len--)
	{
		crc ^= *data++;
		for(i = 0; i < 8; i++)
		{
			if(crc&0x01)
				crc=(crc>>1)^0x8C;
			else
				crc >>= 1;
		}
	}
	return crc;
}
