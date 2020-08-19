// Database info to be used
const mongodbHost = 'localhost';
const mongodbPort = '27017';
const dbName = 'kontrol_pencahayaan_buatan';
const dbUser = 'rikhen';
const dbPass = 'sukses';
const dbURL = 'mongodb://' + dbUser + ':' + dbPass + '@' + mongodbHost + ':' + mongodbPort + '/' + dbName;

// Some more variable definitions
const mqttPort = 1883; // MQTT server port
const httpPort = 3000; // HTTP web server port
const totalData = 3; // The total number of sensor readings to collect per cycle
const waitTime = 1000; // Time (in ms) to wait before rechecking if all sensors have sent their readings
const illuminanceRegex = /Ē\n([0-9]+)\slx/m; // The regex to parse the desired illuminance value from the given pdf files
const setpointsDir = "setpoints" // the directory that contains all the pdf files that will be parsed
var setpoints = []; // An array to hold all the setpoints data at runtime
var data = []; // Temporary data holder
var db; // Variable to store the database object after successfull connection to MongoDB

// Function definitions
function simpanData(waktu) {
    // Loop until all sensors have sent their readings
    const loop = setInterval(() => {
        if (data.length >= totalData && mongoClient.isConnected()) {
            // add timestamp to each data
            data.forEach(datum => {
                datum['waktu'] = waktu;
            });
            // Send all data to MongoDB
            db.collection('data_sensor').insertMany(data, {forceServerObjectId: true}, (err, res) => {
                if (err) throw err;
                console.log('Successfully sent the following data to MongoDB:', data); // for debugging
                data = []; // reset the data holder
                clearInterval(loop);
            });
        }
    }, waitTime);
}
function readSetpoints(setpointsDir, setpointsArray, regex) {
    fs.readdir(setpointsDir, (err, files) => {
        if (err) throw err;
        if (files.length == 0) {
            console.log("No files found in " + setpointsDir + "/, skipping read setpoints...");
            return;
        }
        // Reset the given setpointsArray first
        // Using splice() method in order to modify directly the given global setpoints array,
        // not a local copy of it (i.e. to do a pass-by-reference for the setpointsArray parameter).
        setpointsArray.splice(0, setpointsArray.length);
        files.forEach(file => {
            if (path.posix.extname(file) != ".pdf") return;
            let setpoint = {};
            fs.readFile(setpointsDir + "/" + file, (err, dataBuffer) => {
                if (err) throw err;
                pdf(dataBuffer).then(data => {
                    // let match = regex.exec(data.text);
                    // console.log(match[1]);
                    setpoint['illuminance'] = regex.exec(data.text)[1];
                });
            });
            let split = file.replace(".pdf", "").toLowerCase().split("-");
            setpoint['time'] = split[0];
            setpoint['area'] = split[1].replace(/\s|_+/g, "-");
            setpointsArray.push(setpoint);
        });
    });
}
function sendSetpoints(setpointsArray) {
    if (setpointsArray.length == 0) {
        console.log("The given setpoints array is empty, skipping send setpoints...");
        return;
    }
    let now = new Date();
    let timeStr = ("0" + now.getHours()).slice(-2) + "." + ("0" + now.getMinutes()).slice(-2);
    setpointsArray.forEach(setpoint => {
        if (setpoint.time == timeStr) {
            aedes.publish({
                topic: "set/setpoint/" + setpoint.area,
                payload: setpoint.illuminance,
                retain: true
            });
        }
    });
}

// Initialize the setpoints array
const fs = require('fs');
const path = require('path');
const pdf = require('pdf-parse');
readSetpoints(setpointsDir, setpoints, illuminanceRegex);

// Initialize an MQTT server
const aedes = require('aedes')({id: 'mqtt-server'});
const mqttServer = require('net').createServer(aedes.handle);
mqttServer.listen(mqttPort, function(){
    console.log('server started and listening on port', mqttPort);
});

// Initialize an HTTP server with express and websocket
const express = require('express');
const ws = require('websocket-stream');
const app = express();
app.use(express.static('public'));
const httpServer = require('http').createServer(app);
ws.createServer({ server: httpServer }, aedes.handle);
httpServer.listen(httpPort, function(){
    console.log('websocket server listening on port', httpPort);
});

// Initialize MongoDB client
const mongoClient = new require('mongodb').MongoClient(dbURL, {
    useUnifiedTopology: true,
    poolSize: 20,
});
// Create a new connection to MongoDB server
if (!mongoClient.isConnected()) {
    mongoClient.connect((err, client) => {
        if (err) throw err;
        console.log("Connected correctly to", dbURL);
        db = client.db(dbName);
    });
}

// Fired when a message is published
aedes.on('publish', (packet, client) => {
    // for debugging
    console.log(packet.topic, packet.payload.toString());
    var split = packet.topic.split('/');
    var payloadStr = packet.payload.toString();
    switch (split[0] + '/' + split[1]) {
        case 'status/sensor':
            data.push({tempat: split[2], iluminansi: parseFloat(payloadStr)});
            break;
        case 'status/waktu':
            simpanData(payloadStr);
            break;
    }
});

// Run setInterval() on sendSetpoints() with 1 second delay
const timeoutSetpoints = setInterval(sendSetpoints, 1000, setpoints);