// ESP32-CAM Video Car - Arduino IDE Compatible Sketch
// See README.txt for setup instructions
/* ESP32-CAM Car v4 - Landscape + PWM
 * Board: ESP32-CAM (AI-Thinker) + OV3660
 * L298N: IN1=12 IN2=13 IN3=15 IN4=14
 * PWM:  ENA=2 ENB=33  |  FLASH=4
 * WiFi AP: ESP32-Car */

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "esp_camera.h"
#include "test_jpeg.h"

// Camera pins (AI-Thinker)
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM     0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM       5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// Motor pins
#define MOTOR_IN1 12
#define MOTOR_IN2 13
#define MOTOR_IN3 15
#define MOTOR_IN4 14
#define MOTOR_ENA 2
#define MOTOR_ENB 33

// Flash LED
#define FLASH_LED 4

// PWM
#define PWM_FREQ 5000
#define PWM_RES 8
int motor_speed = 178;  // 70% (255=100%)

WebServer server(80);
bool camera_ok = false;
bool flash_on = false;

// Motor LEDC channels
// Ch0=IN1, Ch1=IN2, Ch2=IN3, Ch3=IN4
// Ch4=ENA, Ch5=ENB
#define CH_IN1 0
#define CH_IN2 1
#define CH_IN3 2
#define CH_IN4 3
#define CH_ENA 4
#define CH_ENB 5

void motor_stop() {
  ledcWrite(CH_IN1,0); ledcWrite(CH_IN2,0);
  ledcWrite(CH_IN3,0); ledcWrite(CH_IN4,0);
}

void motor_forward() {
  ledcWrite(CH_IN1,motor_speed); ledcWrite(CH_IN2,0);
  ledcWrite(CH_IN3,motor_speed); ledcWrite(CH_IN4,0);
}
void motor_backward() {
  ledcWrite(CH_IN1,0); ledcWrite(CH_IN2,motor_speed);
  ledcWrite(CH_IN3,0); ledcWrite(CH_IN4,motor_speed);
}
void motor_left() {
  ledcWrite(CH_IN1,motor_speed); ledcWrite(CH_IN2,0);
  ledcWrite(CH_IN3,0); ledcWrite(CH_IN4,motor_speed);
}
void motor_right() {
  ledcWrite(CH_IN1,0); ledcWrite(CH_IN2,motor_speed);
  ledcWrite(CH_IN3,motor_speed); ledcWrite(CH_IN4,0);
}

bool init_camera() {
  camera_config_t config={};
  config.ledc_channel=LEDC_CHANNEL_0;
  config.ledc_timer=LEDC_TIMER_0;
  config.pin_d0=Y2_GPIO_NUM; config.pin_d1=Y3_GPIO_NUM;
  config.pin_d2=Y4_GPIO_NUM; config.pin_d3=Y5_GPIO_NUM;
  config.pin_d4=Y6_GPIO_NUM; config.pin_d5=Y7_GPIO_NUM;
  config.pin_d6=Y8_GPIO_NUM; config.pin_d7=Y9_GPIO_NUM;
  config.pin_xclk=XCLK_GPIO_NUM; config.pin_pclk=PCLK_GPIO_NUM;
  config.pin_vsync=VSYNC_GPIO_NUM; config.pin_href=HREF_GPIO_NUM;
  config.pin_sccb_sda=SIOD_GPIO_NUM; config.pin_sccb_scl=SIOC_GPIO_NUM;
  config.pin_pwdn=PWDN_GPIO_NUM; config.pin_reset=RESET_GPIO_NUM;
  config.xclk_freq_hz=20000000; config.pixel_format=PIXFORMAT_JPEG;
  config.frame_size=FRAMESIZE_QVGA; config.jpeg_quality=10;
  config.fb_count=1;
  esp_err_t err=esp_camera_init(&config);
  if(err!=ESP_OK){Serial.printf("Cam FAIL:0x%x\n",err);return false;}
  sensor_t*s=esp_camera_sensor_get();
  if(s){Serial.printf("Sensor PID=0x%x\n",s->id.PID);}
  Serial.println("Camera OK"); return true;
}

void handle_cap() {
  if(camera_ok){
    camera_fb_t*fb=esp_camera_fb_get();
    if(fb){String jpg=String((const char*)fb->buf,fb->len);server.send(200,"image/jpeg",jpg);esp_camera_fb_return(fb);return;}
  }
  server.send_P(200,"image/jpeg",(const char*)test_jpeg,test_jpeg_len);
}

void handle_cmd() {
  if(!server.hasArg("dir")){server.send(400,"text/plain","?");return;}
  char d=server.arg("dir")[0];
  if(d=='f')motor_forward();else if(d=='b')motor_backward();
  else if(d=='l')motor_left();else if(d=='r')motor_right();
  else motor_stop();
  server.send(200,"text/plain",String(d));
}

void handle_boost() {
  if(server.hasArg("on")) motor_speed = server.arg("on").toInt() ? 255 : 204;
  server.send(200,"text/plain",String(motor_speed));
}

void handle_flash(){
  flash_on=!flash_on;
  digitalWrite(FLASH_LED,flash_on?HIGH:LOW);
  server.send(200,"text/plain",flash_on?"1":"0");
}

const char PAGE[] PROGMEM=R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset=UTF-8>
<meta name=viewport content="width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no">
<title>Car</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{width:100%;height:100%;overflow:hidden;background:#1a1a2e;color:#eee;font-family:sans-serif}
.row{display:flex;height:100%;width:100%;align-items:center;padding:8px;gap:8px}
.col{flex:1;display:flex;flex-direction:column;align-items:center;justify-content:center;gap:12px;height:100%}
.col-l{flex:0 0 28%;height:100%;justify-content:center;gap:20px}
.col-r{flex:0 0 28%;height:100%;justify-content:center;gap:16px}
.vid{flex:0 0 44%;display:flex;align-items:center;justify-content:center;height:100%;overflow:hidden}
.vid img{width:100%;height:100%;object-fit:contain;border-radius:8px;background:#000}
button{width:76px;height:76px;border:none;border-radius:50%;font-size:26px;color:#fff;cursor:pointer;-webkit-tap-highlight-color:transparent;touch-action:manipulation;user-select:none;display:flex;align-items:center;justify-content:center}
.f{background:#e94560}
.b{background:#0f3460}
.l{background:#16213e}
.r{background:#16213e}
.fl{background:#533483;font-size:20px;margin-top:12px}
.boost{background:#f09;font-size:18px;margin-bottom:4px}
.st{position:fixed;bottom:4px;right:8px;font-size:10px;color:#555;z-index:9}
</style>
</head>
<body>
<div class=row>
<div class="col col-l">
<button class=f ontouchstart=fetch("/cmd?dir=f") ontouchend=fetch("/cmd?dir=s")>▲</button>
<button class=b ontouchstart=fetch("/cmd?dir=b") ontouchend=fetch("/cmd?dir=s")>▼</button>
<button class=fl onclick=fetch("/flash")>💡</button>
</div>
<div class=vid><img id=st src=/cap></div>
<div class="col col-r">
<button class=l ontouchstart=fetch("/cmd?dir=l") ontouchend=fetch("/cmd?dir=s")>◀</button>
<button class=r ontouchstart=fetch("/cmd?dir=r") ontouchend=fetch("/cmd?dir=s")>▶</button>
<button class=boost ontouchstart=fetch("/boost?on=1") ontouchend=fetch("/boost?on=0")>⚡</button>
</div>
</div>
<div class=st id=info></div>
<script>
var st=document.getElementById("st");
function rf(){st.onload=function(){setTimeout(rf,50)};st.onerror=function(){setTimeout(rf,500)};st.src="/cap?t="+Date.now()}
rf();
document.getElementById("info").textContent="IP: "+location.hostname;
</script>
</body></html>
)rawliteral";

void handle_root(){server.send_P(200,"text/html",PAGE);}

void setup(){
  Serial.begin(115200);delay(500);
  Serial.println("\n=== ESP32-CAM Car v4 ===");
  // PWM setup
  ledcSetup(CH_IN1,PWM_FREQ,PWM_RES);ledcAttachPin(MOTOR_IN1,CH_IN1);
  ledcSetup(CH_IN2,PWM_FREQ,PWM_RES);ledcAttachPin(MOTOR_IN2,CH_IN2);
  ledcSetup(CH_IN3,PWM_FREQ,PWM_RES);ledcAttachPin(MOTOR_IN3,CH_IN3);
  ledcSetup(CH_IN4,PWM_FREQ,PWM_RES);ledcAttachPin(MOTOR_IN4,CH_IN4);
  ledcSetup(CH_ENA,PWM_FREQ,PWM_RES);ledcAttachPin(MOTOR_ENA,CH_ENA);
  ledcSetup(CH_ENB,PWM_FREQ,PWM_RES);ledcAttachPin(MOTOR_ENB,CH_ENB);
  ledcWrite(CH_ENA,255);ledcWrite(CH_ENB,255);
  motor_stop();Serial.println("Motor OK");
  // Flash LED
  pinMode(FLASH_LED,OUTPUT);digitalWrite(FLASH_LED,LOW);
  // Camera
  pinMode(PWDN_GPIO_NUM,OUTPUT);digitalWrite(PWDN_GPIO_NUM,LOW);delay(100);
  camera_ok=init_camera();
  // WiFi
  WiFi.mode(WIFI_STA);WiFi.begin("君君的wifi","asdf1234");
  Serial.print("Connecting WiFi");int wt=0;
  while(WiFi.status()!=WL_CONNECTED&&wt<200){delay(100);Serial.print(".");wt++;}
  IPAddress ip;
  if(WiFi.status()==WL_CONNECTED){
    ip=WiFi.localIP();Serial.print("OK IP: ");Serial.println(ip);
  }else{
    Serial.println("FAIL, start AP");
    WiFi.mode(WIFI_AP);WiFi.softAP("ESP32-Car");
    ip=WiFi.softAPIP();Serial.print("AP IP: ");Serial.println(ip);
  }
  // Routes
  server.on("/",handle_root);
  server.on("/cap",handle_cap);
  server.on("/cmd",handle_cmd);
  server.on("/flash",handle_flash);
  server.on("/boost",handle_boost);
  server.onNotFound([](){server.send(404,"text/plain","404");});
  server.begin();
  Serial.print("Camera: ");Serial.println(camera_ok?"ON":"FAIL (test JPEG)");
  Serial.print("Open http://");Serial.print(ip);Serial.println("/");
}

void loop(){server.handleClient();}

