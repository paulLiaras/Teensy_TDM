#include <Arduino.h>
#include <Audio.h>

// --- Entrées / Sorties ---
AudioInputUSB         usbIn;      // Entrée depuis le PC via USB (Stéréo)
AudioOutputUSB        usbOut;     // Sortie vers le PC via USB (Stéréo)

AudioOutputTDM2       tdmOut;     // Envoi vers la Daisy
AudioInputTDM2        tdmIn;      // Retour depuis la Daisy

// Analyseurs pour écouter le retour dans le moniteur série
AudioAnalyzePeak      peakInL; 
AudioAnalyzePeak      peakInR; 

// --- ROUTAGE 1 : PC -> Teensy (USB) -> Daisy (TDM) ---
// Canal USB Gauche (0) -> Canal TDM 0
// Canal USB Droit (1) -> Canal TDM 2 (Qui correspond au Slot 1 de la Daisy !)
AudioConnection       patchOutL(usbIn, 0, tdmOut, 0); 
AudioConnection       patchOutR(usbIn, 1, tdmOut, 2); 

// --- ROUTAGE 2 : Daisy (TDM) -> Teensy -> PC (USB) ---
AudioConnection       patchInL(tdmIn, 0, usbOut, 0);
AudioConnection       patchInR(tdmIn, 2, usbOut, 1);

// Routage vers les analyseurs pour affichage Série
AudioConnection       patchPeakL(tdmIn, 0, peakInL, 0);
AudioConnection       patchPeakR(tdmIn, 2, peakInR, 0);

void setup() {
    Serial.begin(9600);
    
    // Allocation mémoire pour les blocs audio Teensy
    AudioMemory(50); // Le TDM peut exiger jusqu'à 16 blocs d'un coup, 50 est très sécurisant.

    delay(1000);
    Serial.println("--- Démarrage Teensy TDM Pass-through USB <-> Daisy ---");
}

unsigned long lastLog = 0;

void loop() {
    if (millis() - lastLog > 1000) {
        lastLog = millis();
        
        Serial.print("<- RETOUR DAISY (vers USB PC) | ");
        if (peakInL.available() && peakInR.available()) {
            Serial.print("Gauche: ");
            Serial.print(peakInL.read(), 3);
            Serial.print("   Droite: ");
            Serial.print(peakInR.read(), 3);
        }
        Serial.println();
    }
}