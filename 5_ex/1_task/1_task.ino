#define TOP_FLOOR 6
#define TIME_DOORS_STAY_OPEN 5000
#define TIME_PER_FLOOR 500

int state = 1;
int currFloor = 1;
int trgtFloor = 1;
/*
0 = FAULT
1 = Idle
2 = GOINGUP
3 = GOINGDOWN
4 = DOOROPEN
*/

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println(state);
  switch(state) {
    case 0:
      state = 1;
      currFloor = 1;
      trgtFloor = 1;
    case 1:
      if (Serial.available()) {
        trgtFloor = Serial.parseInt(SKIP_ALL);
        if (trgtFloor < 1 || trgtFloor > TOP_FLOOR) {
            state = 0;
            break;
        }
        else if (trgtFloor > currFloor) {
          state = 2;
          break;
        }
        else if (trgtFloor < currFloor) {
          state = 3;
          break;
        }
        else if (trgtFloor == currFloor) {
          state = 4;
          break;
        }
      }
      break;
    case 2:
      while (currFloor < trgtFloor) {
        delay(TIME_PER_FLOOR); // time taken to travel 1 currFloor
        currFloor++;
      }
      state = 4;
      break;
    case 3:
      while (currFloor > trgtFloor) {
        delay(TIME_PER_FLOOR); // time taken to travel 1 currFloor
        currFloor--;
      }
      state = 4;
      break;
    case 4:
      delay(TIME_DOORS_STAY_OPEN); // doors stay open for 5 seconds
      state = 1;
      break;
  }
}

