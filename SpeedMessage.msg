import veins.modules.messages.BaseFrame1609_4;
namespace veins;
packet SpeedMessage extends BaseFrame1609_4
{
    // The speed of the car in m/s
    double speed;
    // The ID of the sending vehicle
    string senderId;
}
