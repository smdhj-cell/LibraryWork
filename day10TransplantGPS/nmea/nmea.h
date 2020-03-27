#ifndef _NMEA_H_
#define _NMEA_H_
//#include "gnss_types.h"
#include "string.h"
#include <stdint.h>

/** GPS status event values. */
// typedef unsigned short int GpsStatusValue;
#define sys_SUCCESS 0
#define sys_FAILED 1

typedef struct
{
    int utc_year;
    int utc_mon;
    int utc_day;

    int utc_hour;
    int utc_min;
    int utc_sec;
    int utc_msec; //毫秒
} nmea_gps_time_t;

/** Represents of Gpslocation.flags */
/** GpsLocation has valid latitude and longitude. */
#define GPS_LOCATION_HAS_LAT_LONG 0x0001
/** GpsLocation has valid altitude. */
#define GPS_LOCATION_HAS_ALTITUDE 0x0002
/** GpsLocation has valid speed. */
#define GPS_LOCATION_HAS_SPEED 0x0004
/** GpsLocation has valid bearing. */
#define GPS_LOCATION_HAS_BEARING 0x0008
/** GpsLocation has valid accuracy. */
#define GPS_LOCATION_HAS_ACCURACY 0x0010

/** Represents a location. */
typedef struct
{
    /** set to sizeof(GpsLocation) */
    size_t size;
    /** Contains GpsLocationFlags bits. */
    uint16_t flags;
    /** Represents GPS Module Sync status */
    uint16_t status;
    /** Represents latitude in degrees. */
    double latitude;
    /** Represents longitude in degrees. */
    double longitude;
    /** Represents altitude in meters above the WGS 84 reference ellipsoid. */
    double altitude;
    /** Represents speed in meters per second. */
    float speed;
    /** Represents heading in degrees. */
    float bearing;
    /** Represents expected accuracy in meters. */
    float accuracy;
    /** Timestamp for the location fix. */
    nmea_gps_time_t timestamp;

    //使用卫星数量
    uint8_t useSatelliteNum;
    // GPS状态
    uint8_t gpsStatus;
    //差分时间
    float diffAge;

    //水平面差值
    float undulation;
    //水平精度因子
    float HDOP;

    //垂直精度因子
    float VDOP;

    //综合精度因子
    float PDOP;

} GpsLocation;

/** Maximum number of SVs  */
#define GPS_MAX_SVS 64

/** Represents SV information. */
typedef struct
{
    /** set to sizeof(GpsSvInfo) */
    size_t size;
    /** Pseudo-random number for the SV. */
    int prn;
    /** Signal to noise ratio. */
    float snr;
    /** Elevation of SV in degrees. */
    float elevation;
    /** Azimuth of SV in degrees. */
    float azimuth;
} GpsSvInfo;

/** Represents SV status. */
typedef struct
{
    /** set to sizeof(GpsSvStatus) */
    size_t size;

    /** Number of SVs currently visible. */
    int num_svs;

    /** Contains an array of SV information. */
    GpsSvInfo sv_list[GPS_MAX_SVS];

    /** Represents a bit mask indicating which SVs have ephemeris data. */
    uint32_t ephemeris_mask;

    /** Represents a bit mask indicating which SVs have almanac data. */
    uint32_t almanac_mask;

    /** Represents a bit mask indicating which SVs were used for computing the most recent position fix. */
    uint32_t used_in_fix_mask;
} GpsSvStatus;

typedef struct
{
    nmea_gps_time_t timestamp;
    /** 纬度基线投影长度，m */
    double projection_lat;
    /** 精度基线投影长度，m */
    double projection_lon;
    /**高度基线投影长度，m */
    double projection_altitude;

    /*base line len*/
    double baselinelen;

    /*正北航向夹角*/
    double heading;
    /*俯仰角*/
    double pitch;

    /* A=Autonomous mode
     * D=Differential mode
     * F=Float RTK
     * R=Real Time Kinematic
     * E=Estimated(dead reckoning) mode
     * N=Data not valid*/
    char status;

} BlsStatus;

#define NMEA_MAX_SIZE 128

typedef struct
{
    int pos;
    int overflow;
    int utc_year;
    int utc_mon;
    int utc_day;
    int utc_diff;
    GpsLocation fix;
    GpsSvStatus sv_status;
    BlsStatus bls_status;
    int sv_status_changed;

    char in[NMEA_MAX_SIZE + 1];
    int r_mutex;
} NmeaReader;

extern uint32_t nmea_init(void *p);
extern void nmea_reader_parse(char *in);

extern NmeaReader *nmea_reader_employ(void);

extern int nmea_reader_dismiss(void);

extern uint32_t nmea_reader_get_location(GpsLocation *plocation);
extern uint32_t nmea_reader_get_svstatus(GpsSvStatus *psvstatus);
extern uint32_t nmea_reader_get_utctime(nmea_gps_time_t *ptime);

#endif
