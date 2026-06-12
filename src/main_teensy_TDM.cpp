#include <Arduino.h>
#include <Audio.h>

// Générateur et sortie TDM8 (8 canaux)
AudioSynthWaveform    testSignal;
AudioOutputTDM2       tdmOut;
AudioAnalyzePeak      peakOut; 

// Entrée TDM (Réception depuis la Daisy)
AudioInputTDM2        tdmIn;
AudioAnalyzePeak      peakIn; 

// --- Étape 1 : On envoie la sinusoïde UNIQUEMENT sur le canal 0
AudioConnection          patchOut(testSignal, 0, tdmOut, 0); 
AudioConnection          patchPeakOut(testSignal, 0, peakOut, 0);

// --- Étape 2 : On écoute ce qui revient UNIQUEMENT sur le canal 0
AudioConnection          patchIn(tdmIn, 0, peakIn, 0);

void setup() {
    Serial.begin(9600);
    
    // Allocation mémoire pour les blocs audio Teensy
    AudioMemory(20); 

 

    // Configuration de la sinusoïde de test (440 Hz pour test audio)
    testSignal.begin(WAVEFORM_SINE);
    testSignal.frequency(440.0); 
    testSignal.amplitude(0.80);

    delay(1000);
    Serial.println("--- Démarrage Teensy TDM Master (Test 1 Canal) ---");
}

unsigned long lastLog = 0;

void loop() {
    if (millis() - lastLog > 1000) {
        lastLog = millis();
        if (peakOut.available()) {
            Serial.print("-> Teensy ENVOIE sur Ch0 : ");
            Serial.println(peakOut.read(), 4);
        }
        
        if (peakIn.available()) {
            Serial.print("<- Teensy RECOIT sur Ch0 : ");
            Serial.println(peakIn.read(), 4);
        } else {
            Serial.println("<- En attente de la Daisy...");
        }
        Serial.println("----------------------------------");
    }
}