#include "pn532lib/PN532.h"
#include "pn532lib/PN532_HSU.h"

class RFID
{
    public:
        void Setup();
        bool Read();
    private:
        PN532_HSU rfid = PN532_HSU(Serial1);
        PN532 nfc = PN532(rfid);
        byte TAG1[4] = {3, 67, 48, 48};
        byte TAG2[4] = {19, 210, 67, 14};
};