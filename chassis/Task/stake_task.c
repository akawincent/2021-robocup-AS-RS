#include "stake_task.h"
#include "motor.h"
#include "bsp_delay.h"
#include "openmv.h"
#include "lsc.h"
#include "turntable_task.h"
#include "ats522.h"

extern uint8_t IDLocation[12];

uint8_t action_state_flag = 0;

void pre_stake_task()
{
    move_by_encoder(Y_direct,2800);
    move_by_encoder(X_direct,-1000);
    move_by_encoder(Y_direct,500);
       
    for(int i=0;i<1;i++){
        distinguish_staketask();
        delay_ms(3000);
        span(i);
        tell_openmv_to_catch_stake();
        
        while(1) {
            set_chassis_speed(0,5,0);
            Get_openmv(&openmv);
            //if(openmv.message == 0x1A||openmv.message == 0x1B||openmv.message == 0x1C
                //||openmv.message == 0x1D||openmv.message == 0x1E||openmv.message == 0x1F
                    //||openmv.message == 0x20) break;
        }
        
        set_chassis_speed(0,0,0);
//        
//        switch(openmv.message){
//            case 0x1A:catch_staketask_A();delay_ms(3000);action_state_flag = 1;break;
//            case 0x1B:catch_staketask_B();delay_ms(3000);action_state_flag = 2;break;
//            case 0x1C:catch_staketask_C();delay_ms(3000);action_state_flag = 3;break;
//            case 0x1D:catch_staketask_D();delay_ms(3000);action_state_flag = 4;break;
//            case 0x1E:catch_staketask_E();delay_ms(3000);action_state_flag = 5;break;
//            case 0x1F:catch_staketask_F();delay_ms(3000);action_state_flag = 6;break;
//            case 0x20:catch_staketask_G();delay_ms(3000);action_state_flag = 7;break;
//        }
//        
//        tell_claw_to_catch();
//        
//        while(1){
//            Get_openmv(&openmv);
//            if(openmv.message == 0x02) break;
//        }
//        
//        switch(action_state_flag){
//            case 1:put_staketask_A();delay_ms(3000);break;
//            case 2:put_staketask_B();delay_ms(3000);break;
//            case 3:put_staketask_C();delay_ms(3000);break;
//            case 4:put_staketask_D();delay_ms(3000);break;
//            case 5:put_staketask_E();delay_ms(3000);break;
//            case 6:put_staketask_F();delay_ms(3000);break;
//            case 7:put_staketask_G();delay_ms(3000);break;
//        }
//        
//        tell_openmv_to_loosen();
//        delay_ms(1500);  
//        IDLocation[i]=get_ball_id(); 
    }
}


void final_stake_task()
{
    move_by_encoder(Y_direct,2800);
    move_by_encoder(X_direct,1000);
    move_by_encoder(Y_direct,500);
       
    for(int i=0;i<2;i++){
        distinguish_staketask();
        delay_ms(3000);
        span(i);
        tell_openmv_to_catch_stake();
        
        while(1) {
            set_chassis_speed(0,5,0);
            Get_openmv(&openmv);
            if(openmv.message == 0x1A||openmv.message == 0x1B||openmv.message == 0x1C
                ||openmv.message == 0x1D||openmv.message == 0x1E||openmv.message == 0x1F
                    ||openmv.message == 0x20) break;
        }
        set_chassis_speed(0,0,0);
        
        switch(openmv.message){
            case 0x1A:catch_staketask_A();delay_ms(3000);action_state_flag = 1;break;
            case 0x1B:catch_staketask_B();delay_ms(3000);action_state_flag = 2;break;
            case 0x1C:catch_staketask_C();delay_ms(3000);action_state_flag = 3;break;
            case 0x1D:catch_staketask_D();delay_ms(3000);action_state_flag = 4;break;
            case 0x1E:catch_staketask_E();delay_ms(3000);action_state_flag = 5;break;
            case 0x1F:catch_staketask_F();delay_ms(3000);action_state_flag = 6;break;
            case 0x20:catch_staketask_G();delay_ms(3000);action_state_flag = 7;break;
        }
        
        tell_claw_to_catch();
        
        while(1){
            Get_openmv(&openmv);
            if(openmv.message == 0x02) break;
        }
        
        switch(action_state_flag){
            case 1:put_staketask_A();delay_ms(3000);break;
            case 2:put_staketask_B();delay_ms(3000);break;
            case 3:put_staketask_C();delay_ms(3000);break;
            case 4:put_staketask_D();delay_ms(3000);break;
            case 5:put_staketask_E();delay_ms(3000);break;
            case 6:put_staketask_F();delay_ms(3000);break;
            case 7:put_staketask_G();delay_ms(3000);break;
        }
        
        tell_openmv_to_loosen();
        delay_ms(1500);  
        IDLocation[i]=get_ball_id(); 
    }
}

