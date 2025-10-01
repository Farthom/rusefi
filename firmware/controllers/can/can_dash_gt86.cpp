/**
 * @file	can_dash_gt86.cpp
 *
 * @date Sept 29, 2025
 * @author Cobalt the Cat, (c) 2025
 */

#include "pch.h"

#if EFI_CAN_SUPPORT
#include "can.h"
#include "can_msg_tx.h"

void canDashboardGt86(CanCycle cycle) {
  criticalAssertVoid(engineConfiguration->canBaudRate == B500KBPS, "GT86/BRZ requires 500Kbps baud rate");

	if (cycle.isInterval(CI::_10ms)) {
		/* 0x140 */
		{
			CanTxMessage msg(CanCategory::NBC, 0x140, 8);
      msg.setShortValue(Sensor::getOrZero(SensorType::Rpm) + 0x8000, 4);
		}
	}	
}

#endif
