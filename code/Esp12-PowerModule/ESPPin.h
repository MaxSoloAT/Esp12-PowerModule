#define A1 1
#define A2 3
#define B1 5 //+
#define B2 4 //+
#define C1 12 //+
#define C2 13 //+
#define D1 14 //+
#define D2 16 //+2


void forward(int input1, int input2){
  pinMode(input1, OUTPUT);
  pinMode(input2, OUTPUT);
  digitalWrite(input1, HIGH);
  digitalWrite(input2, LOW);
}
void stops(int input1, int input2){
  pinMode(input1, OUTPUT);
  pinMode(input2, OUTPUT);
  digitalWrite(input1, LOW);
  digitalWrite(input2, LOW);
}

void PWM(int input1, int input2, int slider){
  pinMode(input1, OUTPUT);
  pinMode(input2, OUTPUT); 
  analogWrite(input1, slider);
  digitalWrite(input2, LOW); 
}
