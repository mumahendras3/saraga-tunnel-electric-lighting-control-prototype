// Constants and variables definitions
const serverAddress = 'localhost'; // MQTT server address
const serverPort = 3000; // MQTT server port (using websocket)
const totalData = 3; // the total number of sensor readings to collect per cycle
const waitTime = 500; // time (in ms) to wait before rechecking if all sensors have sent their readings
var dataCounter = 0; // counter to keep track of how many sensor readings have been collected per cycle

// MQTT Setup
const client = mqtt.connect('ws://' + serverAddress + ':' + serverPort);

// Subscribe to some topics
client.subscribe("status/#");

// Get elements
var sensor = {};
var lampu = {};
var grafik = {};
document.querySelectorAll(".sensor").forEach(function(element){
    sensor[element.id] = element;
});
document.querySelectorAll(".lampu").forEach(function(element){
    lampu[element.id] = element;
});
document.querySelectorAll(".grafik").forEach(function(element){
    grafik[element.id] = [];
    new ApexCharts(element, genChartOpts(element.id)).render();
});

// Function definitions
function statusLampu(id, status) {
    lampu[id].innerHTML = status;
    if (status == "ON") {
        lampu[id].style.backgroundColor = "red";
    }
    else {
        lampu[id].style.backgroundColor = "grey";
    }
};
function statusSensor(id, value) {
    // Store the new data in temporary storage and increase dataCounter
    grafik[id].push({y: value});
    dataCounter += 1;
    // Show the new iluminance value in the web page
    sensor[id].innerHTML = value;
};
function statusWaktu(waktu) {
    // Convert unix timestamp to hour:minute:seconds
    var time = new Date(waktu);
    var timeStr = ('0' + time.getHours()).slice(-2) + ':';
    timeStr += ('0' + time.getMinutes()).slice(-2) + ':';
    timeStr += ('0' + time.getSeconds()).slice(-2);
    // Loop until all sensors have sent their readings
    const loop = setInterval(() => {
        if (dataCounter >= totalData) {
            // add timestamp to each new data and update the graph
            for (const key in grafik) {
                if (grafik.hasOwnProperty(key)) {
                    grafik[key][grafik[key].length - 1]['x'] = timeStr;
                    if (grafik[key].length > 7) grafik[key].shift();
                    ApexCharts.exec(key, 'updateSeries', [{data: grafik[key]}]);
                }
            }
            dataCounter = 0;
            clearInterval(loop);
        }
    }, waitTime);
};
function setBatas(event) {
    event.preventDefault();
    client.publish("set/" + this.className + "/" + this.id, this.children[1].value);
};

// Attaching functions to some events
document.querySelectorAll("form.batas").forEach(function(element){
    element.addEventListener("submit", setBatas);
});

// MQTT upon receive
client.on('message', function(topic, payload){
    var split = topic.split("/");
    var payloadStr = payload.toString();
    switch (split[0] + "/" + split[1]) {
        case "status/lampu": statusLampu(split[2], payloadStr); break;
        case "status/sensor": statusSensor(split[2], parseFloat(payloadStr)); break;
        case "status/waktu": statusWaktu(payloadStr); break;
    }
});