#include <Arduino.h>

const int ENCODING_SIZE = 5;

typedef std::array<bool, ENCODING_SIZE> Encoding;

// Every duration is given in milliseconds
class Display
{
    int digitDuration = 500;
    int outputPins[ENCODING_SIZE] = { 1, 2,7, 6, 5,};
    Encoding errorEncoding = {1, 1, 1, 1, 1};
    Encoding digitEncodings[10] = {
        {1, 0, 0, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0},
        {0, 0, 0, 0, 1},
        {1, 0, 1, 0, 0},
        {0, 1, 0, 1, 0},
        {0, 0, 1, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 1, 0, 1},
    };

    Encoding *getDigitEncoding(int digit)
    {
        if (digit < 0 || digit > 9)
        {
            return &errorEncoding;
        }
        else
        {
            return &digitEncodings[digit];
        }
    }

    void initializeOutputPins()
    {
        for (auto outputPin : outputPins)
        {
            pinMode(outputPin, OUTPUT);
            digitalWrite(outputPin, LOW);
        }
    }

    std::string getEncodingString(Encoding encoding)
    {
        std::string result = "";
        for (auto i = 0; i < ENCODING_SIZE; i++)
        {
            auto value = encoding[i];
            result += std::to_string(value);
        }
        return result;
    }

public:
    Display()
    {
        initializeOutputPins();
    }

    void displayDigit(int digit)
    {
        auto encoding = getDigitEncoding(digit);
        Serial.print("Displaying digit ");
        Serial.print(digit);
        Serial.println();
        displayEncoding(*encoding, digitDuration);
    }

    void clearOutput()
    {
        for (auto outputPin : outputPins)
        {
            digitalWrite(outputPin, LOW);
        }
    }

    void displayEncoding(Encoding encoding, int duration)
    {
        Serial.print("Displaying encoding ");
        Serial.print(getEncodingString(encoding).c_str());
        Serial.println();

        for (auto i = 0; i < ENCODING_SIZE; i++)
        {
            auto outputPin = outputPins[i];
            auto value = encoding[i] ? HIGH : LOW;
            digitalWrite(outputPin, value);
        }
        delay(duration);
        clearOutput();
    }
};