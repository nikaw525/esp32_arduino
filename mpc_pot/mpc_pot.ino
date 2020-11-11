
#define VD_CS 12
#define VA_CS 14
#define VM_CS 27
#define VC_CS 26
#define VP_CS 25
#define HV_CS 4
#define CALLIB_N_CS 19
#define CALLIB_P_CS 2
#define U_D 23

void MPC4022(int cs, int u_d, int imp_amount){  
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

void setup() {  
  pinMode (VD_CS, OUTPUT);
  pinMode (VA_CS, OUTPUT);
  pinMode (VM_CS, OUTPUT);
  pinMode (VC_CS, OUTPUT);
  pinMode (VP_CS, OUTPUT);
  pinMode (HV_CS, OUTPUT);
  pinMode (CALLIB_N_CS, OUTPUT);
  pinMode (CALLIB_P_CS, OUTPUT);
  pinMode (U_D, OUTPUT);
  
  digitalWrite(VD_CS, HIGH); 
  digitalWrite(VA_CS, HIGH);  
  digitalWrite(VM_CS, HIGH);  
  digitalWrite(VC_CS, HIGH);  
  digitalWrite(VP_CS, HIGH);  
  digitalWrite(HV_CS, HIGH);  
  digitalWrite(CALLIB_N_CS, HIGH);  
  digitalWrite(CALLIB_P_CS, HIGH);   
  digitalWrite(U_D, HIGH);
}

void loop() {
  delay(10);
  MPC4022(CALLIB_P_CS,0,4);
  delay(10);
  MPC4022(CALLIB_P_CS,1,4);  
}
