/**
 * @Hardwares: M5StickCPlus2
 * @Platform Version: Arduino M5Stack Board Manager v2.0.9
 * @Dependent Library:
 * M5GFX: https://github.com/m5stack/M5GFX
 * M5Unified: https://github.com/m5stack/M5Unified
 * M5StickCPlus2: https://github.com/m5stack/M5StickCPlus2
 */
#include <Arduino.h>
#include <M5StickCPlus2.h>
#include "ring.hpp"
#include <M5Unified.h>
#include <M5GFX.h>

#include "EEPROM.h"
#include "Preferences.h"
#include <nvs_flash.h>

Preferences preferences;

#define IDLE 0
#define CHANNEL_CHOICE 1
#define SLEEP 2
int state = IDLE;
bool sleep_state = false;

long timerVibration = 0;
long intervalVibration = 0; // 1 second
bool state_vibration = false;

long timerScreenSleep = 0;
long intervalScreenSleep = 10000; // 10 seconds

unsigned long timer_holding = 0;
bool start_holding = false;
int time_screen = 2;

bool flag_start = false;
#include <network_espnow.h>

void initScreen() {
    M5.Display.setRotation(0);
    M5.Display.clear(LIGHTGREY);
    M5.Display.setTextColor(BLACK);
    M5.Display.setTextDatum(CC_DATUM);
    M5.Display.setFont(&Orbitron_Light_24);
    M5.Display.setTextSize(1);
}

void screenBackground() {
    M5.Display.drawString("Batterie", 135/2, 190);
}

void upgradeScreen() {
    M5.Display.fillRect(0, 205, 135, 30, LIGHTGREY);
    M5.Display.drawString(String(StickCP2.Power.getBatteryLevel()), 135/2, 220);
}

void upgradeScreenChannel() {
    M5.Display.fillRect(0, 35, 135, 30, YELLOW);
    M5.Display.fillRect(0, 205, 135, 30, YELLOW);
    M5.Display.fillRect(0, 205, 135, 30, YELLOW);
    M5.Display.drawString(String(channel_number), 135 / 2, 50);
    M5.Display.drawString(String(StickCP2.Power.getBatteryLevel()), 135 / 2, 220);
}

void screenBackgroundChannel() {
    M5.Display.drawString("Channel", 135 / 2, 20);
    M5.Display.drawString("Batterie", 135 / 2, 190);
}


void setup() {

    M5.begin();
    //M5.Display.sleep();
    preferences.begin("my-app", false);
    channel_number = preferences.getInt("channel", 6);

    initScreen();
    screenBackground();
    upgradeScreen();

    Serial.begin(115200);
    //ledcAttach(CTRL_PIN,10000, 8)
    pinMode(32, OUTPUT);

    initESPNOW();
}

void loop() {
    M5.update();

    switch (state) {
        case IDLE:
            if (flag_start) {
                if (intervalVibration != 0) {
                    if (millis() - timerVibration > intervalVibration) {
                        timerVibration = millis();
                        if (state_vibration) {
                            analogWrite(G32,0);
                            state_vibration = false;
                        }
                        else {
                            Serial.println("B");
                            analogWrite(G32,255* encoder_msg.encoder1 / 10);
                            state_vibration = true;
                        }
                    }
                }
                else {
                    analogWrite(G32,255* encoder_msg.encoder1 / 10);
                }
            }

            if (M5.BtnA.wasClicked() && sleep_state == false) {
                Serial.println("BtnA Clicked and sleep_state is false");
                timerScreenSleep = millis();
                analogWrite(G32,0);
                flag_start = false;
            }

            if (M5.BtnA.wasClicked() && sleep_state) {
                Serial.println("BtnA Clicked");
                M5.Display.wakeup();
                delay(100);
                timerScreenSleep = millis();
                initScreen();
                screenBackground();
                upgradeScreen();
                flag_start = false;
                sleep_state = false;
            }

            if (M5.BtnPWR.wasClicked()) {
                Serial.println("BtnPWR Clicked");
                M5.Display.clear(YELLOW);
                screenBackgroundChannel();
                upgradeScreenChannel();
                Serial.println("BtnC was pressed.");
                state = CHANNEL_CHOICE;
                delay(500);
            }
            if (millis() - timerScreenSleep > intervalScreenSleep && sleep_state == false) {
                state = SLEEP;
            }
            break;

        case CHANNEL_CHOICE:
            if (M5.BtnPWR.wasClicked()) {
                Serial.println("BtnPWR Clicked");
                M5.Display.clear(LIGHTGREY);
                screenBackground();
                upgradeScreen();
                state = IDLE;
                delay(500);
            }

            if (M5.BtnB.wasClicked()) {
                Serial.println("BtnB Clicked");
                timerScreenSleep = millis();
                channel_number++;
                if (channel_number > 8) {
                    channel_number = 1;
                }
                upgradeScreenChannel();
            }

            if (M5.BtnA.wasClicked()) {
                Serial.println("BtnA was Clicked");
                preferences.putInt("channel", channel_number);
                M5.Display.drawString("Saved", 135 / 2, 100);
                ESP.restart();
            }
            if (millis() - timerScreenSleep > intervalScreenSleep && sleep_state == false) {
                state = SLEEP;
            }
            break;
        case SLEEP:
            Serial.println("sleep");
            sleep_state = true;
            M5.Display.sleep();
            state = IDLE;
            break;

        default:
            break;
    }

    if (M5.BtnPWR.isHolding()) {
        timerScreenSleep = millis();
        if (start_holding == false) {
            M5.Display.fillRect(0, 60, 135, 60, RED); // Efface la zone d'affichage
            M5.Display.setTextColor(BLACK);
            M5.Display.setTextDatum(CC_DATUM);
            M5.Display.setFont(&Orbitron_Light_24);
            //M5.Display.drawString(String(time_screen), 135 / 2, 90);
            M5.Display.drawString("Power Off :", 135 / 2, 70);
            M5.Display.drawString(String(time_screen)+"s", 135 / 2, 90);
            start_holding = true;
            timer_holding = millis();
        }
        if (millis() - timer_holding > 1000) {
            time_screen--;
            timer_holding = millis();
            M5.Display.fillRect(0, 60, 135, 60, RED); // Efface la zone d'affichage
            M5.Display.drawString("Power Off :", 135 / 2, 70);
            M5.Display.drawString(String(time_screen)+"s", 135 / 2, 90);
            //M5.Display.drawString(String(time_screen), 135 / 2, 90);

        }
    }
    if (M5.BtnPWR.wasReleasedAfterHold()) {
        timer_holding = 0;
        time_screen = 2;
        M5.Display.clear(LIGHTGREY);
        screenBackground();
        upgradeScreen();
    }

    delay(10);

}