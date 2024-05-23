// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef msr_airlib_CommonEnums_hpp
#define msr_airlib_CommonEnums_hpp

#include "common/Common.hpp"
#include <ostream>

namespace msr
{
namespace airlib
{
	enum class EObjectAirsimTag
	{
		OBJECT_AIRSIM_TAG_NONE			= 0,
		OBJECT_AIRSIM_TAG_TRAM			= 1,
		OBJECT_AIRSIM_TAG_TRAMTRACKS	= 2,
		OBJECT_AIRSIM_TAG_PEDESTRIANS	= 3,
		OBJECT_AIRSIM_TAG_CARS			= 4,
		OBJECT_AIRSIM_TAG_TRUCKS		= 5,
		OBJECT_AIRSIM_TAG_TRAILERS		= 6,
		OBJECT_AIRSIM_TAG_BUSSES		= 7,
		OBJECT_AIRSIM_TAG_BICYCLES		= 8,
		OBJECT_AIRSIM_TAG_SCOOTERS		= 9,
		OBJECT_AIRSIM_TAG_MOTORCYCLES	= 10,
		OBJECT_AIRSIM_TAG_BUILDINGS		= 11,
		OBJECT_AIRSIM_TAG_SIGNS_POLE	= 12,
		OBJECT_AIRSIM_TAG_SIGNS_MAIN	= 13,
		OBJECT_AIRSIM_TAG_LAST_EXCLUDE  = 14
	};
}
} //namespace
#endif
