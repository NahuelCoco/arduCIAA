#ifndef __COCO_PID_H_
#define __COCO_PID_H_


#include "chip.h"
#include "coco_timer.h"

#define AUTOMATIC 1
#define MANUAL 0

typedef struct{

	float K, Ti, Td;

	float OutputMax, OutputMin;

	float b, c, N;

	float SampleTime;

	float Setpoint;

	float Input;

	float Output;

	bool Mode;

	float pvOld, spOld, uiOld, udOld;

} PIDstruct;

#ifdef __cplusplus
extern "C" {
#endif

class PID
{
	public:
		PID ( PIDstruct *i );
		void Compute ( PIDstruct *i );
		//void SetMode ( bool SetMode );
		//void SetOutputLimits ( float setOutputMax, float setOutputMin );
		//void SetSampleTime ( float setSampleTime );
		//void forExample ( PIDstruct *internalExample );

	private:
		float *Input;
		float *Output;
		float *Setpoint;
		float K = 0.0;
		float Ti = 0.0;
		float Td = 0.0;
		bool Direction = 0;
		float OutputMin = 0.0; //Mínimo valor de salida del PID
		float OutputMax = 9999.0; //Máximo valor de salida del PID
		float SampleTime = 0.005; //En segundos
		bool Mode = AUTOMATIC;
		float Kp, Ki, Kda, Kdb, Kdc;
		float Up, Ui, Ud;
		float uiOld, udOld, pvOld, spOld;
		float N = 5.0;
		float b = 1.0, c = 0.0;
};



#ifdef __cplusplus
}
#endif

#endif /* __COCO_PID_H_ */

