#include <Arduino.h>
#include <mcp2515.h>

MCP2515 mcp2515(10);

struct can_frame speedMessage, firstRowMessage, secondRowMessage;

int speed, uv, lv = 0;

const uint8_t SPACE = 0x20;
const uint8_t ASCII[10] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};
uint8_t KMH_MESSAGE[8] = {SPACE, SPACE, 0x4b, 0x4d, 0x2f, 0x48, SPACE, SPACE};

void initFISFrames()
{
    firstRowMessage.can_id = 0x261;
    firstRowMessage.can_dlc = 8;
    secondRowMessage.can_id = 0x263;
    secondRowMessage.can_dlc = 8;
}

void insertData(can_frame &message, canid_t id, uint8_t data[])
{
    message.can_id = id;
    for (int i = 0; i < 8; i++)
    {
        message.data[i] = data[i];
    }
}

void splitDigits()
{
    if (speed < 10)
    {
        firstRowMessage.data[2] = ASCII[speed];
    }
    else if (speed < 100)
    {
        firstRowMessage.data[2] = ASCII[speed / 10];
        firstRowMessage.data[3] = ASCII[speed % 10];
    }
    else
    {
        firstRowMessage.data[2] = ASCII[speed / 100];
        firstRowMessage.data[3] = ASCII[(speed / 10) % 10];
        firstRowMessage.data[4] = ASCII[speed % 10];
    }
}

void sendMessage()
{
    mcp2515.sendMessage(&firstRowMessage);
    mcp2515.sendMessage(&secondRowMessage);
}

void setup()
{
    mcp2515.reset();
    mcp2515.setBitrate(CAN_100KBPS, MCP_8MHZ);
    mcp2515.setNormalMode();
    initFISFrames();
    insertData(secondRowMessage, 0x263, KMH_MESSAGE);
}

void loop()
{
    if (mcp2515.readMessage(&speedMessage) == MCP2515::ERROR_OK)
    {
        if (speedMessage.can_id == 0x351)
        {
            lv = speedMessage.data[1];
            uv = speedMessage.data[2];
            speed = ((uv << 8) + lv - 1) / 190;
            splitDigits();
            //sendMessage();
        }
    }
    delay(10);
}