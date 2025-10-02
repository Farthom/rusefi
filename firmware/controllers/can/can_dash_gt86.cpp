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

static uint8_t g1EcmCounter = 0; // For 0x140

void canDashboardGT86Gen1(CanCycle cycle) {
  criticalAssertVoid(engineConfiguration->canBaudRate == B500KBPS, "GT86/BRZ requires 500Kbps baud rate");

	if (cycle.isInterval(CI::_10ms)) {
		/* 0x140 */
		{
      //byte dataA[8] = {0x00, 0x00, 0x68, 0x46, 0x00, 0x00, 0x09, 0x21};	//140 10ms extra

      if (g1EcmCounter > 0xF) {
        g1EcmCounter = 0;
      }

			CanTxMessage msg(CanCategory::NBC, 0x140, 8);
      msg[0] = 0; // Pedal pos
      msg[1] = g1EcmCounter;
      // msg[2, 3] = RPM
      msg.setShortValue(Sensor::getOrZero(SensorType::Rpm), 2);
      msg[4] = 0; // Duplicate of 0
      msg[5] = 0; // Duplicate of 0
      msg[6] = 0x09; // Fake ETB pos 3.5% ish
      msg[7] = 0x21; // Unknown flags

      g1EcmCounter++;
		}

    /* 0x141 */
    {
      // byte dataB[8] = {0x8B, 0x26, 0xFA, 0x27, 0xC9, 0x86, 0xA7, 0x00};	//141 10ms
      CanTxMessage msg(CanCategory::NBC, 0x141, 8);
      // Pedal Position 0% ish
      msg[0] = 0x8B;
      msg[1] = 0x26;

      // Engine Load
      msg[2] = 0xFA;
      msg[3] = 0x27;

      // RPM msg[4-5]
      msg.setShortValue(Sensor::getOrZero(SensorType::Rpm), 4);

      msg[6] = 0xA7;
      msg[7] = 0x00;
    }

    /* 0x142 */
    {
      // byte dataC[8] = {0x84, 0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};	//142 10ms extra
      CanTxMessage msg(CanCategory::NBC, 0x142, 8);
      msg[0] = 0x84;
      msg[1] = 0x26;
      msg[2] = 0x00;
      msg[3] = 0x00;
      msg[4] = 0x00;
      msg[5] = 0x00;
      msg[6] = 0x00;
      msg[7] = 0x00;
    }
	}

  if (cycle.isInterval(CI::_20ms)) {
    /* 0x144 */
    {
      // byte dataD[8] = {0xC0, 0x00, 0x4C, 0x17, 0x94, 0xA4, 0xA0, 0x00};	//144 20ms extra
      CanTxMessage msg(CanCategory::NBC, 0x144, 8);
      msg[0] = 0xC0;
      msg[1] = 0x00;
      msg[2] = 0x4C;
      msg[3] = 0x17;
      msg[4] = 0x94;
      msg[5] = 0xA4;
      msg[6] = 0xA0;
      msg[7] = 0x00;
    }
  }

  if (cycle.isInterval(CI::_50ms)) {
    /* 0x360 */
    {
      // byte dataE[8] = {0xD7, 0x00, 0x6F, 0x50, 0x32, 0x00, 0x01, 0x00};	//360 50ms
      CanTxMessage msg(CanCategory::NBC, 0x360, 8);

      msg[0] = 0xD7;
      msg[1] = 0x00;
      msg[2] = 0x6F;

      // Coolant Temp  (88-128Cold 128-148Norm 148-163Hot)
      msg[3] = Sensor::getOrZero(SensorType::Clt) + 38; // Puts 90C - 110C in the "normal" range
      msg[4] = 0x32;
      msg[5] = 0x00;
      msg[6] = 0x01;
      msg[7] = 0x00;
    }

    /* 0x361 */
    {
      // byte dataF[8] = {0x00, 0x29, 0x00, 0xD3, 0x00, 0xD0, 0x73, 0x72};	//361 50ms
      CanTxMessage msg(CanCategory::NBC, 0x361, 8);
      msg[0] = 0x00;

      // Check engine light
      if (Sensor::getOrZero(SensorType::Clt) >= 120) {
        msg[1] = 0x40; // Fast flashing!!!!
      } else {
        msg[1] = 0x20; // OFF
      }

      msg[2] = 0x00;
      msg[3] = 0xD3;

      // CC light
      msg[4] = 0x00;
      msg[5] = 0xD0;
      msg[6] = 0x73;
      msg[7] = 0x72;
    }
  }
}

#endif
