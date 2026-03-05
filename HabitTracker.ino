#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Arduino_GFX_Library.h>
#include <lvgl.h>
#include "secrets.h"

// =========================================================
// 1. HABIT CONFIGURATION TABLE
// Add, remove, or modify your habits right here!
// =========================================================
struct Habit {
    const char* shortLabel;   
    const char* sheetAction;  
    const char* sheetValue;   
    uint8_t tileIndex;        
    lv_align_t alignment;     
};

Habit habits[] = {
    // Tile 0 (Page 1)
    {"PUL", "Pull Ups",  "10",    0, LV_ALIGN_LEFT_MID},  // Will show PUL \n x5
    {"PSH", "Push Ups",  "10",   0, LV_ALIGN_RIGHT_MID}, // Will show PSH \n x10
    
    // Tile 1 (Page 2)
    {"H2O", "Water",     "40",    1, LV_ALIGN_LEFT_MID},  // Will show H2O \n x1
    {"MED", "Meditation","12m",  1, LV_ALIGN_RIGHT_MID}, // Will show MED \n x10m
    
    // Tile 2 (Page 3)
    {"SWM", "Swim",      "250m", 2, LV_ALIGN_CENTER}     // Will show SWM \n x425m
};
const int NUM_HABITS = sizeof(habits) / sizeof(habits[0]);

// =========================================================
// 2. SYSTEM CONFIGURATION & COLORS
// =========================================================
#define TP_SDA 18
#define TP_SCL 19
#define TP_RST 20
#define TP_INT 21
#define AXS_ADDR 0x63               
#define AXS_TOUCH_DATA_REG 0x01
#define GFX_BL 23

// Standard HTML Hex Colors 
#define COLOR_BLUE   0x007BFF 
#define COLOR_YELLOW 0xFFC107 
#define COLOR_GREEN  0x28A745 
#define COLOR_RED    0xDC3545 

// =========================================================
// 3. HARDWARE DRIVERS
// =========================================================
Arduino_DataBus *bus = new Arduino_HWSPI(15 /* DC */, 14 /* CS */, 1 /* SCK */, 2 /* MOSI */);

Arduino_GFX *gfx = new Arduino_ST7789(
  bus, 22 /* RST */, 1 /* rotation - Landscape */, false /* IPS */,
  172 /* width */, 320 /* height */,
  34 /*col_offset1*/, 0 /*row_offset1*/,
  34 /*col_offset2*/, 0 /*row_offset2*/);

void lcd_reg_init(void) {
  static const uint8_t init_operations[] = {
    BEGIN_WRITE, WRITE_COMMAND_8, 0x11, END_WRITE, DELAY, 120, BEGIN_WRITE,
    WRITE_C8_D16, 0xDF, 0x98, 0x53, WRITE_C8_D8, 0xB2, 0x23, WRITE_COMMAND_8, 0xB7,
    WRITE_BYTES, 4, 0x00, 0x47, 0x00, 0x6F, WRITE_COMMAND_8, 0xBB,
    WRITE_BYTES, 6, 0x1C, 0x1A, 0x55, 0x73, 0x63, 0xF0, WRITE_C8_D16, 0xC0, 0x44, 0xA4,
    WRITE_C8_D8, 0xC1, 0x16, WRITE_COMMAND_8, 0xC3,
    WRITE_BYTES, 8, 0x7D, 0x07, 0x14, 0x06, 0xCF, 0x71, 0x72, 0x77, WRITE_COMMAND_8, 0xC4,
    WRITE_BYTES, 12, 0x00, 0x00, 0xA0, 0x79, 0x0B, 0x0A, 0x16, 0x79, 0x0B, 0x0A, 0x16, 0x82,
    WRITE_COMMAND_8, 0xC8,
    WRITE_BYTES, 32, 0x3F, 0x32, 0x29, 0x29, 0x27, 0x2B, 0x27, 0x28, 0x28, 0x26, 0x25, 0x17, 0x12, 0x0D, 0x04, 0x00, 0x3F, 0x32, 0x29, 0x29, 0x27, 0x2B, 0x27, 0x28, 0x28, 0x26, 0x25, 0x17, 0x12, 0x0D, 0x04, 0x00,
    WRITE_COMMAND_8, 0xD0, WRITE_BYTES, 5, 0x04, 0x06, 0x6B, 0x0F, 0x00,
    WRITE_C8_D16, 0xD7, 0x00, 0x30, WRITE_C8_D8, 0xE6, 0x14, WRITE_C8_D8, 0xDE, 0x01, 
    WRITE_COMMAND_8, 0xB7, WRITE_BYTES, 5, 0x03, 0x13, 0xEF, 0x35, 0x35,
    WRITE_COMMAND_8, 0xC1, WRITE_BYTES, 3, 0x14, 0x15, 0xC0,
    WRITE_C8_D16, 0xC2, 0x06, 0x3A, WRITE_C8_D16, 0xC4, 0x72, 0x12,
    WRITE_C8_D8, 0xBE, 0x00, WRITE_C8_D8, 0xDE, 0x02, 
    WRITE_COMMAND_8, 0xE5, WRITE_BYTES, 3, 0x00, 0x02, 0x00,
    WRITE_COMMAND_8, 0xE5, WRITE_BYTES, 3, 0x01, 0x02, 0x00,
    WRITE_C8_D8, 0xDE, 0x00, WRITE_C8_D8, 0x35, 0x00, WRITE_C8_D8, 0x3A, 0x05, 
    WRITE_COMMAND_8, 0x2A, WRITE_BYTES, 4, 0x00, 0x22, 0x00, 0xCD,
    WRITE_COMMAND_8, 0x2B, WRITE_BYTES, 4, 0x00, 0x00, 0x01, 0x3F,
    WRITE_C8_D8, 0xDE, 0x02, WRITE_COMMAND_8, 0xE5, WRITE_BYTES, 3, 0x00, 0x02, 0x00,
    WRITE_C8_D8, 0xDE, 0x00, WRITE_C8_D8, 0x36, 0x00,
    WRITE_COMMAND_8, 0x21, END_WRITE, DELAY, 10, BEGIN_WRITE, WRITE_COMMAND_8, 0x29, END_WRITE
  };
  bus->batchOperation(init_operations, sizeof(init_operations));
}

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[320 * 20]; 

String pending_action = "";
String pending_value = "";
lv_obj_t * pending_btn = NULL;

// =========================================================
// 4. NETWORKING & GOOGLE SHEETS
// =========================================================
void setup_wifi() {
    Serial.print("Connecting to WiFi");
    WiFi.begin(ssid, password);
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 10) { 
        delay(500); 
        Serial.print(".");
        attempts++;
    }
    if(WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi Connected!");
    } else {
        Serial.println("\nWiFi Failed! Offline mode.");
    }
}

bool send_to_google_sheets(String action, String value) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        
        // Google requires strict redirects for Apps Script
        http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
        http.begin(GOOGLE_SCRIPT_URL);
        http.addHeader("Content-Type", "application/json");
        String jsonPayload = "{\"Action\":\"" + action + "\",\"Value\":\"" + value + "\"}";
        
        int httpCode = http.POST(jsonPayload);
        http.end();
        
        Serial.printf("Google Sheets HTTP Code: %d\n", httpCode);
        
        // Anything above 0 means the server accepted it
        if(httpCode > 0) {
            return true; 
        }
    }
    return false;
}

// =========================================================
// 5. UI BRIDGES (Graphics & Touch)
// =========================================================
void my_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    // FIX FOR ESP32 ENDIANNESS (This fixes the Yellow/Green color bug!)
    // Swaps the high and low bytes of the 16-bit color array so the SPI sends it correctly.
    for(uint32_t i = 0; i < w * h; i++) {
        uint16_t c = color_p[i].full;
        color_p[i].full = (c >> 8) | (c << 8); 
    }

    gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
    lv_disp_flush_ready(disp_drv);
}

void my_touchpad_read(lv_indev_drv_t * indev_driver, lv_indev_data_t * data) {
    static int16_t last_x = 0;
    static int16_t last_y = 0;

    // Fast I2C Polling to ensure Swipes register smoothly
    uint8_t buffer[14];
    Wire.beginTransmission(AXS_ADDR);
    Wire.write(AXS_TOUCH_DATA_REG);
    if (Wire.endTransmission() == 0) {
        Wire.requestFrom((uint16_t)AXS_ADDR, (uint8_t)14, (uint8_t)true);
        if (Wire.available() >= 14) {
            Wire.readBytes(buffer, 14);
            
            uint8_t touch_num = buffer[1];
            if (touch_num > 0) {
                data->state = LV_INDEV_STATE_PR; 
                
                uint16_t raw_x = ((uint16_t)(buffer[2] & 0x0F) << 8) | buffer[3];
                uint16_t raw_y = ((uint16_t)(buffer[4] & 0x0F) << 8) | buffer[5];
                
                // Landscape Mapping
                last_x = raw_y;
                last_y = 172 - 1 - raw_x; 
                
                data->point.x = last_x;
                data->point.y = last_y;
                return; 
            }
        }
    }

    data->state = LV_INDEV_STATE_REL;
    data->point.x = last_x;
    data->point.y = last_y;
}

// =========================================================
// 6. USER INTERFACE
// =========================================================
static void btn_event_cb(lv_event_t * e) {
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        lv_obj_t * btn = lv_event_get_target(e);
        
        Habit *h = (Habit *)lv_event_get_user_data(e);
        
        pending_action = h->sheetAction;
        pending_value = h->sheetValue;
        pending_btn = btn;
        
        // Turn button YELLOW immediately
        lv_obj_set_style_bg_color(btn, lv_color_hex(COLOR_YELLOW), LV_PART_MAIN);
    }
}

void create_counter_button(lv_obj_t * parent, Habit *habit) {
    lv_obj_t * btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 150, 150); 
    lv_obj_align(btn, habit->alignment, 0, 0); 
    
    // Style the button
    lv_obj_set_style_bg_color(btn, lv_color_hex(COLOR_BLUE), LV_PART_MAIN);
    lv_obj_set_style_radius(btn, 20, LV_PART_MAIN); 

    // Attach the habit struct to the button click event!
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, habit);

    // Style the text 
    lv_obj_t * label = lv_label_create(btn);
    
    // --> NEW: Format the text to pull the label, add a newline (\n), and add the value!
    lv_label_set_text_fmt(label, "%s\nx%s", habit->shortLabel, habit->sheetValue);
    
    // --> NEW: Tell LVGL to center the lines of text relative to each other
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

    lv_obj_set_style_text_font(label, &lv_font_montserrat_36, LV_PART_MAIN); 
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN); 
    lv_obj_center(label);
}

void build_counter_ui() {
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x000000), LV_PART_MAIN);

    lv_obj_t * tv = lv_tileview_create(lv_scr_act());
    lv_obj_set_style_bg_color(tv, lv_color_hex(0x000000), LV_PART_MAIN);
    
    // Remove scrollbars so it looks cleaner
    lv_obj_set_scrollbar_mode(tv, LV_SCROLLBAR_MODE_OFF);

    int max_tile = 0;
    for (int i = 0; i < NUM_HABITS; i++) {
        if (habits[i].tileIndex > max_tile) {
            max_tile = habits[i].tileIndex;
        }
    }

    lv_obj_t * tiles[10];
    for (int i = 0; i <= max_tile; i++) {
        tiles[i] = lv_tileview_add_tile(tv, i, 0, LV_DIR_LEFT | LV_DIR_RIGHT);
    }

    for (int i = 0; i < NUM_HABITS; i++) {
        create_counter_button(tiles[habits[i].tileIndex], &habits[i]);
    }
}

// =========================================================
// 7. MAIN SETUP & LOOP
// =========================================================
void setup() {
    Serial.begin(115200);
    
    if (!gfx->begin()) Serial.println("gfx->begin() failed!");
    lcd_reg_init();
    gfx->setRotation(1); 
    gfx->fillScreen(RGB565_BLACK);

    pinMode(GFX_BL, OUTPUT);
    digitalWrite(GFX_BL, HIGH);

    pinMode(TP_RST, OUTPUT);
    pinMode(TP_INT, INPUT_PULLUP);

    digitalWrite(TP_RST, LOW);
    delay(200);
    digitalWrite(TP_RST, HIGH);
    delay(300);

    Wire.begin(TP_SDA, TP_SCL);
    Wire.setClock(400000); 

    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, 320 * 20);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = 320; 
    disp_drv.ver_res = 172; 
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);

    build_counter_ui();
    setup_wifi(); 
}

void loop() {
    lv_timer_handler(); 
    delay(5); 

    if (pending_action != "") {
        bool success = send_to_google_sheets(pending_action, pending_value);
        
        if (success) {
            lv_obj_set_style_bg_color(pending_btn, lv_color_hex(COLOR_GREEN), LV_PART_MAIN);
        } else {
            lv_obj_set_style_bg_color(pending_btn, lv_color_hex(COLOR_RED), LV_PART_MAIN);
        }
        
        lv_refr_now(NULL); 
        delay(1000);       
        
        lv_obj_set_style_bg_color(pending_btn, lv_color_hex(COLOR_BLUE), LV_PART_MAIN);
        
        pending_action = "";
        pending_value = "";
        pending_btn = NULL;
    }
}
