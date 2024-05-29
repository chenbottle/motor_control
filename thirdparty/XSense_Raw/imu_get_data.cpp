#include "imu_get_data.h"
#include <unistd.h>
#include <sys/time.h>

Journaller* gJournal = 0;

int IMU::initIMU(){
    //Journaller* gJournal = 0;
    //cout << "Creating XsControl object..." << endl;
    _control = XsControl::construct();
    cout<<_control<<endl;
    assert(_control != 0);

// Lambda function for error handling
    auto handleError = [=](string errorString)
    {
        _control->destruct();
        cout << errorString << endl;
        cout << "Press ..[ENTER].. to continue." << endl;
        cin.get();  //暂停到回车键
        return -1;
    };    
//cout << "Scanning for devices..." << endl;  //可能有问题
    XsPortInfoArray portInfoArray = XsScanner::scanPorts();

// Find an MTi _device    
    for (auto const &portInfo : portInfoArray){
        if (portInfo.deviceId().isMti() || portInfo.deviceId().isMtig()){
            _mtPort = portInfo;
            break;
        }
    }    
    if (_mtPort.empty())
        return handleError("No MTi _device found. ..Aborting..");

//cout << "Found a _device with ID: " << _mtPort.deviceId().toString().toStdString() << " @ port: " << _mtPort.portName().toStdString() << ", baudrate: " << _mtPort.baudrate() << endl;

//cout << "Opening port..." << endl;
    if (!_control->openPort(_mtPort.portName().toStdString(), _mtPort.baudrate()))
        return handleError("Could not open port. Aborting.");

// Get the _device object
    _device = _control->device(_mtPort.deviceId());
    assert(_device != 0);

//cout << "Device: " << _device->productCode().toStdString() << ", with ID: " << _device->deviceId().toString() << " opened." << endl;

// Create and attach _callback handler to _device
    //CallbackHandler _callback;
    _device->addCallbackHandler(&_callback);

// Put the _device into configuration mode before configuring the _device
//cout << "Putting _device into configuration mode..." << endl;
    if (!_device->gotoConfig())
        return handleError("Could not put _device into configuration mode. Aborting.");

    cout << "Configuring the _device..." << endl;

// Important for Public XDA!
// Call this function if you want to record a mtb file:
    _device->readEmtsAndDeviceConfiguration();

    XsOutputConfigurationArray configArray;
    configArray.push_back(XsOutputConfiguration(XDI_PacketCounter, 0));
    configArray.push_back(XsOutputConfiguration(XDI_SampleTimeFine, 0));



    if (_device->deviceId().isImu())
    {
        cout << "*************isImu*********************" << endl;
        configArray.push_back(XsOutputConfiguration(XDI_DeltaV, 0));//速度变化值
        configArray.push_back(XsOutputConfiguration(XDI_DeltaQ, 0));//角度变化值
        configArray.push_back(XsOutputConfiguration(XDI_MagneticField, 0));
        configArray.push_back(XsOutputConfiguration(XDI_RateOfTurn, 0));
        configArray.push_back(XsOutputConfiguration(XDI_Acceleration,400));
    }
    else if (_device->deviceId().isVru() || _device->deviceId().isAhrs())
    {
        //AHRS:航姿参考系统，由加速度计、磁场计、陀螺仪组成
        //VRU：动态倾角单元
        cout << "*************isVru  isAhrs********************" << endl;
        configArray.push_back(XsOutputConfiguration(XDI_Quaternion, 0));
    }
    else if (_device->deviceId().isGnss())
    {
        cout << "*************isGnss********************" << endl;
        configArray.push_back(XsOutputConfiguration(XDI_Acceleration,0));
        configArray.push_back(XsOutputConfiguration(XDI_Quaternion, 0));
        configArray.push_back(XsOutputConfiguration(XDI_LatLon, 0));
        configArray.push_back(XsOutputConfiguration(XDI_AltitudeEllipsoid, 0));
        configArray.push_back(XsOutputConfiguration(XDI_VelocityXYZ, 0));

        configArray.push_back(XsOutputConfiguration(XDI_MagneticField, 0));
        configArray.push_back(XsOutputConfiguration(XDI_RateOfTurn, 0));
        configArray.push_back(XsOutputConfiguration(XDI_Acceleration,0));
    }
    else
    {
        cout << "*************UNknow********************" << endl;
        return handleError("Unknown _device while configuring. Aborting.");
    }

    if (!_device->setOutputConfiguration(configArray))
        return handleError("Could not configure MTi _device. Aborting.");

    if (!_device->gotoMeasurement())
        return handleError("Could not put _device into measurement mode. Aborting.");

    if (!_device->startRecording())
        return handleError("Failed to start recording. Aborting.");


    cout << string(79, '-') << endl;

    sleep(2);//弃用前三秒数据
}

int IMU::imuGet()
{
    //int64_t startTime = XsTime::timeStampNow();
//while (XsTime::timeStampNow() - startTime <= 10000)
//{
    if (_callback.packetAvailable())
    {

        cout <<  setw(4) << fixed << setprecision(4);//精度位数和保留小数位数
        imuData.timeStamp_ms = XsTime::timeStampNow();

        //获取此时的PC时间
        struct timeval tv;
        gettimeofday(&tv,NULL);
        imuData.PCtime_us = tv.tv_sec*1000000 + tv.tv_usec;   //获取PC的秒数,时间是微秒

        // Retrieve a packet
        XsDataPacket packet = _callback.getNextPacket();
        if (packet.containsCalibratedData())
        {

            XsVector acc = packet.calibratedAcceleration();
            imuData.acceleration = { acc[0],acc[1],acc[2]};//加速度：m/s*s


            XsVector gyr = packet.calibratedGyroscopeData();
            imuData.angularVelo = { gyr[0],gyr[1],gyr[2]};//陀螺仪单位：rad/s

        }

        if (packet.containsOrientation())
        {
            XsQuaternion quaternion = packet.orientationQuaternion();
            imuData.Quaternion = {quaternion.w(), quaternion.x(),quaternion.y(),quaternion.z()};

            XsEuler euler = packet.orientationEuler();//z-y-x欧拉角
            imuData.roll= euler.roll();
            imuData.pitch = euler.pitch();
            imuData.yaw = euler.yaw();


        }

        if (packet.containsLatitudeLongitude())
        {
            XsVector latLon = packet.latitudeLongitude();
            cout << " |Lat:" << latLon[0]
                 << ", Lon:" << latLon[1];

        }
        if (packet.containsVelocity())
        {
            XsVector vel = packet.velocity(XDI_CoordSysEnu);
            imuData.velocity = {vel[0],vel[1],vel[2]};

        }
        cout << flush;//将缓冲区的数据全部输出到终端
    }

    return 1;
}
