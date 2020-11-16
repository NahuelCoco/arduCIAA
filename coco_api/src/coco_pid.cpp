#include "coco_pid.h"

PID::PID ( PIDstruct *i )
{
	i->pvOld = 0.00;
	i->spOld = 0.00;
	i->uiOld = 0.00;
	i->udOld = 0.00;
}

void PID::Compute ( PIDstruct *i )
{
	float Up, Ui, Ud;

	Kp = i->K;
	Ki = ( i->K * i->SampleTime ) / i->Ti;
	Kda = i->Td / ( i->Td + (i->N * i->SampleTime) );
	Kdb = ( i->K * i->Td * i->N ) / ( i->Td + (i->N * i->SampleTime) );
	Kdc = ( i->c * i->K * i->Td * i->N ) / ( i->Td + (i->N * i->SampleTime) );

	Up = Kp*( ( i->b*(i->Setpoint) ) - (i->Input) );
	Ui = i->uiOld + (Ki*( (i->Setpoint) - (i->Input) ));
	Ud = Kda * i->udOld - Kdb*((i->Input) - i->pvOld) + Kdc*((i->Setpoint) - i->spOld);

	if( i->Mode == AUTOMATIC )
	{
		i->Output = Up + Ui + Ud;

		if ( i->Output > i->OutputMax )
		{
			i->Output = i->OutputMax;
		}

		if ( i->Output < i->OutputMin )
		{
			i->Output = i->OutputMin;
		}
	}

	if ( i->Mode == MANUAL )
	{
		i->Output = i->Setpoint;
	}

	i->uiOld = Ui;
	i->udOld = Ud;
	i->pvOld = i->Input;
	i->spOld = i->Setpoint;
}
/*
void PID::SetOutputLimits ( float setOutputMax, float setOutputMin )
{
	OutputMax = setOutputMax;
	OutputMin = setOutputMin;
}

void PID::SetSampleTime ( float setSampleTime )
{
	SampleTime = setSampleTime;
}

void PID::SetMode ( bool SetMode )
{
	Mode = SetMode;
}

void PID::forExample ( PIDstruct *internalExample )
{
	internalExample->K=3;
}
*/
