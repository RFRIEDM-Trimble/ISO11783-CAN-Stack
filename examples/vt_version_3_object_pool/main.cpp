#include "can_general_parameter_group_numbers.hpp"
#include "can_hardware_interface.hpp"
#include "can_network_manager.hpp"
#include "can_partnered_control_function.hpp"
#include "iop_file_interface.hpp"
#include "isobus_virtual_terminal_client.hpp"
#include "socket_can_interface.hpp"

#include <csignal>
#include <iostream>
#include <iterator>
#include <memory>

static std::shared_ptr<isobus::InternalControlFunction> TestInternalECU = nullptr;
static std::shared_ptr<isobus::PartneredControlFunction> TestPartnerVT = nullptr;
static std::shared_ptr<isobus::VirtualTerminalClient> TestVirtualTerminalClient = nullptr;
std::vector<isobus::NAMEFilter> vtNameFilters;
const isobus::NAMEFilter testFilter(isobus::NAME::NAMEParameters::FunctionCode, static_cast<std::uint8_t>(isobus::NAME::Function::VirtualTerminal));
static std::vector<std::uint8_t> testPool;
static SocketCANInterface canDriver("can0");

using namespace std;

void signal_handler(int signum)
{
	CANHardwareInterface::stop();
	if (nullptr != TestVirtualTerminalClient)
	{
		TestVirtualTerminalClient->terminate();
	}
	exit(signum);
}

void update_CAN_network()
{
	isobus::CANNetworkManager::CANNetwork.update();
}

void raw_can_glue(isobus::HardwareInterfaceCANFrame &rawFrame, void *parentPointer)
{
	isobus::CANNetworkManager::CANNetwork.can_lib_process_rx_message(rawFrame, parentPointer);
}

void setup()
{
	CANHardwareInterface::set_number_of_can_channels(1);
	CANHardwareInterface::assign_can_channel_frame_handler(0, &canDriver);
	CANHardwareInterface::start();

	CANHardwareInterface::add_can_lib_update_callback(update_CAN_network, nullptr);
	CANHardwareInterface::add_raw_can_message_rx_callback(raw_can_glue, nullptr);

	std::this_thread::sleep_for(std::chrono::milliseconds(250));

	isobus::NAME TestDeviceNAME(0);

	// Make sure you change these for your device!!!!
	// This is an example device that is using a manufacturer code that is currently unused at time of writing
	TestDeviceNAME.set_arbitrary_address_capable(true);
	TestDeviceNAME.set_industry_group(1);
	TestDeviceNAME.set_device_class(0);
	TestDeviceNAME.set_function_code(static_cast<std::uint8_t>(isobus::NAME::Function::SteeringControl));
	TestDeviceNAME.set_identity_number(2);
	TestDeviceNAME.set_ecu_instance(0);
	TestDeviceNAME.set_function_instance(0);
	TestDeviceNAME.set_device_class_instance(0);
	TestDeviceNAME.set_manufacturer_code(64);
	vtNameFilters.push_back(testFilter);

	testPool = isobus::IOPFileInterface::read_iop_file("VT3TestPool.iop");

	if (0 != testPool.size())
	{
		std::cout << "Loaded object pool from VT3TestPool.iop" << std::endl;
	}
	else
	{
		std::cout << "Failed to load object pool from VT3TestPool.iop" << std::endl;
	}

	TestInternalECU = std::make_shared<isobus::InternalControlFunction>(TestDeviceNAME, 0x1C, 0);
	TestPartnerVT = std::make_shared<isobus ::PartneredControlFunction>(0, vtNameFilters);
	TestVirtualTerminalClient = std::make_shared<isobus::VirtualTerminalClient>(TestPartnerVT, TestInternalECU);
	TestVirtualTerminalClient->set_object_pool(0, isobus::VirtualTerminalClient::VTVersion::Version3, testPool.data(), testPool.size());
	std::signal(SIGINT, signal_handler);
}

int main()
{
	setup();
	TestVirtualTerminalClient->initialize(true);

	while (true)
	{
		// CAN stack runs in other threads. Do nothing forever.
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

	CANHardwareInterface::stop();
	return 0;
}
