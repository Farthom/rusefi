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

void canDashboardGT86Gen1(CanCycle cycle) {
  criticalAssertVoid(engineConfiguration->canBaudRate == B500KBPS, "GT86/BRZ requires 500Kbps baud rate");

	if (cycle.isInterval(CI::_10ms)) {
		/* 0x140 */
		{
			CanTxMessage msg(CanCategory::NBC, 0x140, 8);
		}

    /* 0x141 */
    {
      CanTxMessage msg(CanCategory::NBC, 0x141, 8);
      //msg.setShortValue(Sensor::getOrZero(SensorType::Rpm) + 0x8000, 4);
    }

    /* 0x142 */
    {
      CanTxMessage msg(CanCategory::NBC, 0x142, 8);
    }
	}

  if (cycle.isInterval(CI::_20ms)) {
    /* 0x144 */
    {
      CanTxMessage msg(CanCategory::NBC, 0x144, 8);
    }
  }

  if (cycle.isInterval(CI::_50ms)) {
    /* 0x360 */
    {
      CanTxMessage msg(CanCategory::NBC, 0x360, 8);
    }

    /* 0x361 */
    {
      CanTxMessage msg(CanCategory::NBC, 0x361, 8);
    }
  }
}

#endif
