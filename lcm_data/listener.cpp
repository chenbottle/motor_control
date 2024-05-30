
#include <stdio.h>
#include <fstream>

#include <lcm/lcm-cpp.hpp>
#include "OD_Motor_Msg_.hpp"
#include "imu_data.hpp"

using namespace std;

class Handler {
public:
    ofstream* imu_file;
    Handler(){
        imu_file = new ofstream();
        imu_file->open("imu_data",ios::out);
        *imu_file << "roll:\t" << "pitch:\t" << "yaw:\t" << endl;
    }
    ~Handler() {
        imu_file->close();
    }
    void handleMessage(const lcm::ReceiveBuffer *rbuf, const std::string &chan,
                         const imu_data *imu)//const OD_Motor_Msg_ *msg,
    {       
        *imu_file << imu->roll << "\t" << imu->pitch << "\t" << imu->yaw << endl;
        // printf("Received message on channel %s:\n", chan.c_str());
        // for (int i = 0;i < 6;i++)
        // {
        //     printf("motor[%d]speed:%f\tpoi:%f\tcur:%f\ttem:%d\n",msg->motor_id[i],msg->speed_actual_rad[i]
        //     ,msg->angle_actual_rad[i],msg->current_actual_float[i],msg->temperature[i]);
        // }
        // printf("\n");
    }
};

int main(int argc, char **argv)
{
    lcm::LCM lcm;  

    if (!lcm.good())
        return 1;

    Handler handlerObject;
    lcm.subscribe("motor_data", &Handler::handleMessage, &handlerObject);

    while (0 == lcm.handle()) {
        // Do nothing
    }

    return 0;
}
