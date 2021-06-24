#include <Arduino.h>
#include <mcp2515.h>

MCP2515 mcp2515(10);

struct can_frame speedMessage, firstRowMessage, secondRowMessage;

void initFISFrames()
{
    firstRowMessage.can_id = 0x261;
    firstRowMessage.can_dlc = 8;
    secondRowMessage.can_id = 0x263;
    secondRowMessage.can_dlc = 8;
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
}

void loop()
{
}