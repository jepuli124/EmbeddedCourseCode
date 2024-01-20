

int propotionalGain = 1;
int currentSpeed = 0;
float lastValue = 0;
int list = [100];
int counter = 0;
const int averageValue = 500
const int minSpeed = 120
const int maxSpeed = 120

void setup() {
  // put your setup code here, to run once:
  pinMode(A0, INPUT);
  pinMode(10, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  float value = analogRead(A0);
  Serial.println(analogRead(A0));
  
  if (value-averageValue > currentSpeed + 1){
    if (currentSpeed < maxSpeed-minSpeed){
      currentSpeed = currentSpeed + propotionalGain;
    }
  }
      
  if (value-averageValue < currentSpeed - 1){
    if (currentSpeed > 0){
      currentSpeed = currentSpeed - propotionalGain;
    }
  }

  analogWrite(10, minSpeed+currentSpeed);
  digitalWrite(7, 1);
  digitalWrite(8, 0);
  delay(100);

}
