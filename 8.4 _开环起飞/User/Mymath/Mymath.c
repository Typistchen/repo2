#include "MYMATH.h"
#define LIMIT( x,min,max ) ( (x) < (min)  ? (min) : ( (x) > (max) ? (max) : (x) ) )
void Get_Radian(S16_XYZ *Gyro_in,SI_F_XYZ *Gyro_out)
{
	Gyro_out->X = (float)(Gyro_in->X * RawData_to_Radian);
	Gyro_out->Y = (float)(Gyro_in->Y * RawData_to_Radian);
	Gyro_out->Z = (float)(Gyro_in->Z * RawData_to_Radian);
}

// Fast inverse square-root
// See: http://en.wikipedia.org/wiki/Fast_inverse_square_root
// 快速计算开根号的倒数
float invSqrt(float x)
{
  float halfx = 0.5f * x;
  float y = x;
  long i = *(long*)&y;
  i = 0x5f3759df - (i>>1);
  y = *(float*)&i;
  y = y * (1.5f - (halfx * y * y));
  return y;
}

static float ACC_IIR_FACTOR;
void Calculate_FilteringCoefficient(float Time, float Cut_Off)
{
	ACC_IIR_FACTOR = Time /( Time + 1/(2.0f*Pi*Cut_Off) );
}

/******************************************************************************
函数原型：	void ACC_IIR_Filter(struct _acc *Acc_in,struct _acc *Acc_out)
功    能：	iir低通滤波
*******************************************************************************/ 
	s32 accx=0;
	s32 accy=0;
	s32 accz=0;
extern S16_XYZ Acc_Raw;
extern S16_XYZ Acc_Filter;
extern S16_XYZ  Acc_Adu;
void ACC_IIR_Filter(S16_XYZ *Acc_in,S16_XYZ *Acc_out)
{
	
//	Acc_out->X = Acc_out->X + ACC_IIR_FACTOR*(Acc_in->X - Acc_out->X - Acc_Adu.X) ; 
//	Acc_out->Y = Acc_out->Y + ACC_IIR_FACTOR*(Acc_in->Y - Acc_out->Y - Acc_Adu.Y) ; 
//	Acc_out->Z = Acc_out->Z + ACC_IIR_FACTOR*(Acc_in->Z - Acc_out->Z - Acc_Adu.Z) ; 
	
	Acc_out->X = Acc_out->X + ACC_IIR_FACTOR*(Acc_in->X - Acc_out->X - 0) ; 
	Acc_out->Y = Acc_out->Y + ACC_IIR_FACTOR*(Acc_in->Y - Acc_out->Y + 0) ; 
	Acc_out->Z = Acc_out->Z + ACC_IIR_FACTOR*(Acc_in->Z - Acc_out->Z + 0) ; 
	
}

#define Filter_Num 8
extern S16_XYZ  Gyro_Adu;
/******************************************************************************
函数原型：	void Gyro_Filter(struct _gyro *Gyro_in,struct _gyro *Gyro_out)
功    能：	gyro窗口滑动滤波
*******************************************************************************/ 
void Gyro_Slid_Filter(S16_XYZ *Gyro_in,S16_XYZ *Gyro_out)
{
	static float Filter_x[Filter_Num],Filter_y[Filter_Num],Filter_z[Filter_Num];
	static uint8_t Filter_count  = 0;
	double Filter_sum_x=0,Filter_sum_y=0,Filter_sum_z=0;
	uint8_t i=0;
	
	Filter_x[Filter_count] = Gyro_in->X;
	Filter_y[Filter_count] = Gyro_in->Y;
	Filter_z[Filter_count] = Gyro_in->Z;

	for(i=0;i<Filter_Num;i++)
	{
		Filter_sum_x += Filter_x[i];
		Filter_sum_y += Filter_y[i];
		Filter_sum_z += Filter_z[i];
	}	
	
//	Gyro_out->X = Filter_sum_x / Filter_Num - Gyro_Adu.X;
//	Gyro_out->Y = Filter_sum_y / Filter_Num - Gyro_Adu.Y;
//	Gyro_out->Z = Filter_sum_z / Filter_Num - Gyro_Adu.Z;
	
	Gyro_out->X = Filter_sum_x / Filter_Num + 305;
	Gyro_out->Y = Filter_sum_y / Filter_Num - 78;
	Gyro_out->Z = Filter_sum_z / Filter_Num - 14;
	
	if((Gyro_out->Z<10)&&(Gyro_out->Z>-10))
		Gyro_out->Z=0;

	Filter_count++;
	if(Filter_count == Filter_Num)
		Filter_count=0;
}

void Gyro_Offset(S16_XYZ *Gyro_in,S16_XYZ *Gyro_out)
{
	static u32 Filter_x[50],Filter_y[50],Filter_z[50];
	double Filter_sum_x=0,Filter_sum_y=0,Filter_sum_z=0;
  static u8 gro_cnt=0;

  gro_cnt++;	
	Filter_x[gro_cnt] = Gyro_in->X;
	Filter_y[gro_cnt] = Gyro_in->Y;
	Filter_z[gro_cnt] = Gyro_in->Z;	
	
	if(gro_cnt>=30)
	{
		for(u8 i=1;i<=gro_cnt;i++)
		{
		Filter_sum_x += Filter_x[i];
		Filter_sum_y += Filter_y[i];
		Filter_sum_z += Filter_z[i];			
		}
	Gyro_out->X = Filter_sum_x / gro_cnt;
	Gyro_out->Y = Filter_sum_y / gro_cnt;
	Gyro_out->Z = Filter_sum_z / gro_cnt;
	}	

	if(gro_cnt >= 30)
		gro_cnt=0;	
	
}

void Acc_Offset(S16_XYZ *Acc_in,S16_XYZ *Acc_out)
{
	static u32 Filter_x[50],Filter_y[50],Filter_z[50];
	double Filter_sum_x=0,Filter_sum_y=0,Filter_sum_z=0;
  static u8 acc_cnt=0;

  acc_cnt++;	
	Filter_x[acc_cnt] = Acc_in->X;
	Filter_y[acc_cnt] = Acc_in->Y;
	Filter_z[acc_cnt] = Acc_in->Z - 65536/32;	
	
	if(acc_cnt>=30)
	{
		for(u8 i=1;i<=acc_cnt;i++)
		{
		Filter_sum_x += Filter_x[i];
		Filter_sum_y += Filter_y[i];
		Filter_sum_z += Filter_z[i];			
		}
	Acc_out->X = Filter_sum_x / acc_cnt;
	Acc_out->Y = Filter_sum_y / acc_cnt;
	Acc_out->Z = Filter_sum_z / acc_cnt;
	}	

	if(acc_cnt >= 30)
		acc_cnt=0;		
}


/*********************************************************
*增强型均值滤波算法    去掉一个最大值和最小值
*输入：输入新的数据  
*输出：当前的数据
*注意：1.输入的数据在程序中完成缓存
*      2.不支持负数的输入
**********************************************************/
#define MEANSUM 20
s16 enhanceMeanFilter(s16 newDatas)
{
	s16 meandata[MEANSUM];
	static unsigned char dataNum = 0;
	s16 dataSum = 0;
	unsigned char i = 0;
	static s16 max,min;
	meandata[dataNum ++] = newDatas;
	if(dataNum >= MEANSUM)
	{
		dataNum = 0;
	}
	
	max = min = meandata[0];    //初始化
	//找到最大最小值
	for(i =1;i < MEANSUM; i ++)
	{
		if(meandata[i] > max)
		{
			max = meandata[i];
		}
		
		if(meandata[i] < min)
		{
			min = meandata[i];
		}
	}
	
	for(i = 0;i < MEANSUM;i ++)
	{
		dataSum += meandata[i];
	}
	dataSum -= (max + min); 
	//return (float)dataSum/MEANSUM;
	return dataSum/(MEANSUM-2);
}

