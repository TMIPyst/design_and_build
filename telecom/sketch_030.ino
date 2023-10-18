#include <NewPing.h>
#include <SoftwareSerial.h>
//定义五种运动状态
#define STOP      0
#define FORWARD   1
#define BACKWARD  2
#define TURNLEFT  3
#define TURNRIGHT 4
//定义需要用到的引脚
int leftMotor1 = 2;
int leftMotor2 = 3;
int rightMotor1 = 4;
int rightMotor2 = 5;
int leftPWM=6;
int rightPWM=7;
int sense11=22;
int sense12=23;
int sense21=24;
int sense22=25;
int sense31=26;
int sense32=27;
int button= 28;
int col=29;
//颜色感应及识别
int out=32;
int s0=30;
int s1=31;
int s2=33;
int s3=34;
int led=35;
//蓝牙引脚初始化
SoftwareSerial BTSerial(39, 38); // RX, TX

int outfre1=0;
int outfre2=0;
int outfre3=0;

float exe_time=0.00;

boolean motor_state=false;

NewPing sonar1(sense11, sense12);
NewPing sonar2(sense21, sense22);
NewPing sonar3(sense31, sense32);

void setup() {
  // put your setup code here, to run once:
  //设置控制电机的引脚为输出状态
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
  pinMode(leftPWM, OUTPUT);
  pinMode(rightPWM, OUTPUT);
  pinMode(button, INPUT); 
  pinMode(col,INPUT);
  pinMode(out,INPUT);
  pinMode(s0,OUTPUT);
  pinMode(s1,OUTPUT);
  pinMode(s2,OUTPUT);
  pinMode(s3,OUTPUT); 
  pinMode(led,OUTPUT);
  digitalWrite(leftMotor1, LOW);
  digitalWrite(leftMotor2, LOW);
  digitalWrite(rightMotor1, LOW);
  digitalWrite(rightMotor2, LOW);
  digitalWrite(s0,HIGH);
  digitalWrite(s1,LOW);

  Serial.begin(9600);
  BTSerial.begin(9600); // 将蓝牙模块与软串口通信端口绑定
}

void loop() {
  // put your main code here, to run repeatedly:
  int cmd;
  
  if(digitalRead(button)==LOW){      
      motor_state=!motor_state;
  }
  if(motor_state==true){
    delay(100);
      exe_time=exe_time+0.1;
    obstacleAvoidance();
    /*for(cmd=0;cmd<5;cmd++)//依次执行向前、向后、向左、想有、停止四个运动状态
      {
        motorRun(cmd);  
        delay(2000);//每个命令执行2s 
        exe_time=exe_time+2;
      } /*/
  }else{
    stop();
  }
  
  //时间统计启用
    // exe_time = 0;//需要后续自行统计时间
  Serial.println(exe_time);
  BTSerial.println(exe_time);//蓝牙传输当前时间
  if(!digitalRead(col)){
    Serial.println("碰撞");
  }

}
void obstacleAvoidance(){
  // 获取三个超声波传感器的距离
  int distance1 = sonar1.ping_cm();
  int distance2 = sonar2.ping_cm();
  int distance3 = sonar3.ping_cm();
  Serial.println(distance1);


  if(distance1 < 25){
  if(distance2 + distance3 > 30){
    if(distance2 > distance3){
      turnLeft();
    }
    if(distance3 >= distance2){
      turnRight();
    }
  }
  if(distance3 > 10 && distance2 < 10){
    backward();
    turnRight();
    moveForward();
    turnRight();
    moveForward();
  }
   if(distance2 > 10 && distance3 < 10){
    backward();
    turnLeft();
    moveForward();
    turnLeft();
    moveForward();
  }
}else{
  moveForward();
  }
}


//运动控制函数
/*void motorRun(int cmd)
{
  switch(cmd){
    case FORWARD:
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, HIGH);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, HIGH);
      break;
     case BACKWARD:
      digitalWrite(leftMotor1, HIGH);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, HIGH);
      digitalWrite(rightMotor2, LOW);
      break;
     case TURNLEFT:
      digitalWrite(leftMotor1, HIGH);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, HIGH);
      break;
     case TURNRIGHT:
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, HIGH);
      digitalWrite(rightMotor1, HIGH);
      digitalWrite(rightMotor2, LOW);
      break;
     default:
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, LOW);
  }
}*/

void moveForward() {
      analogWrite(leftPWM, 125);
      analogWrite(rightPWM, 125);
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, HIGH);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, HIGH);
      BTSerial.print(exe_time);
      BTSerial.print(",1\n");//蓝牙信息传输
      delay(200);
}

void backward() {
      analogWrite(leftPWM, 125);
      analogWrite(rightPWM, 125);
      digitalWrite(leftMotor1, HIGH);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, HIGH);
      digitalWrite(rightMotor2, LOW);
      BTSerial.print(exe_time);
      BTSerial.print(",3\n");//蓝牙信息传输
      delay(200);
}

void turnLeft() {
    analogWrite(leftPWM,200);
    analogWrite(rightPWM, 200);
      digitalWrite(leftMotor1, HIGH);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, HIGH);
      BTSerial.print(exe_time);
      BTSerial.print(",0\n");//蓝牙信息传输
      delay(200);
}

void turnRight() {
      analogWrite(leftPWM, 200);
      analogWrite(rightPWM, 200);
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, HIGH);
      digitalWrite(rightMotor1, HIGH);
      digitalWrite(rightMotor2, LOW);
      BTSerial.print(exe_time);
      BTSerial.print(",2\n");//蓝牙信息传输
      delay(200);
}

void smallRight(){
      analogWrite(leftPWM, 100);
      analogWrite(rightPWM, 0);
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, HIGH);
      digitalWrite(rightMotor1, HIGH);
      digitalWrite(rightMotor2, LOW);
      delay(50);
}

void smallLeft(){
      analogWrite(leftPWM, 0);
      analogWrite(rightPWM, 100);
      digitalWrite(leftMotor1, HIGH);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, HIGH);
      delay(50);
}

void stop() {
  digitalWrite(leftMotor1, LOW);
  digitalWrite(leftMotor2, LOW);
  digitalWrite(rightMotor1, LOW);
  digitalWrite(rightMotor2, LOW);
  delay(50);
}

int colorBlue(){
  digitalWrite(s2,LOW);
  digitalWrite(s3,HIGH);
  outfre1=pulseIn(out,LOW);
  outfre1=map(outfre1,-10,-49,255,0);
  Serial.print(" BLUE ");
  Serial.print(outfre1);
  delay(500);
  return outfre1;
}
int colorGreen(){
  digitalWrite(s2,HIGH);
  digitalWrite(s3,HIGH);
  outfre2=pulseIn(out,LOW);
  outfre2=map(outfre3,-10,-49,255,0);
  Serial.print(" GREEN ");
  Serial.println(outfre2);
  delay(500);
}
int colorRed(){
  digitalWrite(s2,LOW);
  digitalWrite(s3,LOW);
  outfre2=pulseIn(out,LOW);
  outfre2=map(outfre2,-10,-49,255,0);
  Serial.print(" RED ");
  Serial.print(outfre2);
  delay(500);
}
