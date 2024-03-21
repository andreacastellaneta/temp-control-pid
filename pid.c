/*
 * pid.c
 *
 *  Created on: Oct 5, 2023
 *      Author: Andrea_Castellaneta
 */
#include "pid.h"
#define	INTEGRAL_GAIN_MAX	2000000
#define	PID_MAX	1000	/* Max value of the duty cycle, in this case the frequency of the PWM is 1 kHz */

void setGainsPID(PID_Controller *pid, float p, float i, float d)
{
	/* Set the PID controller with the specified gain values */
	pid->KP = p;
	/* The P gain decreases rise time (system reacts faster) and steady state error (it does not eliminate it though).
	 * However, it also increases overshoot and a too high value can make system unstable. */
	pid->KI = i;
	/* The I gain eventually eliminates the steady-state error, and it makes the system robust to external forces.
	 * However, it increases overshoot and can make the system unstable. The integral error may saturate. */
	pid->KD = d;
	/* The D gain decreases the rise time and the overshoot, and it makes the system stable.
	 * However, it amplifies high-frequency noise, if present, and the system can become unstable. */
}

void resetPID(PID_Controller *pid)
{
	/* Initialize the PID controller values */
	pid->KP = 0;
	pid->KI = 0;
	pid->KD = 0;
	pid->error_integral = 0;
	pid->last_error = 0;
	pid->output = 0;
}

void applyPID(PID_Controller *pid, int16_t input_error, uint16_t update_rate)
{
	/* Update the integral error */
	pid->error_integral += input_error;

	/* In order to avoid the saturation of the system, we limit the integral error */
	if(pid->error_integral > INTEGRAL_GAIN_MAX)
	{
		pid->error_integral = INTEGRAL_GAIN_MAX;
	}
	if(pid->error_integral < -INTEGRAL_GAIN_MAX)
	{
		pid->error_integral = -INTEGRAL_GAIN_MAX;
	}
	/* Compute the output duty cycle of the PID controller */
	pid->output = (pid->KP) * input_error +
			(pid->KI) * (pid->error_integral)/update_rate +
			(pid->KD) * update_rate * (input_error - (pid->last_error));

	/* Saturate the output in case of an inadmissible value */
	if(pid->output >= PID_MAX)
	{
		pid->output = PID_MAX;
	}
	if(pid->output <= 0)
	{
		pid->output = 0;
	}

	/* Update the previous error */
	pid->last_error = input_error;
}
