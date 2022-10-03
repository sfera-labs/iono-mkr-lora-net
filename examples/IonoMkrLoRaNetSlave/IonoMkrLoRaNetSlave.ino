/*
  MkrWanLoRaNetSlave.ino - LoRaNet slave example for Iono MKR WAN

    Copyright (C) 2018-2022 Sfera Labs S.r.l. - All rights reserved.

    For information, see:
    https://www.sferalabs.cc/

  This code is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  See file LICENSE.txt for further informations on licensing terms.
*/

#include <Arduino.h>
#include <LoRa.h>
#include <IonoLoRaNet.h>
#include <Iono.h>

// LoRaNet keys - same as master
byte siteId[] = "LOL";
byte cryptoKey[] = "16bytesSecretKey";

// this slave unit has address 3
IonoLoRaLocalSlave slave = IonoLoRaLocalSlave(3);

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Starting...");

  // set same LoRa parameters as in master

  if (!LoRa.begin(868E6)) {
    Serial.println("LoRa init failed.");
    while (true);
  }

  LoRa.enableCrc();
  LoRa.setSyncWord(0x34);

  LoRaNet.init(siteId, sizeof(siteId) - 1, cryptoKey);

  // subscribe the inputs/outputs whose state you want to send to the master
  // specifying the wanted minimum stable times and minimun value variations

  Iono.subscribeDigital(DO1, 0, &IonoLoRaLocalSlave::subscribeCallback);
  Iono.subscribeDigital(DO2, 0, &IonoLoRaLocalSlave::subscribeCallback);
  Iono.subscribeDigital(DO3, 0, &IonoLoRaLocalSlave::subscribeCallback);
  Iono.subscribeDigital(DO4, 0, &IonoLoRaLocalSlave::subscribeCallback);

  Iono.subscribeAnalog(AO1, 0, 0, &IonoLoRaLocalSlave::subscribeCallback);

  Iono.subscribeDigital(DI1, 50, &IonoLoRaLocalSlave::subscribeCallback);
  Iono.subscribeDigital(DI2, 50, &IonoLoRaLocalSlave::subscribeCallback);
  Iono.subscribeAnalog(AV3, 50, 0.1, &IonoLoRaLocalSlave::subscribeCallback);
  Iono.subscribeAnalog(AI4, 50, 0.1, &IonoLoRaLocalSlave::subscribeCallback);
  Iono.subscribeDigital(DI5, 50, &IonoLoRaLocalSlave::subscribeCallback);
  Iono.subscribeDigital(DI6, 50, &IonoLoRaLocalSlave::subscribeCallback);

  Serial.print("LoRaNet slave ");
  Serial.println(slave.getAddr());
}

unsigned long ts = 0;

void loop() {
  slave.process();

  // just to show it is alive...
  if (millis() - ts >= 1000) {
    Serial.println("---------");
    ts = millis();
  }
}
