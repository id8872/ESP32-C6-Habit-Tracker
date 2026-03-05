# 📱 ESP32-C6 Touchscreen Habit Tracker

An open-source, physical IoT habit tracker that logs your daily goals directly to Google Sheets. Built using an ESP32-C6, a 1.47" ST7789 IPS LCD, and an AXS5106L capacitive touch controller.

## ✨ Features
* **Swipeable UI:** Uses LVGL (v8.4.0) Tileview for smooth, smartphone-like page swiping.
* **Direct-to-Cloud:** Logs data directly to a Google Sheet using Google Apps Script.
* **Async Visual Feedback:** Buttons turn Yellow while processing, Green on successful cloud sync, and Red if the connection fails.
* **Easy Configuration:** Add or remove habits by editing a single array at the top of the code.

## 🛠 Hardware Used
* **Microcontroller:** ESP32-C6 (8MB Flash)
* **Display:** 1.47" ST7789 IPS LCD (172x320 resolution)
* **Touch Controller:** AXS5106L (I2C)

## 💻 Software Dependencies
* [Arduino_GFX](https://github.com/moononournation/Arduino_GFX) (v1.6.5+)
* [LVGL](https://github.com/lvgl/lvgl) (**Must use v8.4.0** - *v9 is currently unsupported by this layout*)

## ⚠️ Important Hardware Quirks Solved in this Repo
If you are using this specific screen hardware, this code solves several known issues:
1. **BGR vs RGB Color Inversion:** The display hardware expects Big-Endian BGR, but the ESP32 sends Little-Endian RGB. This code implements a fast byte-swap in the `my_disp_flush` function so standard HTML hex codes render correctly.
2. **Dead Touch / Swipe Fails:** The `TP_INT` hardware interrupt pin on this board is flaky during LVGL swipes. This code bypasses the interrupt pin entirely and uses direct high-speed I2C polling (`Wire.setClock(400000)`) for perfectly smooth swiping.

## 🚀 Setup Instructions

### 1. Google Sheets Setup
1. Create a new Google Sheet.
2. Go to **Extensions > Apps Script**.
3. Paste the provided `Code.gs` script (see repository files).
4. Click **Deploy > New Deployment**, select **Web App**, set access to **"Anyone"**, and deploy.
5. Copy the generated Web App URL.

### 2. Arduino Setup
1. Clone this repository.
2. Rename `secrets_template.h` to `secrets.h`.
3. Add your WiFi credentials and your Google Apps Script Web App URL to `secrets.h`.
4. Compile and upload to your ESP32-C6!

## 📝 Customizing Habits
You can easily add your own habits by modifying the `habits[]` array at the top of the `.ino` file:
```cpp
Habit habits[] = {
    // Label, Sheet Action, Sheet Value, Tile Page, Alignment
    {"PUL", "Pull Ups",  "5", 0, LV_ALIGN_LEFT_MID},
    {"H2O", "Water",     "1", 1, LV_ALIGN_CENTER}
};
