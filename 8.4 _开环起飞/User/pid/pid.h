#ifndef __PID_H
#define __PID_H
#include <stdbool.h>


typedef struct 
{
	float kp;
	float ki;
	float kd;
	float i_limit;
	float o_limit;
	float o_max;
	
} pidInit_t;


typedef struct
{
	float desired;			//< set point
	float error;        //< error
	float prevError;    //< previous error
	float integ;        //< integral
	float deriv;        //< derivative
	float kp;           //< proportional gain
	float ki;           //< integral gain
	float kd;           //< derivative gain
	float outP;         //< proportional output (debugging)
	float outI;         //< integral output (debugging)
	float outD;         //< derivative output (debugging)
	float iLimit;       //< integral limit
	float outputLimit;  //< total PID output limit, absolute value. '0' means no limit.
	float outputMax;
	float dt;           //< delta-time dt
	float out;					//< out
} PidObject;


/*pid结构体初始化*/
void pidInit(PidObject* pid, const float desired, const pidInit_t pidParam, const float dt);
float pidUpdate(PidObject* pid, const float error);			/*pid更新*/
void pidReset(PidObject* pid);			/*pid结构体复位*/


#endif /* __PID_H */

