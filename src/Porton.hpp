#include <string>
#include "RFID.hpp"
#include "MServer.hpp"

#define APERTURA GPIO_NUM_12
#define CIERRE GPIO_NUM_13

#define OPEN_OUTPUT GPIO_NUM_19
#define CLOSE_OUTPUT GPIO_NUM_21

#define CURRENT GPIO_NUM_34

enum PortonStatus {
    stop,
    opening,
    closing,
    opened,
    closed,
};

class Porton
{
    public:
        void Init(RFID* rfid, MServer* httpserver);
        void Request(std::string command);
        void ListenCmd();
        void Open();
        void Close();
        void Stop();
        char* GetStatus();
        void CheckCurrent();

    private:
        RFID* rfID;
        MServer* httpServer;
        PortonStatus status = stop;
        const char* openEndpoint = "/open";
        const char*  closeEndpoint = "/close";
        const char*  stopEndpoint = "/stop";
        void SetStatus(PortonStatus);
        uint16_t MAX_CURRENT = 2048;
        const char* GetContent();
};
