#ifndef LASERDEVICEADAPTER_H
#define LASERDEVICEADAPTER_H

#include "DeviceBase.h"
#include <string>
#include "MMDevice.h"

#define ERR_PORT_CHANGE_FORBIDDEN    10004
#define DEVICE_ERR_INVALID_PROPERTY_VALUE

class LaserDevice : public CShutterBase<LaserDevice> {
public:
    LaserDevice();
    ~LaserDevice();

    int Initialize() override;
    int Shutdown() override;

    void GetName(char* name) const override;
    void GetName(char* name, const int maxChars) const;

    int OnPort(MM::PropertyBase* pProp, MM::ActionType eAct);
    int TTLToggle(MM::PropertyBase* pProp, MM::ActionType eAct);
    int TTLToggle2(MM::PropertyBase* pProp, MM::ActionType eAct);
    int TTLToggle3(MM::PropertyBase* pProp, MM::ActionType eAct);
    int SetIntensity1(long intensity);
    int OnIntensity1(MM::PropertyBase* pProp, MM::ActionType eAct);
    int AnalogToggle(MM::PropertyBase* pProp, MM::ActionType eAct);
   

    bool Busy() override;
    int GetOpen(bool& open) override;
    int Fire(double deltaT) override;
    int SetOpen(bool open = true) override;

    // Additional methods
    void sendSerialCommand(const std::string& command);

    // Methods to set/get COM port and baud rate
    void SetCOMPort(const std::string& port);
    std::string GetCOMPort() const;

    void SetBaudRate(int baudRate);
    int GetBaudRate() const;

private:
    void CloseSerialPort();
    int InitializeSerialPort();

    HANDLE serialPortHandle_;
    std::string portName_; // COM port
    int baudRate_;         // Baud rate
    bool initialized_;
    int TTLOn_;
    int TTLOn2_;
    int TTLOn3_;
    int AnalogValue_;
    int analogOn_;
    long intensity1_;
};

// Micro-Manager module API functions
MODULE_API void InitializeModuleData();
MODULE_API MM::Device* CreateDevice(const char* deviceName);
MODULE_API void DeleteDevice(MM::Device* pDevice);

#endif // LASERDEVICEADAPTER_H
MODULE_API void InitializeModuleData();
MODULE_API MM::Device* CreateDevice(const char* deviceName);
MODULE_API void DeleteDevice(MM::Device* pDevice);

#endif // LASERDEVICEADAPTER_H
