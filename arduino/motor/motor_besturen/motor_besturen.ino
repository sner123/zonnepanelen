void setup() {
  Serial.begin(9600); // Start Serial Monitor om de stroomwaarde weer te geven
}

void loop() {
    float AcsValue = 0.0, Samples = 0.0, AvgAcs = 0.0, AcsValueF = 0.0;
    
    for (int x = 0; x < 150; x++) { // Verzamel 150 samples
        AcsValue = analogRead(A0); // Lees de huidige sensorwaarde
        Samples += AcsValue;  // Voeg samples samen
        delay(3); // Laat ADC tot rust komen voor de volgende sample (3 ms)
    }
    
    AvgAcs = Samples / 150.0; // Bereken het gemiddelde van de samples
    
    // Converteer de gemiddelde ADC waarde naar spanning (0-5V)
    float Voltage = AvgAcs * (5.0 / 1024.0);
    
    // Bereken de stroom in Ampère
    // 2.5 is de offset (bij 0A output is 2.5V)
    // 0.066V (66mV) is de stijging in uitgangsspanning bij 1A stroom
    AcsValueF = (Voltage - 2.5) / 0.066;
    
    Serial.print("Sensor Voltage: ");
    Serial.print(Voltage);
    Serial.print(" V, Current: ");
    Serial.print(AcsValueF);
    Serial.println(" A"); // Druk de gemeten stroom af op de seriële monitor
    
    delay(500); // Wacht 500ms voor de volgende meting
}
