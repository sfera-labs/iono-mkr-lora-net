/*
  MkrWanLoRaNetMaster.ino - LoRaNet master example for Iono MKR WAN

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

// LoRaNet keys
byte siteId[] = "LOL";
byte cryptoKey[] = "16bytesSecretKey";

// iono2 is not used in this example setup.
// To add it to the net, program a slave with address 2
// and add it to *slaves below.
IonoLoRaRemoteSlave iono2 = IonoLoRaRemoteSlave(2);
IonoLoRaRemoteSlave iono3 = IonoLoRaRemoteSlave(3);
LoRaRemoteSlave *slaves[] = { &iono3 };

IonoLoRaLocalMaster master = IonoLoRaLocalMaster();

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Starting...");

  if (!LoRa.begin(868E6)) {
    Serial.println("LoRa init failed.");
    while (true);
  }

  LoRa.enableCrc();
  LoRa.setSyncWord(0x34);

  LoRaNet.init(siteId, sizeof(siteId) - 1, cryptoKey);

  // pass the array of slaves pointers and its size
  master.setSlaves(slaves, 1);

  Serial.println("LoRa init succeeded.");
}

unsigned long ts = millis();
int do1 = 0;

void loop() {
  master.process();

  if (millis() - ts >= 1000) {
    Serial.println("---------");
    Serial.print("DO1 ");
    Serial.println(iono3.read(DO1));
    Serial.print("DO2 ");
    Serial.println(iono3.read(DO2));
    Serial.print("DO3 ");
    Serial.println(iono3.read(DO3));
    Serial.print("DO4 ");
    Serial.println(iono3.read(DO4));

    Serial.print("DI1 ");
    Serial.println(iono3.read(DI1));
    Serial.print("DI2 ");
    Serial.println(iono3.read(DI2));
    Serial.print("DI3 ");
    Serial.println(iono3.read(DI3));
    Serial.print("DI4 ");
    Serial.println(iono3.read(DI4));

    Serial.print("AV3 ");
    Serial.println(iono3.read(AV3));
    Serial.print("AI3 ");
    Serial.println(iono3.read(AI3));
    Serial.print("AV4 ");
    Serial.println(iono3.read(AV4));
    Serial.print("AI4 ");
    Serial.println(iono3.read(AI4));

    Serial.print("RSSI ");
    Serial.println(iono3.loraRssi());
    Serial.print("SNR ");
    Serial.println(iono3.loraSnr());
    Serial.print("AGE ");
    Serial.println(iono3.stateAge());

    do1 = (do1 + 1) % 2;
    iono3.write(DO1, do1);
    iono3.write(AO1, do1 * 5);

    ts = millis();
  }
}
