#define analogPin A0 /* ESP8266 Analog Pin ADC0 = A0 */

int adcValue = 0;  /* Variable to store Output of ADC */
int LED = 5;

void setup()
{
  Serial.begin(115200); /* Initialize serial communication at 115200 */
  pinMode(LED, OUTPUT);
}

void loop()
{
  adcValue = analogRead(analogPin); /* Read the Analog Input value */
  if (analogRead(analogPin)> 500){

    digitalWrite(LED, HIGH);
  }else{
    digitalWrite(LED, LOW);
  }
  /* Print the output in the Serial Monitor */
  Serial.print("ADC Value = ");
  Serial.println(adcValue);
  
  delay(100);
}