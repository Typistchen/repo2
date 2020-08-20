#include "IMU.h"

S16_XYZ Acc_Raw;
S16_XYZ Gyro_Raw;
S16_XYZ Acc_Filter;
S16_XYZ Gyro_Filter;
SI_F_XYZ  SI_gyro;
//S16_XYZ  Gyro_Adu;
//S16_XYZ  Acc_Adu;

SI_F_ANGEL  Angel;

float Sin_Pitch=0,Sin_Roll=0,Sin_Yaw=0;
float Cos_Pitch=0,Cos_Roll=0,Cos_Yaw=0;

#define Kp 	2.0f    // ��������
#define Ki 	0.001f  // ���ֳ���
#define halfT 0.001f//������
#define T	0.002f  // ����Ϊ2ms
// ==================================================================================
// ��������
float q0 = 1, q1 = 0, q2 = 0, q3 = 0;     	// ��Ԫ��
float exInt = 0, eyInt = 0, ezInt = 0;    	// �������ۼ�ֵ 


void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az) 
{
	float norm;
	float vx, vy, vz;
	float ex, ey, ez;
	
  //��Ԫ�����֣���õ�ǰ����̬
	float q0_last = q0;	
	float q1_last = q1;	
	float q2_last = q2;	
	float q3_last = q3;	

	//�Ѽ��ٶȼƵ���ά����ת�ɵ�λ����
	norm = invSqrt(ax*ax + ay*ay + az*az);
	ax = ax * norm;
	ay = ay * norm;
	az = az * norm;

	//�����������ٶȷ����ڷ���������ϵ�еı�ʾ��Ϊ��Ԫ����ʾ����ת����ĵ�����
	vx = 2*(q1*q3 - q0*q2);
	vy = 2*(q0*q1 + q2*q3);
	vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;

	//���ٶȼƶ�ȡ�ķ������������ٶȷ���Ĳ�ֵ����������˼���
	ex = ay*vz - az*vy;
	ey = az*vx - ax*vz;
	ez = ax*vy - ay*vx;

	//����ۻ���������ֳ������
	exInt = exInt + ex*Ki;
	eyInt = eyInt + ey*Ki;
	ezInt = ezInt + ez*Ki;

	//�ò���������PI����������ƫ�����������ݶ����е�ƫ����	
	gx = gx + Kp*ex + exInt;
	gy = gy + Kp*ey + eyInt;
	gz = gz + Kp*ez + ezInt;


	//һ�׽����㷨
	q0 = q0_last + (-q1_last*gx - q2_last*gy - q3_last*gz)*halfT;
	q1 = q1_last + ( q0_last*gx + q2_last*gz - q3_last*gy)*halfT;
	q2 = q2_last + ( q0_last*gy - q1_last*gz + q3_last*gx)*halfT;
	q3 = q3_last + ( q0_last*gz + q1_last*gy - q2_last*gx)*halfT; 

//	//���׽����㷨
//	float delta2 = (gx*gx + gy*gy + gz*gz)*T*T;
//	q0 = q0_last*(1-delta2/8) + (-q1_last*gx - q2_last*gy - q3_last*gz)*halfT;
//	q1 = q1_last*(1-delta2/8) + ( q0_last*gx + q2_last*gz - q3_last*gy)*halfT;
//	q2 = q2_last*(1-delta2/8) + ( q0_last*gy - q1_last*gz + q3_last*gx)*halfT;
//	q3 = q3_last*(1-delta2/8) + ( q0_last*gz + q1_last*gy - q2_last*gx)*halfT;

//	//���׽����㷨
//	float delta2 = (gx*gx + gy*gy + gz*gz)*T*T;
//	q0 = q0_last*(1-delta2/8) + (-q1_last*gx - q2_last*gy - q3_last*gz)*T*(0.5 - delta2/48);
//	q1 = q1_last*(1-delta2/8) + ( q0_last*gx + q2_last*gz - q3_last*gy)*T*(0.5 - delta2/48);
//	q2 = q2_last*(1-delta2/8) + ( q0_last*gy - q1_last*gz + q3_last*gx)*T*(0.5 - delta2/48);
//	q3 = q3_last*(1-delta2/8) + ( q0_last*gz + q1_last*gy - q2_last*gx)*T*(0.5 - delta2/48);

//	//�Ľ׽����㷨
//	float delta2 = (gx*gx + gy*gy + gz*gz)*T*T;
//	q0 = q0_last*(1 - delta2/8 + delta2*delta2/384) + (-q1_last*gx - q2_last*gy - q3_last*gz)*T*(0.5 - delta2/48);
//	q1 = q1_last*(1 - delta2/8 + delta2*delta2/384) + ( q0_last*gx + q2_last*gz - q3_last*gy)*T*(0.5 - delta2/48);
//	q2 = q2_last*(1 - delta2/8 + delta2*delta2/384) + ( q0_last*gy - q1_last*gz + q3_last*gx)*T*(0.5 - delta2/48);
//	q3 = q3_last*(1 - delta2/8 + delta2*delta2/384) + ( q0_last*gz + q1_last*gy - q2_last*gx)*T*(0.5 - delta2/48);
			
	//��Ԫ���淶��
	norm = invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	q0 = q0 * norm;
	q1 = q1 * norm;
	q2 = q2 * norm;
	q3 = q3 * norm;
	

}


void Get_Eulerian_Angle(SI_F_ANGEL *angle)
{	
	angle->roll		= -atan2(2.0f*(q0*q1 + q2*q3),q0*q0 - q1*q1 - q2*q2 + q3*q3)*Radian_to_Angle;
	angle->pitch  =  asin (2.0f*(q0*q2 - q1*q3))*Radian_to_Angle;
	if((Gyro_Filter.Z<20)&&(Gyro_Filter.Z>-20))
	{
			Gyro_Filter.Z = 0;	
	}
	angle->yaw  +=  Gyro_Filter.Z * RawData_to_Angle * T;

}

void Get_Angle_Sin(void)
{
	Sin_Pitch=sin(Angel.pitch* Angle_to_Radian);
  Cos_Pitch=cos(Angel.pitch* Angle_to_Radian);
  Sin_Roll=sin(Angel.roll* Angle_to_Radian);
  Cos_Roll=cos(Angel.roll* Angle_to_Radian);
  Sin_Yaw=sin(Angel.yaw* Angle_to_Radian);
  Cos_Yaw=cos(Angel.yaw* Angle_to_Radian);	
}


void Get_Angle(void)
{
  MPU_Get_Accelerometer(&Acc_Raw.X,&Acc_Raw.Y,&Acc_Raw.Z);	
	MPU_Get_Gyroscope(&Gyro_Raw.X,&Gyro_Raw.Y,&Gyro_Raw.Z);
	ACC_IIR_Filter(&Acc_Raw,&Acc_Filter);
	Gyro_Slid_Filter(&Gyro_Raw,&Gyro_Filter);
	Get_Radian(&Gyro_Filter,&SI_gyro);
	IMUupdate(SI_gyro.X,SI_gyro.Y,SI_gyro.Z,Acc_Filter.X,Acc_Filter.Y,Acc_Filter.Z);
  Get_Eulerian_Angle(&Angel);
}

