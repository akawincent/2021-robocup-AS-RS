'''
Author: your name
Date: 2022-04-04 14:27:01
LastEditTime: 2022-04-04 16:37:06
LastEditors: Please set LastEditors
Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
FilePath: \Desktop\mv_test_2.py
'''
import sensor, image, time, math,pyb,struct
from pyb import UART,millis,Servo,Pin,LED

#状态和命令标志位
if_catch_strip = False       #是否抓取条形平台标志位
if_catch_stepped = False     #是否抓取阶梯平台标志位
if_stack_transfer = False    #是否倒垛
if_claw_catch = False        #抓取阶梯平台时是否合拢爪子
if_loosen = False            #是否松开爪子标志位
if_catch_clumn = False       #是否开始抓取立桩   


#摄像头识别颜色阈值
red_threshold   =  (32, 80, 33, 82, 2, 81)
blue_threshold = (25, 85, -53, 30, -88, -20)
green_threshold = (47, 75, -78, -33, -51, 54)
aim_threshold = blue_threshold #red_threshold

#openmv舵机定义
head = Servo(3)     #P9
handl = Servo(1)    #P8
handr = Servo(2)    #P7

# 与主板通信所需信息
uart = UART(3,115200)#定义串口3变量
uart.init(115200, bits=8, parity=None, stop=1)
headbyte1 = 0x2C
headbyte2 = 0x12
cmdbyte = 0x01
statusbyte = 0x02
endbyte = 0x5B
buffer_size = 5

#openmv  LED定义
led_red= pyb.LED(1)
led_green = pyb.LED(2)
led_blue = pyb.LED(3)

def turnoffAllLED():
    led_red.off()
    led_green.off()
    led_blue.off()

#左爪子松开
def left_loosen():
    handl.angle(50)

#左爪子夹紧
def left_catch():
    handl.angle(90)

#右爪子松开
def right_loosen():
    handr.angle(-35)

#右爪子夹紧
def right_catch():
    handr.angle(-70)

#抓取条形平台时的角度
def head_to_Strip():
    head.angle(-50)

#抓取阶梯平台时的角度
def head_to_Stepped():
    head.angle(20)

#松开爪子
def loosen():
    global if_loosen
    if if_loosen:
        if_loosen = False
        left_loosen()
        time.sleep(50)
        right_loosen()

#抓取阶梯平台小球
def claw_catch_stepped():
    global if_claw_catch
    if if_claw_catch:
        if_claw_catch = False
        left_catch()
        time.sleep(50)
        right_catch()
        catchFinished()
        time.sleep(50)
        return


#接收主控发来命令
def uart_receive():
    global uart
    global headbyte1
    global headbyte2
    global cmdbyte
    global statusbyte
    global endbyte
    global if_catch
    global if_loosen
    global if_catch_stepped
    global if_catch_strip
    global if_stack_transfer
    global if_claw_catch
    global aim_threshold
    global red_threshold
    global blue_threshold
    global if_catch_clumn
    if uart.any():
        rec = uart.read()#读取参数
        print(rec[3])
        if(rec[0] == headbyte1 and rec[1] == headbyte2 and rec[2] == cmdbyte and rec[4] == endbyte):
            if rec[3] == 1:                 #接收到抓取条形平台命令
                if_catch_strip = True
                return
            if rec[3] == 2:                 #接收到松开爪子命令
                if_loosen = True
                loosen()
                return
            if rec[3] == 3:                 #接收到抓取阶梯平台命令
                if_catch_stepped = True
                return
            if rec[3] == 4:                 #接收到阶梯平台时可以合爪
                if_claw_catch = True
                claw_catch_stepped()
                return
            if rec[3] == 5:
                aim_threshold = red_threshold
                return
            if rec[3] == 6:
                aim_threshold = blue_threshold
                return
            if rec[3] == 7:
                if_stack_transfer = True
                return
            if rec[3] == 8:
                if_catch_clum = True
                return


#向主控发送状态
def uart_transmit(param):
    global uart
    global headbyte1
    global headbyte2
    global cmdbyte
    global statusbyte
    global endbyte
    cmd = struct.pack("<BBBBB",# uint8类型
                       headbyte1,    #帧头1
                       headbyte2,    #帧头2
                       statusbyte,   #状态帧
                       param,        #返回给主控的状态信息
                       endbyte       #帧尾
                       )
    print(cmd[3])
    uart.write(cmd)                  #串口3发过去

#告诉主控识别到小球，车体运动停止
def distinguish_Finished():
    uart_transmit(0x01)

# 告诉主控抓取完成
def catchFinished():
    uart_transmit(0x02)

#动作组抓取阶梯平台高度1
def catch_height_1():
    uart_transmit(0x0A)

#动作组抓取阶梯平台高度2
def catch_height_2():
    uart_transmit(0x0B)

#动作组抓取阶梯平台高度3
def catch_height_3():
    uart_transmit(0x0C)

#动作组抓取立桩A区
def distingui_A():
    uart_transmit(0x1A)

#动作组抓取立桩B区
def distingui_B():
    uart_transmit(0x1B)

#动作组抓取立桩C区
def distingui_C():
    uart_transmit(0x1C)

#动作组抓取立桩D区
def distingui_D():
    uart_transmit(0x1D)

#动作组抓取立桩E区
def distingui_E():
    uart_transmit(0x1E)

#动作组抓取立桩F区
def distingui_F():
    uart_transmit(0x1F)

#动作组抓取立桩G区
def distingui_G():
    uart_transmit(0x20)




# 寻找最大目标色块区域
def find_max(blobs):
    max_size=0
    for blob in blobs:
        if blob[2]*blob[3] > max_size:
            max_blob = blob
            max_size = blob[2]*blob[3]
    return max_blob



#阶梯平台抓取
def stepped_catch():
    global if_catch_stepped
    if if_catch_stepped:
        head.angle(35)
        handr.angle(-50)
        handl.angle(60)
        turnoffAllLED()
        tnow = millis()
        time.sleep(100)
        while True:
            img = sensor.snapshot()
            if millis() - tnow > 9999:
                if_catch_stepped = False
                catchFinished()
                return
            blobs = img.find_blobs([aim_threshold],pixels_threshold=400,merge = True,margin=2)
            if blobs:
                max_b = find_max(blobs)
                img.draw_rectangle(max_b[0:4])
                img.draw_cross(max_b[5],max_b[6])
                if max_b[5] < 126 and max_b[5] > 57 and max_b[6] < 104 and max_b[6] > 19:
                    print("识别成功")
                    if_catch_stepped= False
                    distinguish_Finished()
                    time.sleep(100)
                    return


#条形平台抓取
def strip_catch():
    global if_catch_strip
    if if_catch_strip:
        head.angle(-45)
        left_loosen()
        right_loosen()
        turnoffAllLED()
        tnow = millis()
        time.sleep(100)
        while True:
            img = sensor.snapshot()
            if millis() - tnow > 9999:
                if_catch_strip = False
                catchFinished()
                return
            blobs = img.find_blobs([aim_threshold],pixels_threshold=400,merge = True,margin=2)
            if blobs:
                max_b = find_max(blobs)
                img.draw_rectangle(max_b[0:4])
                img.draw_cross(max_b[5],max_b[6])
                if max_b[5] < 125 and max_b[5] > 67 and max_b[6] < 76 and max_b[6] > 14:
                    if_catch_strip = False
                    distinguish_Finished()
                    left_catch()
                    time.sleep(50)
                    right_catch()
                    time.sleep(100)
                    catchFinished()
                    catchFinished()
                    catchFinished()
                    catchFinished()
                    catchFinished()
                    time.sleep(50)
                    return

#倒垛抓取
def stack_transfer():
    global if_stack_transfer
    if if_stack_transfer:
        head.angle(-45)
        left_loosen()
        right_loosen()
        turnoffAllLED()
        tnow = millis()
        time.sleep(100)
        while True:
            img = sensor.snapshot()
            if millis() - tnow > 9999:
                if_stack_transfer = False
                catchFinished()
                return
            blobs = img.find_blobs([aim_threshold],pixels_threshold=400,merge = True,margin=2)
            if blobs:
                max_b = find_max(blobs)
                img.draw_rectangle(max_b[0:4])
                img.draw_cross(max_b[5],max_b[6])
                if max_b[5] < 215 and max_b[5] > 141 and max_b[6] < 161 and max_b[6] >113:
                    #print("识别成功")
                    if_stack_transfer = False
                    distinguish_Finished()
                    time.sleep(100)
                    return

#立桩抓取
def clmn_catch():
    global if_catch_clumn
    if if_catch_clumn:
        head.angle(35)
        #head.angle()   #mv垂直角度
        left_loosen()
        right_loosen()
        turnoffAllLED()
        tnow = millis()
        time.sleep(100)
        while True:
            img = sensor.snapshot()
            if millis() - tnow > 9999:
                if_catch_clumn = False
                catchFinished()
                return
            blobs = img.find_blobs([aim_threshold],pixels_threshold=400,merge = True,margin=2)
            if blobs:
                max_b = find_max(blobs)
                img.draw_rectangle(max_b[0:4])
                img.draw_cross(max_b[5],max_b[6])
                if max_b[5] < 165 and max_b[5] > 120:
                    time.sleep(50)
                    if max_b[6] < 30 and max_b[6] > 0:
                        print("A区")
                        if_catch_clumn = False
                        distingui_A()
                        #left_catch()
                        #time.sleep(50)
                        #right_catch()
                        #catchFiSnished()
                        time.sleep(50)
                        return
                    elif max_b[6] < 90 and max_b[6] > 30:
                        print("B区")
                        if_catch_clumn = False
                        distingui_B()
                        #left_catch()
                        #time.sleep(50)
                        #right_catch()
                        #catchFinished()
                        time.sleep(50)
                        return
                    elif max_b[6] < 120 and max_b[6] > 90:
                        print("C区")
                        if_catch_clumn = False
                        distingui_C()
                        #left_catch()
                        #time.sleep(50)
                        #right_catch()
                        #catchFinished()
                        time.sleep(50)
                        return
                elif max_b[6] < 158 and max_b[6] > 120:
                        print("D区")
                        if_catch_clumn = False
                        distingui_D()
                        #left_catch()
                        #time.sleep(50)
                        #right_catch()
                        #catchFinished()
                        time.sleep(50)
                        return
                elif max_b[6] < 192 and max_b[6] > 158:
                        print("E区")
                        if_catch_clumn = False
                        distingui_E()
                        #left_catch()
                        #time.sleep(50)
                        #right_catch()
                        #catchFinished()
                        time.sleep(50)
                        return
                elif max_b[6] < 220 and max_b[6] > 192:
                        print("F区")
                        if_catch_clumn = False
                        distingui_F()
                        #left_catch()
                        #time.sleep(50)
                        #right_catch()
                        #catchFinished()
                        time.sleep(50)
                        return
                elif max_b[6] < 240 and max_b[6] > 220:
                        print("G区")
                        if_catch_clumn = False
                        distingui_G()
                        #left_catch()
                        #time.sleep(50)
                        #right_catch()
                        #catchFinished()
                        time.sleep(50)
                        return


left_loosen()
right_loosen()
head.angle(-45)
print('begin!')
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_whitebal(False)
sensor.set_hmirror(True)

while True:
    uart_receive()
    strip_catch()
    stepped_catch()
    stack_transfer()
    clmn_catch()
    #print(aim_threshold)