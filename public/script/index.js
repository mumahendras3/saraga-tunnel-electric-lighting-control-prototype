// Constants and variables definitions
const serverAddress = 'localhost'; // MQTT server address
const serverPort = 3000; // MQTT server port (using websocket)
const totalData = 3; // the total number of sensor readings to collect per cycle
const waitTime = 500; // time (in ms) to wait before rechecking if all sensors have sent their readings
let dataCounter = 0; // counter to keep track of how many sensor readings have been collected per cycle

// MQTT Setup
const client = mqtt.connect('ws://' + serverAddress + ':' + serverPort);

// Subscribe to some topics
client.subscribe("status/#");

// Get elements
let uploadForm = document.querySelector('form.section');
let uploadInput = document.querySelector('[name=upload]');
let sensor = {};
let lampu = {};
let grafik = {};
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
    let time = new Date(waktu);
    let timeStr = ('0' + time.getHours()).slice(-2) + ':';
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
function upload(form) {
    // Create a new XMLHttpRequest (XHR) DOM object instance
    const XHR = new XMLHttpRequest();
    // Bind the FormData object and the form element
    const FD = new FormData(form);
    // Define what happens on successful data submission
    XHR.addEventListener("load", event => {
      alert(event.target.responseText);
    });
    // Define what happens in case of error
    XHR.addEventListener("error", event => {
      alert('Oops! Something went wrong.');
    });
    // Set up our request
    XHR.open("POST", "/upload");
    // The data sent is what the user provided in the form
    XHR.send(FD);
}
function addImgNode(id) {
    fetch('/get-images')
    .then(res => res.json())
    .then(res => {
        let parentNode = document.getElementById(id);
        let count = 0; // To prevent double titles
        res.forEach(file => {
            if (file == 'place_holder.png') return;
            // Save the area name first
            let areaName = file.slice(0, -6);
            // Create the image node
            let imgElement = document.createElement('img');
            // Set the img source
            imgElement.setAttribute('src', 'images/' + file);
            // Setting the appropriate alt text
            let altStr = 'Simulasi distribusi iluminansi di area ';
            altStr += areaName;
            imgElement.setAttribute('alt', altStr);
            // Set the class to 'image'
            imgElement.setAttribute('class', 'image');
            // Attach all new nodes to the parent node
            // Create only 1 title for each image pair (illuminance dist. and its legend)
            if (count < 1) {
                let title = document.createElement('h2');
                title.innerHTML = areaName;
                parentNode.appendChild(title)
                count++;
            }
            else {
                count--;
            }
            parentNode.appendChild(imgElement);
        });
    })
    .catch(err => console.error(err));
}

// Attaching functions to some events
document.querySelectorAll("form.batas").forEach(function(element){
    element.addEventListener("submit", setBatas);
});
uploadForm.addEventListener('submit', event => {
    event.preventDefault();
    upload(uploadForm);
    uploadInput.value = "";
});

// MQTT upon receive
client.on('message', function(topic, payload){
    let split = topic.split("/");
    let payloadStr = payload.toString();
    switch (split[0] + "/" + split[1]) {
        case "status/lampu": statusLampu(split[2], payloadStr); break;
        case "status/sensor": statusSensor(split[2], parseFloat(payloadStr)); break;
        case "status/waktu": statusWaktu(payloadStr); break;
    }
});

// Show simulation results of all available setpoint values (taken from the uploaded pdf files)
addImgNode('section-simulation')