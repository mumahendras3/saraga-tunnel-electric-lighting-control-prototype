// Constants and variables definitions
const serverAddress = 'localhost'; // MQTT server address
const serverPort = 3000; // MQTT server port (using websocket)
const xAxisNum = 11; // Maximum number of X axis categories for all charts
let areaSelector = document.querySelector('select');
let currentArea = areaSelector.value.replace(/ +/g, '-').toLowerCase();

// MQTT Setup
const client = mqtt.connect('ws://' + serverAddress + ':' + serverPort);

// Subscribe to some topics
client.subscribe('status/#');
client.subscribe('set/illuminance/+');

// Get elements
let uploadForm = document.querySelector('form');
let uploadInput = document.querySelector('[name=upload]');
let illuminances = {};
let lamps = {};
let chartsData = {};
document.querySelectorAll('.illuminance').forEach(function(element){
    illuminances[element.id] = element;
});
document.querySelectorAll('.lamp').forEach(function(element){
    lamps[element.id] = element;
});
document.querySelectorAll('.chart').forEach(function(element){
    chartsData[element.id] = [];
    new ApexCharts(element, genChartOpts(element.id)).render();
});

// Function definitions
function componentToHex(c) {
    var hex = c.toString(16);
    return hex.length == 1 ? '0' + hex : hex;
}  
function rgbToHex(r, g, b) {
    return '#' + componentToHex(r) + componentToHex(g) + componentToHex(b);
}
function hexToRgb(hex) {
    const result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
    return result ? {
        r: parseInt(result[1], 16),
        g: parseInt(result[2], 16),
        b: parseInt(result[3], 16)
    } : null;
}
function invertRgb(colorObj) {
    return Object.keys(colorObj).length != 0 && colorObj.constructor === Object ? {
        r: 255 - colorObj.r,
        g: 255 - colorObj.g,
        b: 255 - colorObj.b
    } : null;
}
// function colorPreview(event) {
//     lamps[event.target.id].textContent = '';
//     lamps[event.target.id].style.backgroundColor = event.target.value;
// }
function colorSet(event) {
    const color = hexToRgb(event.target.value);
    const r = String(color.r);
    const g = String(color.g);
    const b = String(color.b);
    client.publish('set/lamp/' + event.target.id + '/red' , r, {retain: true});
    client.publish('set/lamp/' + event.target.id + '/green' , g, {retain: true});
    client.publish('set/lamp/' + event.target.id + '/blue' , b, {retain: true});
    // lamps[event.target.id].textContent = '...';
    // lamps[event.target.id].style.backgroundColor = event.target.value;
    // // Invert the text color so it's readable
    // const inverted = invertRgb(color);
    // const invertedStr = 'rgb(' + inverted.r + ', ' + inverted.g + ', ' + inverted.b + ')';
    // lamps[event.target.id].style.color = invertedStr;
}
function updateLamp(a, r, g, b, br) {
    const brightness = parseFloat(br);
    lamps[a].textContent = Math.round(brightness).toString() + '%';
    lamps[a].style.backgroundColor = 'rgb(' + r + ', ' + g + ', ' + b + ')';
    // Invert the text color so it's readable
    const inverted = invertRgb({'r': r, 'g': g, 'b': b});
    const invertedStr = 'rgb(' + inverted.r + ', ' + inverted.g + ', ' + inverted.b + ')';
    lamps[a].style.color = invertedStr;
    // Also update the color selector value
    const colorPicker = document.querySelector('#' + a + '[type="color"]');
    colorPicker.value = rgbToHex(parseInt(r), parseInt(g), parseInt(b));
}
async function updateIlluminance(a, tm, lx, lxTrg) {
    const time = new Date(tm);
    const timeStr = ('0' + time.getHours()).slice(-2) + '.' + ('0' + time.getMinutes()).slice(-2);
    chartsData[a].push({x: timeStr, y: parseFloat(lx)});
    if (chartsData[a].length > xAxisNum) chartsData[a].shift();
    // Using 'await' to update the chart data first before updating annotation
    await ApexCharts.exec(a, 'updateSeries', [{data: chartsData[a]}]);
    // Update the illuminance target annotation
    ApexCharts.exec(a, 'addYaxisAnnotation', {
        y: parseFloat(lxTrg),
        strokeDashArray: 20,
        borderColor: '#e03c31',
        fillColor: '#e03c31',
        label: {
            text: 'Target: ' + lxTrg + ' lx',
            textAnchor: 'end',
            offsetY: 20,
            borderColor: '#e03c31',
            style: {
                background: '#e03c31',
                color: '#fff',
                fontSize: '1rem',
                fontWeight: 700,
                padding: {
                    left: 4,
                    right: 4
                }
            }
        }
    }, false);
    // Show the new iluminance value in the web page
    illuminances[a].textContent = lx + ' lx';
}
function updateAll(area, payload) {
    // payload format sent from microcontroller is 'time|red|green|blue|brightness|avg_lux|avg_lux_target' (string)
    const split = payload.split('|');
    const time = split[0];
    const red = split[1];
    const green = split[2];
    const blue = split[3];
    const brightness = split[4];
    const avg_lux = split[5];
    const avg_lux_target = split[6];
    updateIlluminance(area, time, avg_lux, avg_lux_target);
    updateLamp(area, red, green, blue, brightness);
}
function upload(form) {
    // Create a new XMLHttpRequest (XHR) DOM object instance
    const XHR = new XMLHttpRequest();
    // Bind the FormData object and the form element
    const FD = new FormData(form);
    // Define what happens on successful data submission
    XHR.addEventListener('load', event => {
        alert(event.target.responseText);
    });
    // Define what happens in case of error
    XHR.addEventListener('error', event => {
        alert('Oops! Something went wrong.');
    });
    // Set up our request
    XHR.open('POST', '/upload');
    // The data sent is what the user provided in the form
    XHR.send(FD);
}
function displayArea(element) {
    const nextArea = element.target.value;
    // Hide currentArea and show nextArea
    document.querySelectorAll('#' + currentArea + '.section-lamp,' + '#' + currentArea + '.section-sensor').forEach(elm => {
        elm.setAttribute('style', 'display: none');
    });
    document.querySelectorAll('#' + nextArea + '.section-lamp,' + '#' + nextArea + '.section-sensor').forEach(elm => {
        elm.setAttribute('style', 'display: flex');
    });
    currentArea = nextArea;
}

// Attaching functions to some events
areaSelector.addEventListener('change', displayArea);
document.querySelectorAll('[type=color]').forEach(function(element) {
    // element.addEventListener('input', colorPreview, false);
    element.addEventListener('change', colorSet, false);
});
uploadForm.addEventListener('submit', event => {
    event.preventDefault();
    upload(uploadForm);
    uploadInput.value = '';
});

// Display the currently selected area at first load
document.querySelectorAll('#' + currentArea + '.section-lamp,' + '#' + currentArea + '.section-sensor').forEach(elm => {
    elm.setAttribute('style', 'display: flex');
});

// MQTT upon receive
client.on('message', function(topic, payload){
    const split = topic.split('/');
    const payloadStr = payload.toString();
    if (split[0] == 'status' && split[1] != 'time') updateAll(split[1], payloadStr);
});