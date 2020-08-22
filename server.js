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
const illuminanceRegex = /Ē\n([0-9.]+)\slx/m; // The regex to parse the desired illuminance value from the given pdf files
const setpointsDir = 'setpoints'; // the directory that contains all setpoint json files
const imagesDir = 'public/images'; // Public images directory
const binDir = 'bin'; // external executable files directory
let setpoints = []; // An array to hold all the setpoints data at runtime
let sent = {}; // An object that contains the last time setpoints have been sent for each area/location
let data = []; // Temporary data holder
let db; // Variable to store the database object after successfull connection to MongoDB

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
            console.log('No files found in ' + setpointsDir + ', skipping read setpoints...');
            return;
        }
        // Reset the given setpointsArray first
        // Using splice() method in order to modify directly the given global setpoints array,
        // not a local copy of it (i.e. to do a pass-by-reference for the setpointsArray parameter).
        setpointsArray.splice(0, setpointsArray.length);
        files.forEach(file => {
            if (path.posix.extname(file) != '.pdf') return;
            let setpoint = {};
            fs.readFile(path.posix.join(setpointsDir, file), (err, dataBuffer) => {
                if (err) throw err;
                pdf(dataBuffer).then(data => {
                    // let match = regex.exec(data.text);
                    // console.log(match[1]);
                    setpoint['illuminance'] = regex.exec(data.text)[1];
                }, err => {
                    throw err;
                });
            });
            let split = file.replace('.pdf', '').toLowerCase().split('-');
            setpoint['time'] = split[0];
            setpoint['area'] = split[1].replace(/\s|_+/g, '-');
            setpointsArray.push(setpoint);
        });
    });
}
function sendSetpoints(setpointsArray, lastSendTime) {
    if (setpointsArray.length == 0) {
        console.log('The given setpoints array is empty, skipping send setpoints...');
        return;
    }
    let now = new Date();
    let timeStr = ('0' + now.getHours()).slice(-2) + '.' + ('0' + now.getMinutes()).slice(-2);
    setpointsArray.forEach(setpoint => {
        if (setpoint.time == timeStr && setpoint.time != lastSendTime[setpoint.area]) {
            aedes.publish({
                topic: 'set/setpoint/' + setpoint.area,
                payload: setpoint.illuminance,
                retain: true
            });
            lastSendTime[setpoint.area] = setpoint.time;
        }
    });
}
function extractImages(pdfDir, imgDir) {
    fs.readdir(pdfDir, (err, files) => {
        files.forEach(file => {
            let binPath = path.posix.join(binDir, 'pdfimages.exe');
            let pdfPath = path.posix.join(pdfDir, file);
            let split = file.replace('.pdf', '').split('-');
            let imgPath = path.posix.join(imgDir, split[1].replace(/_+/g, ' '));
            const child = execFile(binPath, ['-j', '-l', '1', pdfPath, imgPath], (error, stdout, stderr) => {
                if (error) throw error;
                // Rename the image files so the order is right
                // <file_name>-1.jpg for the illuminance distribution image
                // <file_name>-2.jpg for the illuminance distribution legend image
                fs.rename(imgPath + '-0004.jpg', imgPath + '-1.jpg', err => {
                    if (err) throw err;
                });
                fs.rename(imgPath + '-0002.jpg', imgPath + '-2.jpg', err => {
                    if (err) throw err;
                });
                // Clean leftover images
                fs.unlink(imgPath + '-0000.jpg', err => {
                    if (err) throw err;
                });
                fs.unlink(imgPath + '-0001.pgm', err => {
                    if (err) throw err;
                });
                fs.unlink(imgPath + '-0003.pgm', err => {
                    if (err) throw err;
                });
                fs.unlink(imgPath + '-0005.pgm', err => {
                    if (err) throw err;
                });
            });
        });
    });
}

// Initialize the setpoints array
const fs = require('fs');
const path = require('path');
const pdf = require('pdf-parse');
readSetpoints(setpointsDir, setpoints, illuminanceRegex);

// Extract -map and -legend jpg images from setpoint pdfs
extractImages(setpointsDir, imagesDir);

// Initialize an MQTT server
const aedes = require('aedes')({id: 'mqtt-server'});
const mqttServer = require('net').createServer(aedes.handle);
mqttServer.listen(mqttPort, function(){
    console.log('server started and listening on port', mqttPort);
});

// Initialize an HTTP server with express and websocket
const express = require('express');
const ws = require('websocket-stream');
const fileUpload = require('express-fileupload'); // A simple express middleware for uploading files
const { execFile } = require('child_process'); // Needed to execute pdfimages.exe (to extract images from pdf)
const app = express();
app.use(fileUpload({useTempFiles: true}));
app.use(express.static('public'));
// Add /upload POST handler (for uploading files)
app.post('/upload', async (req, res) => {
    // Check if there aren't any files selected
    if (!req.files || Object.keys(req.files).length === 0) {
        return res.status(400).send('No files selected!');
    }
    // For multiple files
    if (Array.isArray(req.files.upload)) {
        try {
            for (let i = 0; i < req.files.upload.length; i++) {
                await req.files.upload[i].mv(path.posix.join(setpointsDir, req.files.upload[i].name));
            }
            res.send('Files uploaded successfully!');
            // Reread all setpoint values
            readSetpoints(setpointsDir, setpoints, illuminanceRegex);
            // Extract all illuminance distribution images from the setpoint pdfs
            extractImages(setpointsDir, imagesDir);
        } catch (error) {
            res.status(500).send('Oops! Something went wrong.\n\n' + error);
        }
    }
    // For single file
    else {
        req.files.upload.mv(path.posix.join(setpointsDir, req.files.upload.name), error => {
            if (error) {
                res.status(500).send('Oops! Something went wrong.\n\n' + error);
            }    
            else {
                res.send('File uploaded successfully!');
                // Reread all setpoint values
                readSetpoints(setpointsDir, setpoints, illuminanceRegex);
                // Extract all illuminance distribution images from the setpoint pdfs
                extractImages(setpointsDir, imagesDir);
            }    
        });
    }
});
// Add /get-images GET handler (for getting the list of all simulation images)
app.get('/get-images', (req, res) => {
    fs.readdir(imagesDir, (err, files) => {
        if (err) return res.status(500).send('Oops! Something went wrong.\n\n' + err);
        let fileList = JSON.stringify(files);
        res.send(fileList);
    });
});
const httpServer = require('http').createServer(app);
ws.createServer({ server: httpServer }, aedes.handle);
httpServer.listen(httpPort, function(){
    console.log('websocket server listening on port', httpPort);
});

// // Initialize MongoDB client
// const mongoClient = new require('mongodb').MongoClient(dbURL, {
//     useUnifiedTopology: true,
//     poolSize: 20,
// });
// // Create a new connection to MongoDB server
// if (!mongoClient.isConnected()) {
//     mongoClient.connect((err, client) => {
//         if (err) throw err;
//         console.log('Connected correctly to', dbURL);
//         db = client.db(dbName);
//     });
// }

// Fired when a message is published
aedes.on('publish', (packet, client) => {
    // for debugging
    console.log(packet.topic, packet.payload.toString());
    let split = packet.topic.split('/');
    let payloadStr = packet.payload.toString();
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
const timeoutSetpoints = setInterval(sendSetpoints, 1000, setpoints, sent);