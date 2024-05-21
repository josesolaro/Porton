#include "MServer.hpp"



void MServer::Connect()
{
    server = WiFiServer(80);

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Trying to connect");
        WiFi.begin(ssid, password);
        // give time to connect
        delay(3000);
    }
    Serial.println("Conneceted");
    server.begin();
    IPAddress localIP = WiFi.localIP();
    ip = localIP.toString().c_str();;

    Serial.print("IP Address: ");
    Serial.println(localIP);
}

bool MServer::Connected()
{
    return client.connected();
}

std::string MServer::ReadContent()
{    
    while (client.connected())
    {
        Serial.println("connected");
        int size = client.available();
        if (size)
        {
            char buffer[size];
            client.readBytes(buffer, size);
            return std::string(buffer);
        }else{
            return std::string("");
        }
    }
}

void MServer::Close(){
    client.flush();
    client.stop();
}

void MServer::Listen()
{
    std::string request = "";
    client = server.available();
    if (client.connected())
    {
        Serial.println("new client");  
    }
}

void MServer::ServeContent(const char* content)
{
    client.println(content);

    client.flush();
}