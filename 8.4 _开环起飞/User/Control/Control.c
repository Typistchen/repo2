#include "Control.h"
#include "mpu6050.h"
#include "bsp_led.h"
#include "motor.h" 
#include "steer.h" 
#include "beep.h"
#include "math.h"
#include "pid.h"
#include "MYMATH.h"
#include "IMU.h"
#include "usart3_dma.h"
#include "usart2_dma.h"


 
extern int sbus_adu[7];
extern SI_F_ANGEL  Angel;
extern S16_XYZ Gyro_Filter;
extern void Steer_Set_angle(u8 Steerx,int *angle);

Task_loop_t task_tim={0,0,0,0,0};
//pidInit_t INPitch={		          
//	                 20.0f,     //P
//									 5.0f,      //I
//									 0.00f,     //D
//									 500.0f,    //i_limit
//									 1000.0f    //o_limit
//                }; 

//pidInit_t INRoll={		          
//	                 20.0f,     //P
//									 5.0f,      //I
//									 0.00f,     //D
//									 500.0f,    //i_limit
//									 1000.0f    //o_limit
//                }; 
pidInit_t OUTPitch={		          
	                 20.0f,     //P
									 0.0f,      //I
									 0.00f,     //D
									 500.0f,    //i_limit
									 1000.0f    //o_limit
                  }; 

pidInit_t OUTRoll={		          
	                 40.0f,     //P
									 0.0f,      //I
									 0.00f,     //D
									 500.0f,    //i_limit
									 1000.0f    //o_limit
                  }; 

pidInit_t INYaw={		          
	                 20.0f,     //P
									 0.0f,      //I
									 0.00f,     //D
									 500.0f,    //i_limit
									 1000.0f    //o_limit
                  }; 

	
PidObject outRoll;
PidObject outPitch;
//PidObject inRoll;
//PidObject inPitch;
PidObject inYaw;
									

/*
typedef struct
{
	float desired;			//< set point                                                              void pidInit(PidObject* pid, const float desired, const pidInit_t pidParam, const float dt)                  
	float error;        //< error                                                                  {
	float prevError;    //< previous error                                                         	pid->error     = 0;
	float integ;        //< integral                                                               	pid->prevError = 0;
	float deriv;        //< derivative                                                             	pid->integ     = 0;
	float kp;           //< proportional gain                                                      	pid->deriv     = 0;
	float ki;           //< integral gain                                                          	pid->desired = desired;
	float kd;           //< derivative gain                                                        	pid->kp = pidParam.kp;
	float outP;         //< proportional output (debugging)                                        	pid->ki = pidParam.ki;
	float outI;         //< integral output (debugging)                                            	pid->kd = pidParam.kd;
	float outD;         //< derivative output (debugging)                                          	pid->iLimit = pidParam.i_limit;
	float iLimit;       //< integral limit                                                         	pid->outputLimit = pidParam.o_limit;
	float outputLimit;  //< total PID output limit, absolute value. '0' means no limit.            	pid->dt = dt;
	float dt;           //< delta-time dt                                                          }
	float out;					//< out
} PidObject;

pidInit 和 dt 和 desired 赋值给 pidobject 
最后参与计算的是pidobject
*/							
void control_init(void)
{
	outPitch.desired = 	0.6*(sbus_adu[1] + 50.0) - 30.0;																			          //俯仰的期望值
	outRoll.desired  =  0.3*(sbus_adu[0] + 50.0) - 15.0;																			          //横滚的期望值                                         
	inYaw.desired   =   0.3*(sbus_adu[3] + 50.0) - 15.0;
	
	pidInit(&outRoll,  outRoll.desired , OUTRoll,  0.025);			                                        //PidObject* pid   desired  pidInit_t pidParam  const float dt
	pidInit(&outPitch, outPitch.desired, OUTPitch, 0.002);	
//	pidInit(&inRoll,   0, INPitch, 0.025);			
//	pidInit(&inPitch,  0, OUTRoll, 0.002);
  pidInit(&inYaw,    inYaw.desired, OUTRoll, 0.002);	
}	

/*    
pid算法
pidupdate (&pid_object,pidobject.error)
*/

void pid_task()
{
//	inRoll.error = inRoll.desired - Gyro_Filter.X;																		//内环横滚角误差  期望值-陀螺仪测出的原始值
	outRoll.error = outRoll.desired - Angel.roll ;																			//外环横滚角误差  期望值- IMU解算	+ 遥控器						
//	inPitch.error = inPitch.desired - Gyro_Filter.Y;																	//内环俯仰角误差  期望值-陀螺仪测出的原始值
	outPitch.error = outRoll.desired - Angel.roll ;                        							//外环俯仰角误差  期望值- IMU解算	+ 遥控器	
	inYaw.error = inYaw.desired - Gyro_Filter.Z *1000.0/32768.0 ;	
/*
	PID更新
*/
//	pidUpdate(&inRoll,inRoll.error);
	pidUpdate(&outRoll,outRoll.error);
//	pidUpdate(&inPitch,inPitch.error);
	pidUpdate(&outPitch,outPitch.error);
	pidUpdate(&inYaw,inYaw.error);
}

void Task_2ms(void)
{
//	Steer_Set_angle(Steer0,outRoll.out);                                                //左横滚角
//	Steer_Set_angle(Steer1,3200 - outRoll.out);																						//右横滚角
//	Steer_Set_angle(Steer2,outPitch.out);																								//俯仰角控制
//	Steer_Set_angle(Steer3,(sbus_adu[3]+50)*19 +600);																    //偏航角控制  
//	Motor_Speed_Set(sbus_adu[2]*19);																		           			//电机控制
}

void Task_10ms(int*flag)
{
		if(*flag <= 5)
		{
//			Motor_Speed_Set(0);	
			Steer_Mode();
		}
}

void control_task(void)
{
	 static int flag = 0;
	task_tim.time_1ms++;
	task_tim.time_2ms++;	
	task_tim.time_10ms++;
	
  if(task_tim.time_1ms >= 2)
	{
	  task_tim.time_1ms = 0;
		Get_Angle();
		sbus_re_calculate(sbus_adu);
		pid_task();
		if(sbus_re_calculate(sbus_adu) == 1)
		{
			flag++;
		}
	}	
  if(task_tim.time_2ms >= 4)
	{
	  task_tim.time_2ms -= 4; 
		Task_2ms();
	}
	 if(task_tim.time_10ms >= 20)
	 {
		 task_tim.time_10ms -= 20;
		 Task_10ms(&flag);			 
	 }

}
