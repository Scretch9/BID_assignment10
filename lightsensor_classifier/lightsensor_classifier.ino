#define LOWEST_VALUE 600
#define HIGHEST_VALUE 1100

#define LED_LEFT 6
#define LED_RIGHT 9
#define sensorLeft A4
#define sensorRight A5
#define histogramSize 200
#define maxSensorValue  1000


bool peaksFound = false;

int valueHistogram[histogramSize];
int histogramWidth = 1000;

int whiteValue;
int greyValue;
int blackValue;

int upperThreshold;
int lowerThreshold;

int sampleCount = 0;


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  pinMode(sensorLeft, INPUT);
  pinMode(sensorRight, INPUT);

  pinMode(LED_LEFT, OUTPUT);
  pinMode(LED_RIGHT, OUTPUT);

  //Serial.println("Start measuring!");
}

//TODO
// separate sampling for each sensor

void loop() {

  //start sampling values
  if (sampleCount < histogramWidth) {
    goForward();
    //put both sensor values into the histogram
    //putIntoHistogram(analogRead(sensorLeft));
    //delay(5);
    putIntoHistogram(analogRead(sensorLeft));
    delay(10);
    sampleCount += 1;
    //Serial.println(analogRead(sensorRight));
  } else {
    goForward();
    if (!peaksFound) {
      Serial.println("finding maxima...");
      delay(10);
      findMaxima();
      getThresholds();
      peaksFound = true;

      Serial.print("upper threshold: "); Serial.println(upperThreshold);
      Serial.print("lower threshold: "); Serial.println(lowerThreshold);
      Serial.print("white: "); Serial.println(whiteValue);
      Serial.print("grey: "); Serial.println(greyValue);
      Serial.print("black: "); Serial.println(blackValue);
    } else {

      int measureLeft = putIntoHistogram(analogRead(sensorLeft));
      int measureRight = putIntoHistogram(analogRead(sensorRight));
      int left = classifyValue(measureLeft);
      int right = classifyValue(measureRight);
      Serial.print("left: "); Serial.print(left);
      Serial.print("right: "); Serial.println(right);
      driveControl(left,right);
      delay(1000);
    }

  }
}

void driveControl(int left,int right) {
  if (left == 0 && right == 2) {
    goLeft();
  }
  if (left == 1 && right == 0) {
    goLeft();
  }
  if (left == 2 && right == 1) {
    goLeft();
  }

  if (left == 0 && right == 1) {
    goRight();
  }
  if (left == 1 && right == 2) {
    goRight();
  }
  if (left == 2 && right == 0) {
    goRight();
  }
}

void lampsOff() {
  digitalWrite(LED_RIGHT, LOW);
  digitalWrite(LED_LEFT, LOW);
}

void goForward() {
  digitalWrite(LED_RIGHT, HIGH);
  digitalWrite(LED_LEFT, HIGH);
}

void goBackward() {
  digitalWrite(LED_RIGHT, HIGH);
  digitalWrite(LED_LEFT, HIGH);
  delay(500);
  digitalWrite(LED_RIGHT, LOW);
  digitalWrite(LED_LEFT, LOW);
}

void goLeft() {
  digitalWrite(LED_RIGHT, LOW);
  digitalWrite(LED_LEFT, HIGH);
}

void goRight() {
  digitalWrite(LED_LEFT, LOW);
  digitalWrite(LED_RIGHT, HIGH);
}

void getThresholds() {
  upperThreshold = (int)((whiteValue + greyValue) / 2);
  lowerThreshold = (int)((blackValue + greyValue) / 2);
}

int classifyValue(int value) {
	int out = 0;
	if(value > lowerThreshold) out++;
	if(value > upperThreshold) out++;
	Serial.println(out);
	return out;
}

int putIntoHistogram(int value) {
  int i = map(value, LOWEST_VALUE, HIGHEST_VALUE, 0 , histogramSize);
  valueHistogram[i]++;
  Serial.println(i);
  return i;
}


void findMaxima() {
  int localMax[(int) histogramSize / 2] = {0};
  int pointerToCurrentLocalMax = 0;
  int histogramPeaks[3] = {};


  //find peaks
  for (int i = 1; i < histogramSize; i++) {
    if (valueHistogram[i - 1] < valueHistogram[i] && valueHistogram[i] > valueHistogram[i + 1]) {
      localMax[pointerToCurrentLocalMax] = i;
      pointerToCurrentLocalMax ++;
    }
  }

  //select only the three highest peaks
  for (int j = 0; j < 3; ++j)
  {
    int _max = 0;
    int index = 0;
    for (int i = 1; i < (int) (histogramSize / 2); i++) {
      if (valueHistogram[localMax[i]] > _max) {
        _max = valueHistogram[localMax[i]];
        index = i;
      }
    }
    histogramPeaks[j] = localMax[index];
    valueHistogram[localMax[index]] = 0;
  }

  setLightValues(histogramPeaks);

  Serial.println("Found Maxima!");
  delay(10);
}

void setLightValues(int peaks[]){
  blackValue = 10000;
  whiteValue = 0;
  int sum = 0;

  for (int i = 0; i < 3; ++i)
  {
    Serial.println(peaks[i]);
    delay(10);
    sum += peaks[i];
    if (peaks[i] < blackValue) blackValue = peaks[i];
    if (peaks[i] > whiteValue) whiteValue = peaks[i];
  }

  greyValue =  sum - whiteValue -  blackValue;
}


