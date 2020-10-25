// Maximum x axis categories

// Add locales
Apex.chart = {
  locales: [{
      "name": "id",
      "options": {
        "months": [
          "Januari",
          "Februari",
          "Maret",
          "April",
          "Mei",
          "Juni",
          "Juli",
          "Agustus",
          "September",
          "Oktober",
          "November",
          "Desember"
        ],
        "shortMonths": [
          "Jan",
          "Feb",
          "Mar",
          "Apr",
          "Mei",
          "Jun",
          "Jul",
          "Agu",
          "Sep",
          "Okt",
          "Nov",
          "Des"
        ],
        "days": ["Minggu", "Senin", "Selasa", "Rabu", "kamis", "Jumat", "Sabtu"],
        "shortDays": ["Min", "Sen", "Sel", "Rab", "Kam", "Jum", "Sab"],
        "toolbar": {
          "exportToSVG": "Unduh SVG",
          "exportToPNG": "Unduh PNG",
          "exportToCSV": "Unduh CSV",
          "menu": "Menu",
          "selection": "Pilihan",
          "selectionZoom": "Perbesar Pilihan",
          "zoomIn": "Perbesar",
          "zoomOut": "Perkecil",
          "pan": "Geser",
          "reset": "Atur Ulang Zoom"
        }
      }
  }],
  defaultLocale: "id"
};

// A function to generate per-chart options with specific chart id
function genChartOpts(ID) {
  return {
    series: [{
      name: "Iluminansi",
      data: []
    }],
    chart: {
      type: 'line',
      id: ID,
      fontFamily: 'Verdana',
      dropShadow: {
        enabled: true,
        color: '#000',
        top: 18,
        left: 7,
        blur: 10,
        opacity: 0.2
      },
      toolbar: {
        show: true
      },
      animations: {
        speed: 1000,
        easing: 'linear',
        dynamicAnimation: {
          speed: 1000,
        },
      },
    },
    colors: ['#77B6EA'],
    dataLabels: {
      enabled: true,
      style: {
        fontSize: '1rem'
      },
      background: {
        opacity: 1
      }
    },
    stroke: {
      curve: 'smooth'
    },
    grid: {
      borderColor: '#e7e7e7',
      row: {
        colors: ['#f3f3f3', 'transparent'], // takes an array which will be repeated on columns
        opacity: 0.5
      },
    },
    markers: {
        size: 1
    },
    xaxis: {
      type: 'category',
      title: {
        text: 'Waktu',
        offsetY: -20,
        style: {
          fontSize: '1.2rem'
        }
      },
      labels: {
        style: {
          fontSize: '1rem'
        }
      }
    },
    yaxis: {
      title: {
        text: 'Iluminansi (lx)',
        offsetX: -10,
        style: {
          fontSize: '1.2rem'
        }
      },
      labels: {
        style: {
          fontSize: '1rem'
        }
      },
      min: 0,
      forceNiceScale: true
    },
    noData: {
      text: 'Loading...'
    },
  };
}