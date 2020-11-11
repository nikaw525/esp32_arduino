
#define VD_CS 12
#define VA_CS 14
#define VM_CS 27
#define VC_CS 26
#define VP_CS 25
#define HV_CS 4
#define CALLIB_N_CS 19
#define CALLIB_P_CS 2
#define U_D 23

#define UP 1
#define DOWN 0

void potentiometer_set(int cs, int u_d, int imp_amount){  
  digitalWrite(U_D,u_d);
  delay(1);
  digitalWrite(cs, LOW);
  delay(1);
  for(int i=0; i<imp_amount; i++){
    digitalWrite(U_D,!u_d);
    delay(1);
    digitalWrite(U_D,u_d);
    delay(1);
  }
  digitalWrite(cs, HIGH);
}

void potentiometer_init(){
  int output_pins[]={VD_CS, VA_CS, VM_CS, VC_CS, VP_CS, HV_CS, CALLIB_N_CS, CALLIB_P_CS, U_D};

  for(int i = 0; i<9; i++){
    pinMode(output_pins[i],OUTPUT);
    digitalWrite(output_pins[i],HIGH);
  }  
}

void setup() {  
  potentiometer_init();
}

void loop() {
  delay(10);
  potentiometer_set(CALLIB_P_CS,DOWN,4);
  delay(10);
  potentiometer_set(CALLIB_P_CS,UP,4);  
}
