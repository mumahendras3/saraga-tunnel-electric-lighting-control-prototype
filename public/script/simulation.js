// Some Global variables
let areaSelector = document.querySelector('select');
let currentArea = areaSelector.value.replace(/ +/g, '-').toLowerCase();
let placeholder = document.querySelector('.placeholder');

async function getSetpoints() {
    const res = await fetch('/get-setpoints');
    return res.json();
}
function addImgNode(id) {
    fetch('/get-images')
    .then(res => res.json())
    .then(async res => {
        if (res == []) return;
        // if (res == ['building_layout.jpeg']) return;
        // if (res == ['logo_cita.png']) return;
        // if (res == ['building_layout.jpeg', 'logo_cita.png']) return;
        let parentNode = document.getElementById(id);
        // Empty the parent node first
        // parentNode.textContent = '';
        const setpoints = await getSetpoints();
        if (setpoints == []) return;
        res.forEach((imgPath, index) => {
            // Skip unconverted image files (just in case)
            if (/-0[0-9]+\./.test(imgPath)) return;
            // Skip building_layout.jpeg and logo_cita.png
            if (/building_layout/.test(imgPath)) return;
            if (/logo_cita/.test(imgPath)) return;
            // Extract the time and area information first
            const baseNameNoExt = imgPath.slice(0, -6).split('/')[1];
            const split = baseNameNoExt.split('-');
            const time = split[0];
            const areaName = split[1].replace(/_+/g, ' '); // Just in case there are underscores
            const areaNameForId = areaName.replace(/ +/g, '-').toLowerCase(); // Will be used later
            // let title = areaName + ' (' + time + ', Ē = ';
            let title = 'Waktu: ' + time + ', Persentase Peredupan Lampu: ';
            if (index % 2 == 0) {
                title += setpoints[index/2].brightness + '%, Target Iluminansi: ';
                title += setpoints[index/2].illuminance.replace(/\./, ',') + ' lx';
            }
            else {
                title += setpoints[(index-1)/2].brightness + '%, Target Iluminansi: ';
                title += setpoints[(index-1)/2].illuminance.replace(/\./, ',') + ' lx)';
            }
            // Create the image node
            let imgElement = document.createElement('img');
            // Set to hidden first except if it's the currently selected area
            if (areaNameForId != currentArea) imgElement.setAttribute('style', 'display: none');
            // Set the img source
            imgElement.setAttribute('src', imgPath);
            // Setting the appropriate alt text
            let altStr = 'Simulasi distribusi iluminansi di area ' + areaName;
            imgElement.setAttribute('alt', altStr);
            // Set the class to 'image' and id to areaNameForClassName
            imgElement.setAttribute('class', 'image');
            imgElement.setAttribute('id', areaNameForId);
            // Attach all new nodes to the parent node
            // Create only 1 title for each image pair (illuminance dist. and its legend)
            if (index % 2 == 0) {
                let headerElement = document.createElement('div');
                // Set to hidden first
                if (areaNameForId != currentArea) headerElement.setAttribute('style', 'display: none');
                headerElement.setAttribute('class', 'area-header');
                headerElement.setAttribute('id', areaNameForId);
                headerElement.textContent = title;
                parentNode.appendChild(headerElement);
            }
            parentNode.appendChild(imgElement);
        });
    })
    .then(() => {
        const nodes = document.querySelectorAll('#' + currentArea);
        if (nodes.length == 0) placeholder.setAttribute('style', 'display: block');
    })
    .catch(err => console.error(err));
}
function displayArea(element) {
    const nextArea = element.target.value;
    // Hide the placeholder text first (if it's shown now)
    placeholder.setAttribute('style', 'display: none');
    // Hide currentArea and show nextArea
    let nodes = document.querySelectorAll('#' + currentArea);
    if (nodes.length > 0) nodes.forEach(elm => {
        elm.setAttribute('style', 'display: none');
    });
    nodes = document.querySelectorAll('#' + nextArea);
    if (nodes.length > 0) nodes.forEach(elm => {
        elm.setAttribute('style', 'display: block');
    });
    else document.querySelector('.placeholder').setAttribute('style', 'display: block');
    currentArea = nextArea;
}

// Attach event listeners
areaSelector.addEventListener('change', displayArea);

// Add simulation results of all available setpoint values (taken from the uploaded pdf files)
addImgNode('simulation-results');