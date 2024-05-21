#include "Wifi.h"
#include "WiFiServer.h"
#include "WiFiClient.h"

class MServer 
{
    public:
        void Connect();
        void Listen();
        std::string ReadContent();
        void Close();
        void ServeContent(const char* content);
        bool Connected();

    private:
        WiFiServer server;
        WiFiClient client;
        const char* ip;
        const char* password = "N8DM8CGH12M1";
        const char* ssid = "HITRON-4A50_EXT";
};