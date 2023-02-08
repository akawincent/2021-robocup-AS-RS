#ifndef __MOTOR_ENCODER_H__
#define __MOTOR_ENCODER_H__

void TIM3CaptureChannel1Callback(void);
void TIM3CaptureChannel3Callback(void);
void TIM5CaptureChannel1Callback(void);
void TIM5CaptureChannel3Callback(void);

void TIM3_IcOverflowCntCallback(void);
void TIM5_IcOverflowCntCallback(void);

extern int encoder;

#endif
