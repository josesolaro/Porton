#include "RFID.hpp"

void RFID::Setup()
{
    nfc.begin();
    nfc.SAMConfig();
}

bool RFID::Read()
{
    bool match = true;
    boolean success;
    uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0}; // Buffer to store the returned UID
    uint8_t uidLength;                     // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
    
    if (success)
    {
        for (uint8_t i = 0; i < uidLength; i++)
        {
            if(uid[i] != TAG1[i] && uid[i] != TAG2[i]){
                match = false;
            }
        }
    }
    else
    {
        Serial.println("Timed out! Waiting for a card...");
        match = false;
    }
    return match;
}