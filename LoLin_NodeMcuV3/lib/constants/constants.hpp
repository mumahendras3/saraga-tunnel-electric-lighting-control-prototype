#ifndef CONSTANTS_H
#define CONSTANTS_H

// I2C device address definitions
#define TCA_ADDR_DEF 0x70
// Total number of I2C mux used
#define NUM_TCA 2
// Total number of I2C channels per I2C mux
#define NUM_TCA_CH 8
// Maximum number of light sensors that can be used
#define MAX_NUM_BH1750 NUM_TCA*NUM_TCA_CH*2

// Total number of light sensors used at area 'Meja dan Kursi 1'
#define NUM_BH1750_MEJA_DAN_KURSI_1 1
// Total number of light sensors used at area 'Meja dan Kursi 2'
#define NUM_BH1750_MEJA_DAN_KURSI_2 1
// Total number of light sensors used at area 'Meja dan Kursi 3'
#define NUM_BH1750_MEJA_DAN_KURSI_3 1
// Total number of light sensors used at area 'Pameran Poster 1'
#define NUM_BH1750_PAMERAN_POSTER_1 1
// Total number of light sensors used at area 'Pameran Poster 2'
#define NUM_BH1750_PAMERAN_POSTER_2 1
// Total number of light sensors used at area 'Panggung Serbaguna'
#define NUM_BH1750_PANGGUNG_SERBAGUNA 2
// Total number of light sensors used at area 'Pejalan Kaki 1'
#define NUM_BH1750_PEJALAN_KAKI_1 2
// Total number of light sensors used at area 'Pejalan Kaki 2'
#define NUM_BH1750_PEJALAN_KAKI_2 6
// Total number of light sensors used at area 'Pejalan Kaki 3'
#define NUM_BH1750_PEJALAN_KAKI_3 2
// Total number of light sensors used at area 'Pejalan Kaki 4'
#define NUM_BH1750_PEJALAN_KAKI_4 2

// Luminous intensity of the red, green, and blue LED for each pixel in the LED strip
// in mcd (milicandela) (taken from datasheet)
#define LUM_RED 310
#define LUM_GREEN 780
#define LUM_BLUE 215

// Index of first LED in area 'Meja dan Kursi 1'
#define INDEX_LED_MEJA_DAN_KURSI_1 10
// Total number of LEDs used at area 'Meja dan Kursi 1'
#define NUM_LED_MEJA_DAN_KURSI_1 6
// Distance (in meter) from LEDs to measuring point in area 'Meja dan Kursi 1'
#define DIST_LED_MEJA_DAN_KURSI_1 0.16

// Index of first LED in area 'Meja dan Kursi 2'
#define INDEX_LED_MEJA_DAN_KURSI_2 16
// Total number of LEDs used at area 'Meja dan Kursi 2'
#define NUM_LED_MEJA_DAN_KURSI_2 5
// Distance (in meter) from LEDs to measuring point in area 'Meja dan Kursi 2'
#define DIST_LED_MEJA_DAN_KURSI_2 0.16

// Index of first LED in area 'Meja dan Kursi 3'
#define INDEX_LED_MEJA_DAN_KURSI_3 21
// Total number of LEDs used at area 'Meja dan Kursi 3'
#define NUM_LED_MEJA_DAN_KURSI_3 6
// Distance (in meter) from LEDs to measuring point in area 'Meja dan Kursi 3'
#define DIST_LED_MEJA_DAN_KURSI_3 0.16

// Index of first LED in area 'Pameran Poster 1'
#define INDEX_LED_PAMERAN_POSTER_1 34
// Total number of LEDs used at area 'Pameran Poster 1'
#define NUM_LED_PAMERAN_POSTER_1 3
// Distance (in meter) from LEDs to measuring point in area 'Pameran Poster 1'
#define DIST_LED_PAMERAN_POSTER_1 0.22

// Index of first LED in area 'Pameran Poster 2'
#define INDEX_LED_PAMERAN_POSTER_2 37
// Total number of LEDs used at area 'Pameran Poster 2'
#define NUM_LED_PAMERAN_POSTER_2 4
// Distance (in meter) from LEDs to measuring point in area 'Pameran Poster 2'
#define DIST_LED_PAMERAN_POSTER_2 0.22

// Index of first LED in area 'Panggung Serbaguna'
#define INDEX_LED_PANGGUNG_SERBAGUNA 8
// Total number of LEDs used at area 'Panggung Serbaguna'
#define NUM_LED_PANGGUNG_SERBAGUNA 2
// Distance (in meter) from LEDs to measuring point in area 'Panggung Serbaguna'
#define DIST_LED_PANGGUNG_SERBAGUNA 0.15

// Index of first LED in area 'Pejalan Kaki 1'
#define INDEX_LED_PEJALAN_KAKI_1 0
// Total number of LEDs used at area 'Pejalan Kaki 1'
#define NUM_LED_PEJALAN_KAKI_1 8
// Distance (in meter) from LEDs to measuring point in area 'Pejalan Kaki 1'
#define DIST_LED_PEJALAN_KAKI_1 0.22

// Index of first LED in area 'Pejalan Kaki 2'
#define INDEX_LED_PEJALAN_KAKI_2 27
// Total number of LEDs used at area 'Pejalan Kaki 2'
#define NUM_LED_PEJALAN_KAKI_2 7
// Distance (in meter) from LEDs to measuring point in area 'Pejalan Kaki 2'
#define DIST_LED_PEJALAN_KAKI_2 0.22

// Index of first LED in area 'Pejalan Kaki 3'
#define INDEX_LED_PEJALAN_KAKI_3 41
// Total number of LEDs used at area 'Pejalan Kaki 3'
#define NUM_LED_PEJALAN_KAKI_3 3
// Distance (in meter) from LEDs to measuring point in area 'Pejalan Kaki 3'
#define DIST_LED_PEJALAN_KAKI_3 0.22

// Index of first LED in area 'Pejalan Kaki 4'
#define INDEX_LED_PEJALAN_KAKI_4 44
// Total number of LEDs used at area 'Pejalan Kaki 4'
#define NUM_LED_PEJALAN_KAKI_4 3
// Distance (in meter) from LEDs to measuring point in area 'Pejalan Kaki 4'
#define DIST_LED_PEJALAN_KAKI_4 0.22

// Total number of LEDs used
#define NUM_LED NUM_LED_MEJA_DAN_KURSI_1+NUM_LED_MEJA_DAN_KURSI_2+NUM_LED_MEJA_DAN_KURSI_3+NUM_LED_PAMERAN_POSTER_1+NUM_LED_PAMERAN_POSTER_2+NUM_LED_PANGGUNG_SERBAGUNA+NUM_LED_PEJALAN_KAKI_1+NUM_LED_PEJALAN_KAKI_2+NUM_LED_PEJALAN_KAKI_3+NUM_LED_PEJALAN_KAKI_4

#endif