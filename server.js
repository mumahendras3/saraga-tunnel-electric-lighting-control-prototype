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
const totalData = 3; // the total number of sensor readings to collect per cycle
const waitTime = 1000; // time (in ms) to wait before rechecking if all sensors have sent their readings
var data = []; // temporary data holder
var db; // variable to store the database object after successfull connection to MongoDB

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