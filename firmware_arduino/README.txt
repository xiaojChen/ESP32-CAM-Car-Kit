========================================
 ESP32-CAM 智能小车 - Arduino IDE 使用指南
========================================

【一、安装 Arduino IDE】
1. 下载 https://www.arduino.cc/en/software
2. 安装后打开 Arduino IDE

【二、安装 ESP32 开发板支持】
1. 文件 → 首选项 → "附加开发板管理器网址" 填入：
   https://espressif.github.io/arduino-esp32/package_esp32_index.json
2. 工具 → 开发板 → 开发板管理器
3. 搜索 "esp32"，安装 "esp32 by Espressif Systems"
4. 等待安装完成（可能需要 5-10 分钟，需联网）

【三、安装摄像头库】
1. 工具 → 管理库
2. 搜索 "ESP32 camera"
3. 安装 "ESP32 camera" 或 "esp32-camera"

【四、打开项目】
1. 文件 → 打开
2. 选择本文件夹（esp32_cam_car/）
3. 会自动加载 esp32_cam_car.ino

【五、选择开发板】
1. 工具 → 开发板 → ESP32 Arduino → "AI Thinker ESP32-CAM"
   (如果找不到，选 "ESP32 Wrover Module")
2. 工具 → Partition Scheme → "Huge APP (3MB No OTA/1MB SPIFFS)"
3. 工具 → Port → 选择 ESP32-CAM 对应的串口

【六、上传程序】
1. USB 连接 ESP32-CAM 到电脑
2. 按住 BOOT 按钮不放
3. 按一下 RESET 按钮（不要松开 BOOT）
4. 松开 RESET 按钮
5. 松开 BOOT 按钮
6. 点击 Arduino IDE 的上传按钮（-> 箭头）

【七、使用】
1. 上传成功后，打开串口监视器（工具 → 串口监视器）
2. 波特率选 115200
3. 等待 ESP32 连接 WiFi，查看分配的 IP 地址
4. 手机连上家里 WiFi，浏览器输入 IP 即可控制
5. 如果连不上家里 WiFi，ESP32 会自动开启热点 "ESP32-Car"
   手机连接此热点后访问 192.168.4.1

========================================
如需帮助，请查看 docs/ 目录下的产品手册
