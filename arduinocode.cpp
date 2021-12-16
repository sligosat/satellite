const int buzzer = 9; // sets the data pin to 9
int ThermistorPin = A0; // sets or temperature sensor to pin A0
int Vo;
float R1 = 10000;
float logR2, R2, T, Tc, Tf;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
int pressureValue;
float pressure;
float altitude;

float num1;
float num2;
float num3;
float num4;
float num5;

void setup () {
  Serial.begin (9600);
pinMode(buzzer, OUTPUT);
}

void loop () {
  buzzer(); // calls our buzzer loop
  pressureReading(); // calls our pressure loop
  temperatureReading(); // calls our temperature loop
  printResults(); // calls all our print statements from all our sensors
}

void pressureReading() {
  pressureValue = analogRead (A1);
  pressure = ((pressureValue / 1024.0) + 0.095) / 0.0009;

  num1 = pressure * 100.0;
  num2 = num1 / 101325.0;
  num3 = log10 (num2);
  num4 = (num3 / 5.25588);
  num5 = pow(10.0, num4) - 1.0;
  altitude =  num5 / -0.0000225577 + 81; // remove the 81 with better sensor    
}

void temperatureReading() {
  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2) + 4); // remove 4 with better sensor
  Tc = T - 273.15;
  Tf = (Tc * 9.0) / 5.0 + 32.0;
}

void printResults() {
  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.print(" Altitude: ");
  Serial.print(altitude);
  Serial.print(" Temperature: ");
  Serial.println(Tc);
  delay (500);
  }
void buzzer() {
  tone(buzzer, 1000); // sets the frequency of buzzer to 1khz
  delay(1000); // the tone lasts for 1 second
  noTone(buzzer); // stops buzzer 
  delay(1000); // waits a second, then restarts the loop
}