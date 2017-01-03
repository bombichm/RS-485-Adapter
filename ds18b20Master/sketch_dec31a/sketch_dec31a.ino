char myStr[] = "this is a test today";
int i;

void setup(){
  Serial.begin(9600);
}

void loop() { 
  for (i = 0; i < sizeof(myStr) - 1; i++){
    Serial.print(i, DEC);
    Serial.print(" = ");
    Serial.write(myStr[i/2]);
    Serial.println();
  }
  delay(5000); // slow down the program
}
