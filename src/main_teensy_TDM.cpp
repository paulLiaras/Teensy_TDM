#include <Arduino.h>
#include <Audio.h>

// --- Entrées / Sorties ---
AudioInputUSB         usbIn;      // Entrée depuis le PC via USB (Stéréo)

// --- ATTÉNUATEURS DE VOLUME ---
AudioAmplifier        ampL;       // Pour baisser le volume Gauche
AudioAmplifier        ampR;       // Pour baisser le volume Droit

AudioOutputUSB        usbOut;     // Sortie vers le PC via USB (Stéréo)

AudioOutputTDM2       tdmOut;     // Envoi vers la Daisy
AudioInputTDM2        tdmIn;      // Retour depuis la Daisy

// --- SONDES DE DIAGNOSTIC ---
AudioAnalyzePeak      peakUsbL;   // Sonde : Ce qui arrive du PC (après atténuation)
AudioAnalyzePeak      peakUsbR;   
AudioAnalyzePeak      peakTdmL;   // Sonde : Ce qui revient de la Daisy
AudioAnalyzePeak      peakTdmR; 

// --- ROUTAGE 1 : PC -> Ampli -> Teensy -> Daisy ---
// 1a. On branche l'USB dans les atténuateurs
AudioConnection       patchUsbAmpL(usbIn, 0, ampL, 0);
AudioConnection       patchUsbAmpR(usbIn, 1, ampR, 0);

// 1b. On envoie le son atténué dans le bus TDM
AudioConnection       patchOutL(ampL, 0, tdmOut, 0); 
AudioConnection       patchOutR(ampR, 0, tdmOut, 2); 

// On branche nos sondes APRES les atténuateurs pour voir le vrai volume envoyé
AudioConnection       patchProbeUsbL(ampL, 0, peakUsbL, 0);
AudioConnection       patchProbeUsbR(ampR, 0, peakUsbR, 0);

// --- ROUTAGE 2 : Daisy -> Teensy -> PC ---
AudioConnection       patchInL(tdmIn, 0, usbOut, 0);
AudioConnection       patchInR(tdmIn, 2, usbOut, 1);

// On branche nos sondes sur le retour Daisy
AudioConnection       patchProbeTdmL(tdmIn, 0, peakTdmL, 0);
AudioConnection       patchProbeTdmR(tdmIn, 2, peakTdmR, 0);

void setup() {
    Serial.begin(9600);
    AudioMemory(50);
    
    // --- RÉGLAGE DU VOLUME ---
    // 1.0 = Volume normal (100%)
    // 0.5 = Volume divisé par 2 (50%)
    // 0.1 = Volume divisé par 10 (10%)
    ampL.gain(0.5); 
    ampR.gain(0.5); 
    
    delay(1000);
    // Serial.println("--- DEMARRAGE DU DIAGNOSTIC COMPLET (AVEC ATTENUATION) ---");
}

unsigned long lastLog = 0;

void loop() {
    if (millis() - lastLog > 500) {
        lastLog = millis();
        
        // if (peakUsbL.available() && peakUsbR.available() && peakTdmL.available() && peakTdmR.available()) {
        //     Serial.print("ENTREE PC : [ L: ");
        //     Serial.print(peakUsbL.read(), 3);
        //     Serial.print(" | R: ");
        //     Serial.print(peakUsbR.read(), 3);
        //     Serial.print(" ]  ====>  RETOUR DAISY : [ L: ");
        //     Serial.print(peakTdmL.read(), 3);
        //     Serial.print(" | R: ");
        //     Serial.print(peakTdmR.read(), 3);
        //     Serial.println(" ]");
        // }
    }
}