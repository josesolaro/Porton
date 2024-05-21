#include "Porton.hpp"
#include <Arduino.h>
void Porton::Init(RFID* rfid, MServer* httpserver)
{
    rfID = rfid;
    httpServer = httpserver;

    httpServer->Connect();
    rfID->Setup();
}

void Porton::Open()
{
    if (status == stop || status == closed)
    {
        SetStatus(opening);
    }
}

void Porton::Close()
{
    if (status == stop || status == opened)
    {
        SetStatus(closing);
    }
}

void Porton::Stop()
{
    if (digitalRead(APERTURA) == 0)
    {
        SetStatus(opened);
    }
    else if (digitalRead(CIERRE) == 0)
    {
        SetStatus(closed);
    }
    else
    {
        SetStatus(stop);
    }
}

void Porton::CheckCurrent()
{
    if (status == opening || status == closing)
    {
        uint16_t oldValue = 0;
        uint16_t newValue = 0;
        for(;;)
        {
            newValue = analogRead(CURRENT);
            if (oldValue > newValue)
            {
                break;
            }
            oldValue = newValue;
            delay(1);
        }
        if(oldValue > MAX_CURRENT){
            Stop();
        }
    }
}

void Porton::SetStatus(PortonStatus newStatus)
{
    digitalWrite(CLOSE_OUTPUT, LOW);
    digitalWrite(OPEN_OUTPUT, LOW);
    switch (newStatus)
    {
    case opening:
        Serial.println("OPENING");
        digitalWrite(OPEN_OUTPUT, HIGH);
        break;
    case closing:
        Serial.println("CLOSING");
        digitalWrite(CLOSE_OUTPUT, HIGH);
        break;
    default:
        Serial.println("STOP");
        digitalWrite(CLOSE_OUTPUT, LOW);
        digitalWrite(OPEN_OUTPUT, LOW);
    }
    status = newStatus;
}

char *Porton::GetStatus()
{
    switch (status)
    {
    case opening:
        return (char *)"Abriendo";
    case closing:
        return (char *)"Cerrando";
    case stop:
        return (char *)"Detenido";
    case opened:
        return (char *)"Abierta";
    case closed:
        return (char *)"Cerrado";
    default:
        return (char *)"Error";
        break;
    }
}

void Porton::ListenCmd()
{
    WiFiClient* client  = nullptr;
    httpServer->Listen();
    if(httpServer->Connected())
    {
        std::string request = httpServer->ReadContent();
        if(request.length() > 0)
        {
            Request(request);
            httpServer->ServeContent(GetContent());
        }else{
            httpServer->Close();
        }

    }
    while(httpServer->Connected()){
        httpServer->Close();
    }

    if(rfID->Read()){
        Open();
        Close();
    }
    CheckCurrent();
}

void Porton::Request(std::string command)
{
    std::string cmd = command.substr(0, command.find('\n') - 1);
    const char *c = cmd.substr(cmd.find(' ') + 1, cmd.rfind(' ') - 4).c_str();

    if (strcmp(c, openEndpoint) == 0)
    {
        Open();
    }
    else if (strcmp(c, closeEndpoint) == 0)
    {
        Close();
    }
    else if (strcmp(c, stopEndpoint) == 0)
    {
        Stop();
    }
}

const char* Porton::GetContent(){
    char* c = GetStatus();
    std::string content = std::string("HTTP/1.1 200 OK\n"
    "Content-Type: text/html\n"
    "Connection: close\n\n"
    "<!DOCTYPE HTML>\n"
    "<html>\n"
    "<head>\n"
    "<style>\n"
    "button { font-size: 50px; margin: 20px; height:300px }\n"
    "</style>\n"
    "</head>\n"
    "<link rel='shortcut icon' href='about:blank' />\n"
    "<h1 style=\"font-size:50px\">\n");
    content += c;
    content +=
    "</h1>\n"
    "<div style=\"display: flex; flex-direction: column;\">\n"
    "<button onclick=\"location.href='./open'\">Abrir</button>\n"
    "<button onclick=\"location.href='./close'\">Cerrar</button>\n"
    "<button onclick=\"location.href='./stop'\">Detener</button>\n"
    "</div>\n"
    "</html>\n";
    return content.c_str();
}