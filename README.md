# Saraga ITB Tunnel's Electric Lighting Control System Prototype
All the source codes that are used for Saraga ITB tunnel's electric lighting control system prototype as my undergraduate final assignment

# DEPENDENCIES
1. Node.js with npm
2. ImageMagick
3. xpdf-tools
4. PlatformIO

# SETUP
1. Clone this repo (`git clone https://github.com/mumahendras3/saraga-tunnel-electric-lighting-control-prototype.git`)
2. Install these libraries in PlatformIO: Adafruit NeoPixel, PubSubClient, RTClib, and hp_BH1750
3. Compile and upload the microcontroller program
4. Run `npm install` on the root of this repo

# RUNNING
1. Run `node server.js` on the root of this repo
2. Access the web-based HMI on port 3000
3. Upload DIALux simulation result PDFs as desired
4. Connect the PCB to a 5V DC power source
5. Make sure the microcontroller is connected to the same network as the server computer

Done!