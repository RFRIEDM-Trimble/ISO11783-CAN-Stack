//================================================================================================
/// @file can_callbacks.cpp
///
/// @brief An object to represent common callbacks used within this CAN stack.
/// @author Adrian Del Grosso
///
/// @copyright 2022 Adrian Del Grosso
//================================================================================================
#include "can_callbacks.hpp"

namespace isobus
{
	ParameterGroupNumberCallbackData::ParameterGroupNumberCallbackData(std::uint32_t parameterGroupNumber, CANLibCallback callback, void *parentPointer) :
	  mCallback(callback),
	  mParameterGroupNumber(parameterGroupNumber),
	  mParent(parentPointer)
	{
	}

	ParameterGroupNumberCallbackData::ParameterGroupNumberCallbackData(const ParameterGroupNumberCallbackData &oldObj)
	{
		mCallback = oldObj.mCallback;
		mParameterGroupNumber = oldObj.mParameterGroupNumber;
		mParent = oldObj.mParent;
	}

	bool ParameterGroupNumberCallbackData::operator==(const ParameterGroupNumberCallbackData &obj)
	{
		return ((obj.mCallback == this->mCallback) &&
		        (obj.mParameterGroupNumber == this->mParameterGroupNumber) &&
		        (obj.mParent == this->mParent));
	}

	ParameterGroupNumberCallbackData &ParameterGroupNumberCallbackData::operator=(const ParameterGroupNumberCallbackData &obj)
	{
		mCallback = obj.mCallback;
		mParameterGroupNumber = obj.mParameterGroupNumber;
		mParent = obj.mParent;
		return *this;
	}

	std::uint32_t ParameterGroupNumberCallbackData::get_parameter_group_number() const
	{
		return mParameterGroupNumber;
	}

	CANLibCallback ParameterGroupNumberCallbackData::get_callback() const
	{
		return mCallback;
	}

	void *ParameterGroupNumberCallbackData::get_parent() const
	{
		return mParent;
	}
} // namespace isobus
