# Saraga ITB Tunnel's Electric Lighting Control System Prototype
All the source codes that are used for Saraga ITB tunnel's electric lighting control system prototype as my undergraduate final assignment

## DEPENDENCIES
1. Node.js with npm
2. ImageMagick
3. xpdf-tools
4. PlatformIO

## SETUP
1. Clone this repo (`git clone https://github.com/mumahendras3/saraga-tunnel-electric-lighting-control-prototype.git`)
2. Install these libraries in PlatformIO: Adafruit NeoPixel, PubSubClient, RTClib, and hp_BH1750
3. Modify the `ssid`, `password`, and `mqtt_server` variable in `LoLin_NodeMcuV3/src/main.cpp` accordingly
4. Compile and upload the microcontroller program
5. Run `npm install` on the root of this repo
6. Modify the MongoDB database info to be used in `server.js` (at the top of that file) accordingly

## RUNNING
1. Make sure the MongoDB server that will be used is already running
2. Run `node server.js` on the root of this repo
3. Access the web-based HMI on port 3000
4. Upload DIALux simulation result PDFs as desired
5. Connect the PCB to a 5V DC power source
6. Make sure the microcontroller is connected to the same network as the server computer
7. Done!