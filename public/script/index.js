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
let illuminances = {};
let lamps = {};
let charts = {};
document.querySelectorAll(".illuminance").forEach(function(element){
    illuminances[element.id] = element;
});
document.querySelectorAll(".lamp").forEach(function(element){
    lamps[element.id] = element;
});
document.querySelectorAll(".chart").forEach(function(element){
    charts[element.id] = [];
    new ApexCharts(element, genChartOpts(element.id)).render();
});

// Function definitions
function statusLampu(id, status) {
    lamps[id].innerHTML = status;
    if (status == "ON") {
        lamps[id].style.backgroundColor = "red";
    }
    else {
        lamps[id].style.backgroundColor = "grey";
    }
};
function statusSensor(id, value) {
    // Store the new data in temporary storage and increase dataCounter
    charts[id].push({y: value});
    dataCounter += 1;
    // Show the new iluminance value in the web page
    illuminances[id].innerHTML = value;
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
            // add timestamp to each new data and update the chart
            for (const key in charts) {
                if (charts.hasOwnProperty(key)) {
                    charts[key][charts[key].length - 1]['x'] = timeStr;
                    if (charts[key].length > 7) charts[key].shift();
                    ApexCharts.exec(key, 'updateSeries', [{data: charts[key]}]);
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
        // Give some time for the server to extract the simulation images from
        // the uploaded pdfs and then refresh the displayed simulation images
        setTimeout(addImgNode, 2000, 'section-simulation');
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
        if (res == []) return;
        let parentNode = document.getElementById(id);
        // Empty the parent node first
        parentNode.textContent = '';
        let count = 0; // To prevent double titles
        res.forEach(path => {
            // Extract the time and area information first
            const baseNameNoExt = path.slice(0, -6).split('/')[1];
            const split = baseNameNoExt.split('-');
            const time = split[0];
            const areaName = split[1].replace(/_+/g, ' '); // Just in case there are underscores
            const title = areaName + ' (' + time + ')';
            // Create the image node
            let imgElement = document.createElement('img');
            // Set the img source
            imgElement.setAttribute('src', path);
            // Setting the appropriate alt text
            let altStr = 'Simulasi distribusi iluminansi di area ';
            altStr += title;
            imgElement.setAttribute('alt', altStr);
            // Set the class to 'image'
            imgElement.setAttribute('class', 'image');
            // Attach all new nodes to the parent node
            // Create only 1 title for each image pair (illuminance dist. and its legend)
            if (count < 1) {
                let titleElement = document.createElement('h2');
                titleElement.innerHTML = title;
                parentNode.appendChild(titleElement);
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
        case "status/lamp": statusLampu(split[2], payloadStr); break;
        case "status/illuminance": statusSensor(split[2], parseFloat(payloadStr)); break;
        case "status/waktu": statusWaktu(payloadStr); break;
    }
});

// Show simulation results of all available setpoint values (taken from the uploaded pdf files)
addImgNode('section-simulation')