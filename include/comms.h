#include <LoRa.h>

class LoRa
{
public:
    LoRa(double frequency, int ss, int rst, int dio0)
    {
        frequency = _frequency;
        ss = _ss;
        rst = _rst;
        dio0 = _dio0;

        initialize();
    }

    template <typename remoteData>
    void sendData(remoteData data) // sends data to ground station, all data will be send in a new line
    {
        LoRa.beginPacket();
        LoRa.print(String(data));
        LoRa.endPacket();
    }

    String recieveData()
    {
        int packetSize = LoRa.parsePacket();
        if (packetSize)
        {
            while (LoRa.available())
            {
                return LoRa.readString();
            }
        }
    }
    
private:
    double _frequency;
    int _ss;
    int _rst;
    int _dio0;

    void initialize() {
        Serial.print("Initializing LoRa systems  -  ");

        if (!LoRa.begin(frequency))
        { // initialize ratio at 433 MHz
            Serial.println("ERROR");
            while (1)
                ;
        }

        Serial.println("done.");
    }
};