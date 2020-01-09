//---------------------------------------------------------------------------


#pragma hdrstop

#include <stdint.h>
#include <string.h>

#include "Forecast.h"
#include "Detection.h"
#include "Trace.h"
#include <math.h>

Forecast MtForecast;

void __fastcall Forecast::Initial(void)
{
	uint8_t i;

	memset(ForecastArea, 0, sizeof(ForecastArea));

	for(i=0; i<MAX_POINTS-1; i+=1)
	{
		ForecastArea[i].Next = &ForecastArea[i+1];
		ForecastArea[i].NextSon = NULL;
	}

	ForecastArea[i].Next = NULL;
	ForecastArea[i].NextSon = NULL;
}

void __fastcall Forecast::Forecast_XY(int mesr,int opt[],int fcast[],float PErr[][3])
{
	int Err = 10000,optErr;
	const int T=10;
	float q = 10;
//	float Fai[3][3]={1,T,T*T/2,
//					  0,1,T,
//					  0,0,1};
//	float G2[3][3]={T*T*T*T/16,T*T*T/8,T*T/4,
//					T*T*T/8,T*T//4,T/2,
//					T*T//4,T/2,1};

	optErr = (fcast[0]-mesr)*(fcast[0]-mesr);
	PErr[0][0] += optErr;
	/////PErr = Fai * PErr *Fai + G*q*I*G

	PErr[0][0] += PErr[1][0]*T + PErr[2][0]*T*T/2 + PErr[0][1]*T + PErr[1][1]*T*T + PErr[2][1]*T*T*T/2
					+ PErr[0][2]*T*T/2 + PErr[1][2]*T*T*T/2 + PErr[2][2]*T*T*T*T/4 + q*T*T*T*T/16;

	PErr[0][1] += PErr[1][1]*T + PErr[2][1]*T*T/2 +  PErr[0][2]*T + PErr[1][2]*T*T + PErr[2][2]*T*T*T/2 + q*T*T*T/8;

	PErr[0][2] += PErr[1][2]*T + PErr[2][2]*T*T/2 + q/4;

	PErr[1][0] += PErr[2][0]*T + PErr[1][1]*T + PErr[2][1]*T*T
					+ PErr[1][2]*T*T/2 + PErr[2][2]*T*T*T/2 + q*T*T*T/8;

	PErr[1][1] +=  PErr[2][1]*T+ PErr[1][2]*T + PErr[2][2]*T*T + q*T*T/4;

	PErr[1][2] += PErr[2][2]*T + q*T/2;

	PErr[2][0] += PErr[2][1]*T + PErr[2][2]*T*T/2 + q*T*T/4;

	PErr[2][1] += PErr[2][2]*T + q*T/2;

	PErr[2][2] += q;

	float tmp = PErr[0][0]+Err;
	kg[0] = PErr[0][0]/tmp;
	kg[1] = PErr[1][0]/tmp;
	kg[2] = PErr[2][0]/tmp;

	opt[0] = fcast[0] + kg[0]*(mesr - fcast[0]);
	opt[1] = fcast[1] + kg[1]*(mesr - fcast[0]);
	opt[2] = fcast[2] + kg[2]*(mesr - fcast[0]);

	PErr[1][0] -= kg[1]*PErr[0][0];
	PErr[1][1] -= kg[1]*PErr[0][1];
	PErr[1][2] -= kg[1]*PErr[0][2];
	PErr[2][0] -= kg[2]*PErr[0][0];
	PErr[2][1] -= kg[2]*PErr[0][1];
	PErr[2][2] -= kg[2]*PErr[0][2];

	PErr[0][0] -= kg[0]*PErr[0][0];  //不能写在前面
	PErr[0][1] -= kg[0]*PErr[0][1];
	PErr[0][2] -= kg[0]*PErr[0][2];

	fcast[0] = opt[0] + opt[1]*T + opt[2]*T*T/2;  //fcast为上一帧的预测点mesr 上一帧的数据
	fcast[1] = opt[1] + opt[2]*T;
	fcast[2] = opt[2];
}

void __fastcall Forecast::Forecast_NextPoint(trace_t (&TraceAreas)[])      //fix me !!!!!!!!!!!!!!!!!!!
{
	int x, y, vx, vy, ax, ay; //Errx, Erry;
	int px, py;
	uint8_t index, prev, pprev;
	area_t *at;
	int32_t a,b,c,d,e,f,g,h;


	for(int i=0; i<MAX_POINTS; i+=1)
	{
		ForecastArea[i].Status = TraceAreas[i].Sta;

		if(TraceAreas[i].Sta != AREA_DEAD)
		{
			//forecast
			index = TraceAreas[i].Index;
			index = (index-1) & TRACE_BUF_LEN_MASK;

			at = TraceAreas[i].OldArea;
			px = x = (at[index].x1 + at[index].x2) / 2;
			py = y = (at[index].y1 + at[index].y2) / 2;
			x_mesr = x;
			y_mesr = y;


#if 1
//			const int Err=10000,T=10;
//			const float q=10;
//			static float XPErr[3][3]={Err,Err/T,0,
//							Err/T,q*T*T/16+2*Err/(T*T),q*T/4,
//							0,q*T/4,2*q};
//			static float YPErr[3][3]={Err,Err/T,0,
//							Err/T,q*T*T/16+2*Err/(T*T),q*T/4,
//							0,q*T/4,2*q};
//
//			static int fcast = 0;
//			if(fcast == 0)
//			{
//				x_fcast[0] = x_mesr;
//				y_fcast[0] = y_mesr;
//				x_fcast[1] = 0;
//				y_fcast[1] = 0;
//				x_fcast[2] = 0;
//				y_fcast[2] = 0;
//				fcast = 1;
//			}
//
//			Forecast_XY(x_mesr,x_opt,x_fcast,XPErr);
//			Forecast_XY(y_mesr,y_opt,y_fcast,YPErr);
//			x = x_fcast[0];
//			y = y_fcast[0];
//
//			Sta[i].Err = MIN_DIS_ERR;



			vx = vy = ax = ay = 0;// = Errx = Erry = 0;

			if(TraceAreas[i].Nums > 1)
			{
				//预测误差
				//Errx = (ForecastArea[i].x1 + ForecastArea[i].x2)/2;
				//Erry = (ForecastArea[i].y1 + ForecastArea[i].y2)/2;

				//Errx -= x;
				//Erry -= y;

				//velocity
				for(int j= 0; j<TraceAreas[i].Nums-1; j+=1)
				{
					prev = (index - 1) & TRACE_BUF_LEN_MASK;

					vx += ((at[index].x1 + at[index].x2)/2 - (at[prev].x1 + at[prev].x2)/2);
					vy += ((at[index].y1 + at[index].y2)/2 - (at[prev].y1 + at[prev].y2)/2);

					index = prev;
				}

				vx /= (TraceAreas[i].Nums - 1);
				vy /= (TraceAreas[i].Nums - 1);

				//Acceleration
				index = TraceAreas[i].Index;
				index = (index-1) & TRACE_BUF_LEN_MASK;

				if(TraceAreas[i].Nums > 2)
				{
					int vx1, vx2, vy1, vy2;
					for(int j= 0; j<TraceAreas[i].Nums-2; j+=1)
					{
						prev = (index - 1) & TRACE_BUF_LEN_MASK;
						pprev = (index - 2) & TRACE_BUF_LEN_MASK;

						vx1 = ((at[index].x1 + at[index].x2)/2 - (at[prev].x1 + at[prev].x2)/2);
						vy1 = ((at[index].y1 + at[index].y2)/2 - (at[prev].y1 + at[prev].y2)/2);

						vx2 = ((at[prev].x1 + at[prev].x2)/2 - (at[pprev].x1 + at[pprev].x2)/2);
						vy2 = ((at[prev].y1 + at[prev].y2)/2 - (at[pprev].y1 + at[pprev].y2)/2);

						ax += (vx1 - vx2);
						ay += (vy1 - vy2);

						index = prev;
					}

					ax /= (TraceAreas[i].Nums - 2);
					ay /= (TraceAreas[i].Nums - 2);
				}

				index = TraceAreas[i].Index;
				index = (index-1) & TRACE_BUF_LEN_MASK;
				prev = (index-1) & TRACE_BUF_LEN_MASK;

				a = (at[index].x1 + at[index].x2)/2;
				b = (at[index].y1 + at[index].y2)/2;

				c = (at[prev].x1 + at[prev].x2)/2;
				d = (at[prev].y1 + at[prev].y2)/2;

				c -= a;
				d -= b;

				Sta[i].Err = c*c + d*d;
			}
			 //else
			{
				Sta[i].Err = MAX_DIS_ERR;
			}

			if(Sta[i].Err < MIN_DIS_ERR)
				Sta[i].Err = MIN_DIS_ERR;

			Sta[i].vx = vx;
			Sta[i].vy = vy;
			Sta[i].ax = ax;
			Sta[i].ay = ay;

			x += (vx + ax/2);// - 0.5*Errx;
			y += (vy + ay/2);// - 0.5*Erry;

			//px += (vx/2 + ax/8);
			//py += (vy/2 + ay/8);

//#else
			if(TraceAreas[i].Nums == 2)   //linear
			{
				int16_t y2,x2, y1,x1;

				x2 = (at[index].x1 + at[index].x2) / 2;
				y2 = (at[index].y1 + at[index].y2) / 2;

				index = (index-1) & TRACE_BUF_LEN_MASK;

				x1 = (at[index].x1 + at[index].x2) / 2;
				y1 = (at[index].y1 + at[index].y2) / 2;

				b = x2 - x1;
				c = x1;

				e = y2 - y1;
				f = y1;

				//x = b*2 + c;
				//y = e*2 + f;

				px = b*3/2 + c;
				py = e*3/2 + f;
			}
			else //if(TraceAreas[i].Nums == 3)   //f(t) = at^2 + bt + c
			{
				int16_t y2,x2, y1,x1, y0, x0;

				x2 = (at[index].x1 + at[index].x2) / 2;
				y2 = (at[index].y1 + at[index].y2) / 2;

				index = (index-1) & TRACE_BUF_LEN_MASK;

				x1 = (at[index].x1 + at[index].x2) / 2;
				y1 = (at[index].y1 + at[index].y2) / 2;

				index = (index-1) & TRACE_BUF_LEN_MASK;

				x0 = (at[index].x1 + at[index].x2) / 2;
				y0 = (at[index].y1 + at[index].y2) / 2;

				a = (x0 + x2)/2 - x1;
				b = (x2 - x0)/2;
				c = x1;

				d = (y0 + y2)/2 - y1;
				e = (y2 - y0)/2;
				f = y1;

				//x = a*4 + b*2 + c;
				//y = d*4 + e*2 + f;

				px = a*9/4 + b*3/2 + c;
				py = d*9/4 + e*3/2 + f;
			}
//			else if(TraceAreas[i].Nums > 3)   //f(t) = at^3 + bt^2 + ct + d
//			{
//				int16_t y3,x3,y2,x2, y1,x1, y0, x0;
//
//				x3 = (at[index].x1 + at[index].x2) / 2;
//				y3 = (at[index].y1 + at[index].y2) / 2;
//
//				index = (index-1) & TRACE_BUF_LEN_MASK;
//
//				x2 = (at[index].x1 + at[index].x2) / 2;
//				y2 = (at[index].y1 + at[index].y2) / 2;
//
//				index = (index-1) & TRACE_BUF_LEN_MASK;
//
//				x1 = (at[index].x1 + at[index].x2) / 2;
//				y1 = (at[index].y1 + at[index].y2) / 2;
//
//				index = (index-1) & TRACE_BUF_LEN_MASK;
//
//				x0 = (at[index].x1 + at[index].x2) / 2;
//				y0 = (at[index].y1 + at[index].y2) / 2;
//
//				a = (-x0 + 3*x1 - 3*x2 + x3)/6;
//				b = (x1 + x3)/2 - x2;
//				c = (x0 - 6*x1 + 3*x2 + 2*x3)/6;
//				d = x2;
//
//				e = (-y0 + 3*y1 - 3*y2 + y3)/6;
//				f = (y1 + y3)/2 - y2;
//				g = (y0 - 6*y1 + 3*y2 + 2*y3)/6;
//				h = y2;
//
//				px = a*8 + b*4 + c*2 + d;
//				py = e*8 + f*4 + g*2 + h;
//			}

			//Sta[i].Err = MAX_DIS_ERR;
#endif
			x_fcast[0]=x;
			y_fcast[0]=y;
			index = TraceAreas[i].Index;
			index = (index-1) & TRACE_BUF_LEN_MASK;

			ForecastArea[i].x1 = x - (at[index].x2 - at[index].x1)/2;
			ForecastArea[i].x2 = x + (at[index].x2 - at[index].x1)/2;
			ForecastArea[i].y1 = y - (at[index].y2 - at[index].y1)/2;
			ForecastArea[i].y2 = y + (at[index].y2 - at[index].y1)/2;

			ForecastPoint[i].x1 = px - (at[index].x2 - at[index].x1)/2;
			ForecastPoint[i].x2 = px + (at[index].x2 - at[index].x1)/2;
			ForecastPoint[i].y1 = py - (at[index].y2 - at[index].y1)/2;
			ForecastPoint[i].y2 = py + (at[index].y2 - at[index].y1)/2;
		}
	}
}

#pragma package(smart_init)
