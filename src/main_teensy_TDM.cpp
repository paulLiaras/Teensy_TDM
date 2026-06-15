#include <Arduino.h>
#include <Audio.h>

// Générateur et sortie TDM8 (8 canaux)
AudioSynthWaveform    testSignal[6];
AudioOutputTDM2       tdmOut;

// Entrée TDM (Réception depuis la Daisy)
AudioInputTDM2        tdmIn;

// Analyseurs pour écouter les 6 canaux de retour
AudioAnalyzePeak      peakIn[6]; 

// --- Étape 1 : On envoie 6 sinusoïdes sur les 6 canaux (slots 0 à 5)
// IMPORTANT : On utilise uniquement les ports PAIRS (0, 2, 4, 6, 8, 10)
AudioConnection          patchOut0(testSignal[0], 0, tdmOut, 0); 
AudioConnection          patchOut1(testSignal[1], 0, tdmOut, 2); 
AudioConnection          patchOut2(testSignal[2], 0, tdmOut, 4); 
AudioConnection          patchOut3(testSignal[3], 0, tdmOut, 6); 
AudioConnection          patchOut4(testSignal[4], 0, tdmOut, 8); 
AudioConnection          patchOut5(testSignal[5], 0, tdmOut, 10); 

// --- Étape 2 : On écoute ce qui revient sur LES 6 CANAUX
AudioConnection          patchIn0(tdmIn, 0, peakIn[0], 0);
AudioConnection          patchIn1(tdmIn, 2, peakIn[1], 0);
AudioConnection          patchIn2(tdmIn, 4, peakIn[2], 0);
AudioConnection          patchIn3(tdmIn, 6, peakIn[3], 0);
AudioConnection          patchIn4(tdmIn, 8, peakIn[4], 0);
AudioConnection          patchIn5(tdmIn, 10, peakIn[5], 0);

void setup() {
    Serial.begin(9600);
    
    // Allocation mémoire pour les blocs audio Teensy
    AudioMemory(50); // Le TDM peut exiger jusqu'à 16 blocs d'un coup, 50 est très sécurisant.

    // Configuration des 6 sinusoïdes avec des fréquences ET des amplitudes différentes
    for (int i = 0; i < 6; i++) {
        testSignal[i].begin(WAVEFORM_SINE);
        testSignal[i].frequency(110.0 * (i + 1)); // 110, 220, 330, 440, 550, 660 Hz
        // En modifiant l'amplitude pour chaque canal, on peut les identifier à la lecture
        testSignal[i].amplitude(0.1 * (i + 1)); // Amplitudes : 0.1, 0.2, 0.3, 0.4, 0.5, 0.6
    }

    delay(1000);
    Serial.println("--- Démarrage Teensy TDM Master (Test 6 Canaux) ---");
}

unsigned long lastLog = 0;

void loop() {
    if (millis() - lastLog > 1000) {
        lastLog = millis();
        
        Serial.print("<- RECOIT (Daisy) | ");
        for (int i = 0; i < 6; i++) {
            if (peakIn[i].available()) {
                Serial.print("Ch");
                Serial.print(i);
                Serial.print(": ");
                Serial.print(peakIn[i].read(), 3);
                Serial.print("   ");
            }
        }
        Serial.println();
    }
}