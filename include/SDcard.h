#include <SD.h>

class SDcard
{
public:
    SDcard(byte chipSelect)
    {
        _chipSelect = chipSelect;   // SD card CS pin

        initialize();
    }

    void clear()
    {
        File dataFile = SD.open("DATALOG.txt", FILE_WRITE | O_TRUNC);

        if (dataFile)
        {
            dataFile.println("");
            dataFile.close();
        }
        else
        {
            Serial.println("ERROR opening DATALOG.txt");
        }
    }

    template <typename writeData>
    void write(writeData data)
    {
        File dataFile = SD.open("DATALOG.txt", FILE_WRITE);

        if (dataFile)
        {
            dataFile.print(data);
            dataFile.close();
        }
        else
        {
            Serial.println("ERROR opening DATALOG.txt");
        }
    }

    template <typename writeDataLn>
    void writeln(writeDataLn data)
    {
        File dataFile = SD.open("DATALOG.txt", FILE_WRITE);

        if (dataFile)
        {
            dataFile.println(data);
            dataFile.close();
        }
        else
        {
            Serial.println("ERROR opening DATALOG.txt");
        }
    }
private:
    byte _chipSelect;

    void initialize() {
        Serial.print("Initializing SD card  -  ");
        // SendData("Initializing SD card  -  ");

        if (!SD.begin(_chipSelect))
        {
            Serial.println("ERROR");
            // SendData("ERROR");
            while (1)
                ;
        }
        Serial.println("done.");
        // SendData("done.");
    }
};