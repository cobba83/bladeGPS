#ifndef _BLADEGPS_H
#define _BLADEGPS_H

#include <stdlib.h>
#include <stdio.h>
#include <libbladeRF.h>
#include <string.h>
#include <time.h>
#include <math.h>
#ifdef _WIN32
// To avoid conflict between time.h and pthread.h on Windows
#define HAVE_STRUCT_TIMESPEC
#endif
#include <pthread.h>
#include "gpssim.h"

#define TX_FREQUENCY	1575420000
#define TX_SAMPLERATE	2600000
#define TX_BANDWIDTH	2500000
#define TX_GAIN		-5

#define NUM_BUFFERS			32
#define SAMPLES_PER_BUFFER	(32 * 1024)
#define NUM_TRANSFERS		16
#define TIMEOUT_MS			1000

#define NUM_IQ_SAMPLES  (TX_SAMPLERATE / 10)
#define FIFO_LENGTH     (NUM_IQ_SAMPLES * 2)

// Interactive mode directions
#define UNDEF 0
#define NORTH 1
#define SOUTH 2
#define EAST  3
#define WEST  4
#define UP    5
#define DOWN  6
// Interactive keys
#define NORTH_KEY 'w'
#define SOUTH_KEY 's'
#define EAST_KEY  'd'
#define WEST_KEY  'a'
#define UP_KEY    'e'
#define DOWN_KEY  'q'
// Interactive motion
#define MAX_VEL 2.7 // 2.77 m/s = 10 km/h
#define DEL_VEL 0.2

typedef struct {
	char navfile[MAX_CHAR];
	char almfile[MAX_CHAR];
	char umfile[MAX_CHAR];
	int staticLocationMode;
	int restInterfaceMode;
	int nmeaGGA;
	int iduration;
	int verb;
	gpstime_t g0;
	double llh[3];
	int interactive;
	int timeoverwrite;
	int iono_enable;
	int path_loss_enable;
} option_t;

typedef struct {
	pthread_t thread;
	pthread_mutex_t lock;
	//int error;

	struct bladerf *dev;
	int16_t *buffer;
} tx_t;

typedef struct {
	pthread_t thread;
	pthread_mutex_t lock;
	//int error;

	int ready;
	pthread_cond_t initialization_done;
} gps_t;

typedef struct {
	option_t opt;

	tx_t tx;
	gps_t gps;

	int status;
	bool finished;
	int16_t *fifo;
	long head, tail;
	size_t sample_length;

	pthread_cond_t fifo_read_ready;
	pthread_cond_t fifo_write_ready;

	double time;
} sim_t;

extern void *gps_task(void *arg);
extern int is_fifo_write_ready(sim_t *s);

#endif
