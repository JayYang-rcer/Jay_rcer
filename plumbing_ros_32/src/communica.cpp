#include "plumbing_ros_32/communica.h"

using namespace std;
using namespace boost::asio;

const unsigned char serial_header[2] = {0x55,0xaa};
const unsigned char serial_ender[2] = {0x0d,0x0a};

union Recieve_Union0
{
    short data;
    unsigned char tmp_array[2];
}Stm32ToRos_RPM1,Stm32ToRos_RPM2;


union Send_Union0
{
    short data;
    unsigned char tmp_array[2];
}RosToStm32_RPM1,RosToStm32_RPM2;


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


bool ROS_READ_FROM_STM32(int16_t &p_RPM1, int16_t &p_RPM2, unsigned char &crtlFlag)
{
    unsigned char length;
    unsigned char recieve_buf[150] = {0};
    unsigned char check_value;

    try
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
    check_value = serial_get_crc8_value(recieve_buf,length + 3);
    if(check_value != recieve_buf[3 + length])
    {
        std::cerr << "check_value_error" << std::endl;
        return false;
    }

    //读取速度值
    for(int i = 0; i < 2; i++)
    {
        Stm32ToRos_RPM1.tmp_array[i] = recieve_buf[i + 3];
        Stm32ToRos_RPM2.tmp_array[i] = recieve_buf[i + 5];
    }

    //读取控制标志位
    crtlFlag = recieve_buf[7];

    p_RPM1 = Stm32ToRos_RPM1.data;
    p_RPM2 = Stm32ToRos_RPM2.data;

    return true;
}


void ROS_WRITE_TO_STM32(short target_RPM1, short target_RPM2, unsigned char crtlFlag)
{
    // 协议数据缓存数组
	unsigned char Buf[11] = {0};
	int i, Length = 0;

    //赋值两个电机的转子转速
    RosToStm32_RPM1.data = target_RPM1;
    RosToStm32_RPM2.data = target_RPM2;

    //设置消息头
    for (i = 0; i < 2; i++)
    {
        Buf[i] = serial_header[i];
    }

    Length = 5; //2 + 2 + 1 = 5
	Buf[2] = Length;    
    for (i = 0; i < 2; i++)
    {
        Buf[i+3] = RosToStm32_RPM1.tmp_array[i];
        Buf[i+5] = RosToStm32_RPM2.tmp_array[i];
    }

    // 预留控制指令
	Buf[3 + Length - 1] = crtlFlag;              // buf[7]
      
    // 设置校验值、消息尾
    Buf[3 + Length] = serial_get_crc8_value(Buf, 3 + Length);	//buf[8]
    Buf[3 + Length + 1] = serial_ender[0];
    Buf[3 + Length + 2] = serial_ender[1];

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
