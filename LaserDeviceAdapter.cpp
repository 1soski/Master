// LaserDeviceAdapter.cpp

#include "pch.h"
#include "LaserDeviceAdapter.h"
#include "ModuleInterface.h"
#include <windows.h>
#include <iostream> // Include iostream for cerr

// Define Micro-Manager module API functions
MODULE_API void InitializeModuleData() {
    RegisterDevice("LaserDevice", MM::ShutterDevice, "Laser Device");
}

MODULE_API MM::Device* CreateDevice(const char* deviceName) {
    if (deviceName == nullptr)
        return nullptr;

    if (strcmp(deviceName, "LaserDevice") == 0) {
        return new LaserDevice();
    }

    return nullptr;
}

MODULE_API void DeleteDevice(MM::Device* pDevice) {
    delete pDevice;
}

// Implementation of LaserDevice class methods
LaserDevice::LaserDevice() : initialized_(false), portName_("COM5"), baudRate_(9600), serialPortHandle_(INVALID_HANDLE_VALUE), TTLOn_(0), TTLOn2_(0), TTLOn3_(0), analogOn_(0), intensity1_(0) {
    CPropertyAction* pAct = new CPropertyAction(this, &LaserDevice::OnPort);
    CreateProperty(MM::g_Keyword_Port, "Undefined", MM::String, false, pAct, true);

}

LaserDevice::~LaserDevice() {
    Shutdown();
}

int LaserDevice::Initialize() {
    if (initialized_)
        return DEVICE_OK;

    CPropertyAction* pAct = new CPropertyAction(this, &LaserDevice::TTLToggle);
    CreateProperty("TTLToggle", "0", MM::Integer, false, pAct);

    AddAllowedValue("TTLToggle", "0");
    AddAllowedValue("TTLToggle", "1");

    pAct = new CPropertyAction(this, &LaserDevice::TTLToggle2);
    CreateProperty("TTLToggle2", "0", MM::Integer, false, pAct);

    AddAllowedValue("TTLToggle2", "0");
    AddAllowedValue("TTLToggle2", "1");

    pAct = new CPropertyAction(this, &LaserDevice::TTLToggle3);
    CreateProperty("TTLToggle3", "0", MM::Integer, false, pAct);

    AddAllowedValue("TTLToggle3", "0");
    AddAllowedValue("TTLToggle3", "1");

    //pAct = new CPropertyAction(this, &LaserDevice::AnalogToggle);
    //CreateProperty("AnalogToggle", "0", MM::Integer, false, pAct);

    pAct = new CPropertyAction(this, &LaserDevice::OnIntensity1);
    CreateProperty("OnIntensity1", "4095", MM::Integer, false, pAct);
    
    SetPropertyLimits("OnIntensity1", 0, 4095);

    SetIntensity1(4095);

    initialized_ = true;
    return DEVICE_OK;
}

int LaserDevice::Shutdown() {
    if (!initialized_)
        return DEVICE_OK;

    CloseSerialPort();
    initialized_ = false;
    return DEVICE_OK;
}

void LaserDevice::GetName(char* name) const {
    strncpy(name, "LaserDevice", MM::MaxStrLength);
}

void LaserDevice::GetName(char* name, const int maxChars) const {
    strncpy(name, "LaserDevice", maxChars);
}

int LaserDevice::OnPort(MM::PropertyBase* pProp, MM::ActionType eAct)
{
    if (eAct == MM::BeforeGet)
    {
        pProp->Set(portName_.c_str());
    }
    else if (eAct == MM::AfterSet)
    {
        if (initialized_)
        {
            // revert
            pProp->Set(portName_.c_str());
            return ERR_PORT_CHANGE_FORBIDDEN;
        }

        pProp->Get(portName_);
    }
    return DEVICE_OK;
}

int LaserDevice::TTLToggle(MM::PropertyBase* pProp, MM::ActionType eAct)
{
    if (eAct == MM::BeforeGet)
    {
        pProp->Set("TTLOn");

    }
    else if (eAct == MM::AfterSet) {
        const char* command;
        if (TTLOn_ == 0) {
            command = "turnTTLOn1\n";
            TTLOn_ = 1;
        }
        else {
            command = "turnTTLOff1\n";
            TTLOn_ = 0;
        }
        long ttl = (long)TTLOn_;
       
        SendSerialCommand(portName_.c_str(), command, "\n");
         pProp->Get(ttl);

    }
    return DEVICE_OK;
}

int LaserDevice::TTLToggle2(MM::PropertyBase* pProp, MM::ActionType eAct)
{
    if (eAct == MM::BeforeGet)
    {
        pProp->Set("TTLOn2");

    }
    else if (eAct == MM::AfterSet) {
        const char* command;
        if (TTLOn2_ == 0) {
            command = "turnTTLOn2\n";
            TTLOn2_ = 1;
        }
        else {
            command = "turnTTLOff2\n";
            TTLOn2_ = 0;
        }
        long ttl2 = (long)TTLOn2_;
    
        SendSerialCommand(portName_.c_str(), command, "\n");
        pProp->Get(ttl2);      
    }
    return DEVICE_OK;
}

int LaserDevice::TTLToggle3(MM::PropertyBase* pProp, MM::ActionType eAct)
{
    if (eAct == MM::BeforeGet)
    {
        pProp->Set("TTLOn3");

    }
    else if (eAct == MM::AfterSet) {
        const char* command;
        if (TTLOn3_ == 0) {
            command = "turnTTLOn3\n";
            TTLOn3_ = 1;
        }
        else {
            command = "turnTTLOff3\n";
            TTLOn3_ = 0;
        }
        long ttl3 = (long)TTLOn3_;
        
        SendSerialCommand(portName_.c_str(), command, "\n");
        pProp->Get(ttl3);   
    }
    return DEVICE_OK;
}

int LaserDevice::AnalogToggle(MM::PropertyBase* pProp, MM::ActionType eAct)
{
    const char* command;
    if (analogOn_ == 0) {
        command = "turnAnalogOn1\n";
        analogOn_ = 1;
    }
    else {
        command = "turnAnalogOff1\n";
        analogOn_ = 0;
    }
    SendSerialCommand(portName_.c_str(), command, "\n");
    return DEVICE_OK;
}

int LaserDevice::SetIntensity1(long intensity)
{
    std::stringstream ss;

    // Append the current intensity to the stringstream
    ss << "I" << intensity;

    // Prepend "setAnalog1 " before the existing stringstream content
    std::string command = "setAnalog1 " + ss.str();

    PurgeComPort(portName_.c_str());
    int ret = SendSerialCommand(portName_.c_str(), command.c_str(), "\n");
    if (ret != DEVICE_OK)
        return ret;

    std::string answ = "";
    ret = GetSerialAnswer(portName_.c_str(), "\r\n", answ);
    if (ret != DEVICE_OK)
        return ret;

    intensity1_ = atoi(answ.c_str());

    return DEVICE_OK;
}

int LaserDevice::OnIntensity1(MM::PropertyBase* pProp, MM::ActionType eAct)
{

    if (eAct == MM::BeforeGet)
    {
        // instead of relying on stored state we could actually query the device
        pProp->Set(intensity1_);
    }
    else if (eAct == MM::AfterSet)
    {
        long pos;
        pProp->Get(pos);
        return SetIntensity1(pos);
    }
    return DEVICE_OK;
}

bool LaserDevice::Busy() {
    return false; // Modify as needed
}

int LaserDevice::GetOpen(bool& open) {
    // Implementation of GetOpen method
    return DEVICE_OK; // Modify as needed
}

int LaserDevice::Fire(double deltaT) {
    // Implementation of Fire method
    return DEVICE_OK; // Modify as needed
}

int LaserDevice::SetOpen(bool open) {
    // Implementation of SetOpen method
    return DEVICE_OK; // Modify as needed
}

void LaserDevice::sendSerialCommand(const std::string& command) {
    DWORD bytesWritten;
    WriteFile(serialPortHandle_, command.c_str(), static_cast<DWORD>(command.size()), &bytesWritten, NULL);
    // Add error handling if needed
}

// Implementation of methods to set/get COM port and baud rate
void LaserDevice::SetCOMPort(const std::string& port) {
    portName_ = port;
}

std::string LaserDevice::GetCOMPort() const {
    return portName_;
}

void LaserDevice::SetBaudRate(int baudRate) {
    baudRate_ = baudRate;
}

int LaserDevice::GetBaudRate() const {
    return baudRate_;
}

int LaserDevice::InitializeSerialPort() {
    serialPortHandle_ = CreateFileW(LPCWSTR(portName_.c_str()), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (serialPortHandle_ == INVALID_HANDLE_VALUE) {
        // Handle error
        return DEVICE_ERR;
    }

    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(serialPortHandle_, &dcbSerialParams)) {
        // Handle error
        CloseHandle(serialPortHandle_);
        serialPortHandle_ = INVALID_HANDLE_VALUE;
        return DEVICE_ERR;
    }

    dcbSerialParams.BaudRate = baudRate_;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(serialPortHandle_, &dcbSerialParams)) {
        // Handle error
        CloseHandle(serialPortHandle_);
        serialPortHandle_ = INVALID_HANDLE_VALUE;
        return DEVICE_ERR;
    }

    return DEVICE_OK;
}

void LaserDevice::CloseSerialPort() {
    if (serialPortHandle_ != INVALID_HANDLE_VALUE) {
        CloseHandle(serialPortHandle_);
        serialPortHandle_ = INVALID_HANDLE_VALUE;
    }
}

