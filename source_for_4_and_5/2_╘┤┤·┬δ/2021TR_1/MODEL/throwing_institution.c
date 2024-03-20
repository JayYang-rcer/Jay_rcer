#include "includes.h"

CTRL_DES_DEF CTRL_DES = {0, 0, RPM_MIN, 0};
const CTRL_DES_DEF CTRL_RESET = {0, 0, RPM_MIN, 0};

void Throwing_Institution_Control(u8 ID, float yaw_des, float pitch_des, int rpm_des, u8 is_set)
{
		// ÅäÖÃ¿ØÖÆ¶Î
		CanTxMsg tx_message;
		// ÅäÖÃ¿ØÖÆ¶Î
		tx_message.IDE = CAN_Id_Standard;
		tx_message.RTR = CAN_RTR_Data;
		tx_message.StdId = ID;
	
		// ¸´Î»
		if (ID == RESET_TI || ID == SET_TI)
		{
			tx_message.DLC = 0x01;
			CAN_Transmit(CAN2, &tx_message);
			return;
		}
	
		tx_message.DLC = 0x07;
		// ÅäÖÃÊı¾İ¶Î	
		yaw_des = fminf(fmaxf(YAW_MIN, yaw_des), YAW_MAX);
		pitch_des = fminf(fmaxf(PITCH_MIN, pitch_des), PITCH_MAX);

		/// convert floats to unsigned ints ///
		int yaw_int = float_to_uint(yaw_des, YAW_MIN, YAW_MAX, 16);
		int pitch_int = float_to_uint(pitch_des, PITCH_MIN, PITCH_MAX, 16);
		/// pack ints into the can buffer ///
		tx_message.Data[0] = yaw_int>>8;
		tx_message.Data[1] = yaw_int&0xFF;
		tx_message.Data[2] = pitch_int>>8;
		tx_message.Data[3] = pitch_int&0xFF;
		tx_message.Data[4] = rpm_des>>8;
		tx_message.Data[5] = rpm_des&0xFF;
		tx_message.Data[6] = is_set;
		
		CAN_Transmit(CAN2, &tx_message);
}

