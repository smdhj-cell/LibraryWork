#include "nmea.h"
//#include "gnss_debug.h"
//#include "gnss_util.h"

#define xSemaphoreTake(x, y)
#define xSemaphoreGive(x)
#define xSemaphoreCreateMutex() (1)

#define portMAX_DELAY

/*****************************************************************/
/*****************************************************************/
/*****                                                       *****/
/*****       N M E A   T O K E N I Z E R                     *****/
/*****                                                       *****/
/*****************************************************************/
/*****************************************************************/

//typedef struct
//{
//    const char *p;
//    const char *end;
//} Token;
//
//#define MAX_NMEA_TOKENS (32)
//
//typedef struct
//{
//    int count;
//    Token tokens[MAX_NMEA_TOKENS];
//} NmeaTokenizer;
//
//static int nmea_tokenizer_init(NmeaTokenizer *t, const char *p, const char *end)
//{
//    int count = 0;
//    // char*  q = NULL;
//    int special_flag = 0;
//    const char *special_ptr = NULL;
//
//    // the initial '$' is optional
//    if (p < end && p[0] == '$')
//        p += 1;
//
//    // remove trailing newline
//    if (end > p && end[-1] == '\n')
//    {
//        end -= 1;
//        if (end > p && end[-1] == '\r')
//            end -= 1;
//    }
//
//    // get rid of checksum at the end of the sentecne
//    if (end >= p + 3 && end[-3] == '*')
//    {
//        end -= 3;
//    }
//
//    // special handle "...,123,*68"
//    if (end >= p + 1 && end[-1] == ',')
//    {
//        special_flag = 1;
//        special_ptr = end - 1;
//    }
//
//    while (p < end)
//    {
//        const char *q = p;
//
//        q = (const char *)gnss_memchr(p, ',', end - p);
//        if (q == NULL)
//            q = end;
//
//        if (q >= p)
//        {
//            if (count < MAX_NMEA_TOKENS)
//            {
//                t->tokens[count].p = p;
//                t->tokens[count].end = q;
//                count += 1;
//            }
//        }
//        if (q < end)
//            q += 1;
//
//        p = q;
//    }
//
//    if (1 == special_flag)
//    {
//        if (count < MAX_NMEA_TOKENS)
//        {
//            t->tokens[count].p = special_ptr;
//            t->tokens[count].end = special_ptr;
//            count += 1;
//        }
//    }
//
//    t->count = count;
//    return count;
//}
//
//static Token nmea_tokenizer_get(NmeaTokenizer *t, int index)
//{
//    Token tok;
//    static const char *dummy = "";
//
//    if (index < 0 || index >= t->count)
//    {
//        tok.p = tok.end = dummy;
//    }
//    else
//        tok = t->tokens[index];
//
//    return tok;
//}
//
//static int str2int(const char *p, const char *end)
//{
//    int result = 0;
//    int len = end - p;
//
//    for (; len > 0; len--, p++)
//    {
//        int c;
//
//        if (p >= end)
//            goto Fail;
//
//        c = *p - '0';
//        if ((unsigned)c >= 10)
//            goto Fail;
//
//        result = result * 10 + c;
//    }
//    return result;
//
//Fail:
//    return -1;
//}
//
//static double str2float(const char *p, const char *end)
//{
//    int len = end - p;
//    char temp[16];
//
//    if (len == 0 || len >= (int)sizeof(temp))
//    {
//        return 0.;
//    }
//
//    memcpy(temp, p, len);
//    temp[len] = 0;
//    return gnss_atof(temp);
//}

/*****************************************************************/
/*****************************************************************/
/*****                                                       *****/
/*****       N M E A   P A R S E R                           *****/
/*****                                                       *****/
/*****************************************************************/
/*****************************************************************/

/** Point to gnss.c gnss_handler_t.nmea_reader */
static NmeaReader *pnmea_reader = NULL;
//
//static int nmea_reader_update_time(NmeaReader *r, Token tok_time)
//{
//    int hour = 0;
//    int min = 0;
//    double sec = 0.0;
//    Token tok = tok_time;
//    uint32_t tmp;
//
//    if (tok.p + 6 > tok.end)
//    {
//        return -1;
//    }
//
//    if (r->utc_year < 0 || r->fix.timestamp.utc_year < 0)
//    {
//        // no date yet,  set default value : 1980.01.06
//        r->utc_year = 1980;
//        r->utc_mon = 1;
//        r->utc_day = 6;
//
//        r->fix.timestamp.utc_year = 1980;
//        r->fix.timestamp.utc_mon = 1;
//        r->fix.timestamp.utc_day = 6;
//    }
//
//    hour = str2int(tok.p, tok.p + 2);
//    min = str2int(tok.p + 2, tok.p + 4);
//    sec = str2float(tok.p + 4, tok.end);
//
//    r->fix.timestamp.utc_hour = hour;
//    r->fix.timestamp.utc_min = min;
//    r->fix.timestamp.utc_sec = sec;
//    tmp = (uint32_t)(sec * 1000);
//    r->fix.timestamp.utc_msec = tmp % 1000;
//    return 0;
//}
//
//static int nmea_reader_update_date(NmeaReader *r, Token date, Token time)
//{
//    Token tok = date;
//    int day, mon, year;
//
//    if (tok.p + 6 != tok.end)
//    {
//        return -1;
//    }
//
//    day = str2int(tok.p, tok.p + 2);
//    mon = str2int(tok.p + 2, tok.p + 4);
//    year = str2int(tok.p + 4, tok.p + 6) + 2000;
//
//    if ((day | mon | year) < 0)
//    {
//        return -1;
//    }
//
//    r->utc_year = year;
//    r->utc_mon = mon;
//    r->utc_day = day;
//
//    /** sync fix.timestamp */
//    r->fix.timestamp.utc_year = year;
//    r->fix.timestamp.utc_mon = mon;
//    r->fix.timestamp.utc_day = day;
//
//    return nmea_reader_update_time(r, time);
//}
//
//static double nmea_convert_from_hhmm(Token tok)
//{
//    double val = str2float(tok.p, tok.end);
//    int degrees = (int)((int)(val) / 100);
//    double minutes = val - degrees * 100.;
//    double dcoord = degrees + minutes / 60.0;
//    return dcoord;
//}
//
//static int nmea_reader_update_latlong(NmeaReader *r, Token latitude, char latitudeHemi, Token longitude, char longitudeHemi)
//{
//    double lat, lon;
//    Token tok;
//
//    /** Latitude  */
//    tok = latitude;
//    if (tok.p + 6 > tok.end)
//    {
//        return -1;
//    }
//    lat = nmea_convert_from_hhmm(tok);
//    if (latitudeHemi == 'S')
//        lat = -lat;
//
//    /** Longtitude  */
//    tok = longitude;
//    if (tok.p + 6 > tok.end)
//    {
//        return -1;
//    }
//    lon = nmea_convert_from_hhmm(tok);
//    if (longitudeHemi == 'W')
//        lon = -lon;
//
//    /** set fix.flags */
//    r->fix.flags |= GPS_LOCATION_HAS_LAT_LONG;
//    r->fix.latitude = lat;
//    r->fix.longitude = lon;
//
//    return 0;
//}
//
//static int nmea_reader_update_useSatelliteNum(NmeaReader *r, Token useSatelliteNum)
//{
//    Token tok = useSatelliteNum;
//
//    if (tok.p >= tok.end)
//    {
//        return -1;
//    }
//
//    r->fix.useSatelliteNum = str2int(tok.p, tok.end);
//    return 0;
//}
//static int nmea_reader_update_gpsStatus(NmeaReader *r, Token gpsStatus)
//{
//    Token tok = gpsStatus;
//
//    if (tok.p >= tok.end)
//    {
//        return -1;
//    }
//
//    r->fix.gpsStatus = str2int(tok.p, tok.end);
//    return 0;
//}
//
//static int nmea_reader_update_diffAge(NmeaReader *r, Token diffAge)
//{
//    Token tok = diffAge;
//
//    if (tok.p >= tok.end)
//    {
//        return -1;
//    }
//
//    r->fix.diffAge = str2float(tok.p, tok.end);
//    return 0;
//}
//
//static int nmea_reader_update_undulation(NmeaReader *r, Token undulation)
//{
//    Token tok = undulation;
//
//    if (tok.p >= tok.end)
//    {
//        return -1;
//    }
//
//    r->fix.undulation = str2float(tok.p, tok.end);
//    return 0;
//}
//
//static int nmea_reader_update_hdop(NmeaReader *r, Token hdop)
//{
//    Token tok = hdop;
//
//    if (tok.p >= tok.end)
//    {
//        return -1;
//    }
//
//    r->fix.HDOP = str2float(tok.p, tok.end);
//    return 0;
//}
//
//static int nmea_reader_update_altitude(NmeaReader *r, Token altitude, Token units)
//{
//    Token tok = altitude;
//
//    if (tok.p >= tok.end)
//    {
//        return -1;
//    }
//
//    r->fix.flags |= GPS_LOCATION_HAS_ALTITUDE;
//    r->fix.altitude = str2float(tok.p, tok.end);
//    return 0;
//}
//
//static int nmea_reader_update_bearing(NmeaReader *r, Token bearing)
//{
//    Token tok = bearing;
//
//    if (tok.p >= tok.end)
//    {
//        return -1;
//    }
//
//    r->fix.flags |= GPS_LOCATION_HAS_BEARING;
//    r->fix.bearing = str2float(tok.p, tok.end);
//    return 0;
//}
//
//static int nmea_reader_update_speed(NmeaReader *r, Token speed)
//{
//    Token tok = speed;
//
//    if (tok.p >= tok.end)
//    {
//        return -1;
//    }
//
//    r->fix.flags |= GPS_LOCATION_HAS_SPEED;
//    r->fix.speed = str2float(tok.p, tok.end);
//    return 0;
//}
//
//static int nmea_reader_update_accuracy(NmeaReader *r, Token accuracy)
//{
//    Token tok = accuracy;
//
//    if (tok.p >= tok.end)
//    {
//        return -1;
//    }
//
//    r->fix.accuracy = str2float(tok.p, tok.end);
//
//    if (r->fix.accuracy == 99.99f)
//    {
//        return 0;
//    }
//
//    r->fix.flags |= GPS_LOCATION_HAS_ACCURACY;
//    return 0;
//}
//
//static int nmea_reader_update_fixStatus(NmeaReader *r, Token fixStatus)
//{
//    Token tok = fixStatus;
//
//    if (tok.p >= tok.end)
//    {
//        return -1;
//    }
//
//    r->fix.status = tok.p[0];
//    return 0;
//}
//
//static int nmea_reader_update_bls_time(NmeaReader *r, Token tok_time)
//{
//
//    int hour = 0;
//    int min = 0;
//    double sec = 0.0;
//    Token tok = tok_time;
//    uint32_t tmp;
//
//    if (tok.p + 6 > tok.end)
//    {
//        return -1;
//    }
//
//    r->bls_status.timestamp.utc_year = r->fix.timestamp.utc_year;
//    r->bls_status.timestamp.utc_mon = r->fix.timestamp.utc_mon;
//    r->bls_status.timestamp.utc_day = r->fix.timestamp.utc_day;
//
//    hour = str2int(tok.p, tok.p + 2);
//    min = str2int(tok.p + 2, tok.p + 4);
//    sec = str2float(tok.p + 4, tok.end);
//
//    r->bls_status.timestamp.utc_hour = hour;
//    r->bls_status.timestamp.utc_min = min;
//    r->bls_status.timestamp.utc_sec = sec;
//    tmp = (uint32_t)(sec * 1000);
//    r->bls_status.timestamp.utc_msec = tmp % 1000;
//    return 0;
//}
//
//static int nmea_reader_update_bls_projection(NmeaReader *r, Token tok_projection_lat, Token tok_projection_lon, Token tok_projection_altitude)
//{
//
//    r->bls_status.projection_lat = str2float(tok_projection_lat.p, tok_projection_lat.end);
//    r->bls_status.projection_lon = str2float(tok_projection_lon.p, tok_projection_lon.end);
//    r->bls_status.projection_altitude = str2float(tok_projection_altitude.p, tok_projection_altitude.end);
//
//    return 0;
//}
//
//static int nmea_reader_update_bls_status(NmeaReader *r, Token tok_baselinelen, Token tok_heading, Token tok_pitch, Token tok_blsStatus)
//{
//    r->bls_status.baselinelen = str2float(tok_baselinelen.p, tok_baselinelen.end);
//    r->bls_status.heading = str2float(tok_heading.p, tok_heading.end);
//    r->bls_status.pitch = str2float(tok_pitch.p, tok_pitch.end);
//
//    r->bls_status.status = tok_blsStatus.p[0];
//    if (('A' != r->bls_status.status) && ('D' != r->bls_status.status) && ('F' != r->bls_status.status) && ('R' != r->bls_status.status) && ('E' != r->bls_status.status) &&
//        ('N' != r->bls_status.status))
//    {
//
//        r->bls_status.status = 'N';
//    }
//
//#if 0
//
//    NMEA_DEBUG("bls time:%d-%d-%d %d:%d:%d.%d\r\n",r->bls_status.timestamp.utc_year,
//    		                               r->bls_status.timestamp.utc_mon,
//										   r->bls_status.timestamp.utc_day,
//										   r->bls_status.timestamp.utc_hour,
//										   r->bls_status.timestamp.utc_min,
//										   r->bls_status.timestamp.utc_sec,
//										   r->bls_status.timestamp.utc_msec);
//
//    NMEA_DEBUG("bls info:lat:%f,lon:%f,alt:%f,bllen:%f,heading:%f,pitch:%f,mode:%c\r\n",
//    		                               r->bls_status.projection_lat,
//										   r->bls_status.projection_lon,
//										   r->bls_status.projection_altitude,
//										   r->bls_status.baselinelen,
//										   r->bls_status.heading,
//										   r->bls_status.pitch,
//										   r->bls_status.status);
//#endif
//    return 0;
//}
//
//void nmea_reader_parse(char *in)
//{
//    static NmeaTokenizer tzer[1];
//    Token tok;
//    const char *ptr = NULL;
//    static int beidou_sv;
//    NmeaReader *r = pnmea_reader;
//
//    if (r == NULL || NULL == in)
//    {
//        return;
//    }
//    if (strlen(in) < 9)
//    {
//        return;
//    }
//
//    nmea_tokenizer_init(tzer, in, in + strlen(in));
//
//    tok = nmea_tokenizer_get(tzer, 0);
//    if (tok.p + 5 > tok.end)
//    {
//        return;
//    }
//
//    /** Take locked */
////    xSemaphoreTake(pnmea_reader->r_mutex, portMAX_DELAY);
//
//    ptr = tok.p;
//    /**  ignore first two characters. */
//    tok.p += 2;
//    if (!G_memcmp(tok.p, "GGA", 3))
//    {
//        // GPS fix
//        // Token  tok_time          = nmea_tokenizer_get(tzer, 1);
//        // Token  tok_latitude      = nmea_tokenizer_get(tzer, 2);
//        // Token  tok_latitudeHemi  = nmea_tokenizer_get(tzer, 3);
//        // Token  tok_longitude     = nmea_tokenizer_get(tzer, 4);
//        // Token  tok_longitudeHemi = nmea_tokenizer_get(tzer, 5);
//        Token tok_fixStatus = nmea_tokenizer_get(tzer, 6);
//        Token tok_useSatelliteNum = nmea_tokenizer_get(tzer, 7);
//
//        Token tok_altitude = nmea_tokenizer_get(tzer, 9);
//        Token tok_altitudeUnits = nmea_tokenizer_get(tzer, 10);
//        Token tok_undulation = nmea_tokenizer_get(tzer, 11);
//        Token tok_diffAge = nmea_tokenizer_get(tzer, 13);
//
//        nmea_reader_update_gpsStatus(r, tok_fixStatus);
//        if ((tok_fixStatus.p[0] == '1') || //单点定位
//            (tok_fixStatus.p[0] == '2') || //差分定位
//            (tok_fixStatus.p[0] == '4') || //固定解
//            (tok_fixStatus.p[0] == '5') || //浮点解
//            (tok_fixStatus.p[0] == '7') || //人工输入固定值
//            (tok_fixStatus.p[0] == '9'))   // wAAS差分
//
//        {
//
//            /* nmea_reader_update_time(r, tok_time);
//             nmea_reader_update_latlong(r, tok_latitude,
//                                       tok_latitudeHemi.p[0],
//                                       tok_longitude,
//                                       tok_longitudeHemi.p[0]);
//       */
//            nmea_reader_update_useSatelliteNum(r, tok_useSatelliteNum);
//
//            nmea_reader_update_diffAge(r, tok_diffAge);
//            nmea_reader_update_undulation(r, tok_undulation);
//
//            nmea_reader_update_altitude(r, tok_altitude, tok_altitudeUnits);
//        }
//    }
//    else if (!G_memcmp(tok.p, "GSA", 3))
//    {
//        int is_beidou = !G_memcmp(ptr, "BD", 2);           // BD: is_beidou=1;GPS or GN : is_beidou=0
//        Token tok_fixStatus = nmea_tokenizer_get(tzer, 2); // position mode : 0-not; 1-2D;2-3D
//        Token tok_accuracy = nmea_tokenizer_get(tzer, 15); // position dilution of precision dop
//        Token tok_hdop = nmea_tokenizer_get(tzer, 16);     //
//
//        int i;
//#if 0
//        {    
//             NMEA_DEBUG("MSG=%s",in);
//             NMEA_DEBUG("0,%x,%x|1,%x,%x|2,%x,%x|3,%x,%x|4,%x,%x|\
//                          5,%x,%x|6,%x,%x|7,%x,%x|8,%x,%x|9,%x,%x|\
//                          10,%x,%x|11,%x,%x|12,%x,%x|13,%x,%x|14,%x,%x|\
//                          15,%x,%x|16,%x,%x|17,%x,%x|18,%x,%x|19,%x,%x\r\n",
//                          nmea_tokenizer_get(tzer, 0).p,nmea_tokenizer_get(tzer, 0).end,
//                          nmea_tokenizer_get(tzer, 1).p,nmea_tokenizer_get(tzer, 1).end,
//                          nmea_tokenizer_get(tzer, 2).p,nmea_tokenizer_get(tzer, 2).end,
//                          nmea_tokenizer_get(tzer, 3).p,nmea_tokenizer_get(tzer, 3).end,
//                          nmea_tokenizer_get(tzer, 4).p,nmea_tokenizer_get(tzer, 4).end,
//                          nmea_tokenizer_get(tzer, 5).p,nmea_tokenizer_get(tzer, 5).end,
//                          nmea_tokenizer_get(tzer, 6).p,nmea_tokenizer_get(tzer, 6).end,
//                          nmea_tokenizer_get(tzer, 7).p,nmea_tokenizer_get(tzer, 7).end,
//                          nmea_tokenizer_get(tzer, 8).p,nmea_tokenizer_get(tzer, 8).end,
//                          nmea_tokenizer_get(tzer, 9).p,nmea_tokenizer_get(tzer, 9).end,
//                          nmea_tokenizer_get(tzer, 10).p,nmea_tokenizer_get(tzer, 10).end,
//                          nmea_tokenizer_get(tzer, 11).p,nmea_tokenizer_get(tzer, 11).end,
//                          nmea_tokenizer_get(tzer, 12).p,nmea_tokenizer_get(tzer, 12).end,
//                          nmea_tokenizer_get(tzer, 13).p,nmea_tokenizer_get(tzer, 13).end,
//                          nmea_tokenizer_get(tzer, 14).p,nmea_tokenizer_get(tzer, 14).end,
//                          nmea_tokenizer_get(tzer, 15).p,nmea_tokenizer_get(tzer, 15).end,
//                          nmea_tokenizer_get(tzer, 16).p,nmea_tokenizer_get(tzer, 16).end,
//                          nmea_tokenizer_get(tzer, 17).p,nmea_tokenizer_get(tzer, 17).end,
//                          nmea_tokenizer_get(tzer, 18).p,nmea_tokenizer_get(tzer, 18).end,
//                          nmea_tokenizer_get(tzer, 19).p,nmea_tokenizer_get(tzer, 19).end);
//        }
//#endif
//
//        if (tok_fixStatus.p[0] != '\0' && 0 == is_beidou)
//        {
//            // Always update accuracy
//            nmea_reader_update_accuracy(r, tok_accuracy);
//            nmea_reader_update_hdop(r, tok_hdop);
//
//            r->sv_status.used_in_fix_mask = 0ul; // clear mask
//
//            for (i = 3; i <= 14; ++i)
//            {
//                Token tok_prn = nmea_tokenizer_get(tzer, i);
//                int prn = str2int(tok_prn.p, tok_prn.end);
//
//                if (prn > 0 && prn < 32)
//                {
//                    r->sv_status.used_in_fix_mask |= (1ul << (prn - 1));
//                }
//            }
//
//            r->sv_status_changed = 1;
//        }
//    }
//    else if (!G_memcmp(tok.p, "GSV", 3))
//    {
//        int is_beidou = !memcmp(ptr, "BD", 2); // BD: is_beidou=1;GPS or GN : is_beidou=0
//        Token tok_noSatellites = nmea_tokenizer_get(tzer, 3);
//        int noSatellites = str2int(tok_noSatellites.p, tok_noSatellites.end);
//        int tmp;
//
//        if (noSatellites > 0)
//        {
//            Token tok_noSentences = nmea_tokenizer_get(tzer, 1);
//            Token tok_sentence = nmea_tokenizer_get(tzer, 2);
//
//            int sentence = str2int(tok_sentence.p, tok_sentence.end);
//            int totalSentences = str2int(tok_noSentences.p, tok_noSentences.end);
//            int curr;
//            int i;
//
//            if (sentence == 1 && 0 == is_beidou)
//            {
//                // GPGSV / GNGSV : first sentence, clear cache
//                r->sv_status.num_svs = 0;
//                r->sv_status.ephemeris_mask = 0ul;
//                r->sv_status.almanac_mask = 0ul;
//                memset(r->sv_status.sv_list, 0, sizeof(r->sv_status.sv_list));
//            }
//
//            curr = r->sv_status.num_svs;
//
//            i = 0;
//            if ((sentence == 1) && 1 == is_beidou)
//            {
//                noSatellites += r->sv_status.num_svs;
//                beidou_sv = noSatellites;
//            }
//
//            if (1 == is_beidou)
//            {
//                tmp = beidou_sv;
//            }
//            else
//            {
//                tmp = noSatellites;
//            }
//
//            while (i < 4 && r->sv_status.num_svs < tmp)
//            {
//                Token tok_prn = nmea_tokenizer_get(tzer, i * 4 + 4);
//                Token tok_elevation = nmea_tokenizer_get(tzer, i * 4 + 5);
//                Token tok_azimuth = nmea_tokenizer_get(tzer, i * 4 + 6);
//                Token tok_snr = nmea_tokenizer_get(tzer, i * 4 + 7);
//
//                r->sv_status.sv_list[curr].prn = str2int(tok_prn.p, tok_prn.end);
//                // D("r->sv_status.sv_list[%d].prn = %d", curr, r->sv_status.sv_list[curr].prn);
//
//                if (1 == is_beidou && r->sv_status.sv_list[curr].prn < 200)
//                {
//                    r->sv_status.sv_list[curr].prn += 200;
//                    // D("BDGSV : r->sv_status.sv_list[%d].prn = %d", curr, r->sv_status.sv_list[curr].prn);
//                }
//
//                r->sv_status.sv_list[curr].elevation = str2float(tok_elevation.p, tok_elevation.end);
//                r->sv_status.sv_list[curr].azimuth = str2float(tok_azimuth.p, tok_azimuth.end);
//                r->sv_status.sv_list[curr].snr = str2float(tok_snr.p, tok_snr.end);
//
//                r->sv_status.ephemeris_mask |= (1ul << (r->sv_status.sv_list[curr].prn - 1));
//                r->sv_status.almanac_mask |= (1ul << (r->sv_status.sv_list[curr].prn - 1));
//                r->sv_status.num_svs += 1;
//                curr += 1;
//                i += 1;
//            }
//
//            if (sentence == totalSentences && 1 == is_beidou)
//            {
//                // D("totalSentences=%d\n,sentence=%d\n,curr=%d\n,noSatellites=%d\n,r->sv_status.num_svs=%d\n",totalSentences,sentence,curr,noSatellites,r->sv_status.num_svs);
//            }
//        }
//    }
//    else if (!G_memcmp(tok.p, "GLL", 3))
//    {
//        Token tok_fixstaus = nmea_tokenizer_get(tzer, 6);
//
//        if (tok_fixstaus.p[0] == 'A')
//        {
//            Token tok_latitude = nmea_tokenizer_get(tzer, 1);
//            Token tok_latitudeHemi = nmea_tokenizer_get(tzer, 2);
//            Token tok_longitude = nmea_tokenizer_get(tzer, 3);
//            Token tok_longitudeHemi = nmea_tokenizer_get(tzer, 4);
//            // Token tok_time = nmea_tokenizer_get(tzer,5);
//
//            nmea_reader_update_latlong(r, tok_latitude, tok_latitudeHemi.p[0], tok_longitude, tok_longitudeHemi.p[0]);
//        }
//    }
//    else if (!G_memcmp(tok.p, "RMC", 3))
//    {
//        Token tok_time = nmea_tokenizer_get(tzer, 1);
//        Token tok_fixStatus = nmea_tokenizer_get(tzer, 2);
//        Token tok_latitude = nmea_tokenizer_get(tzer, 3);
//        Token tok_latitudeHemi = nmea_tokenizer_get(tzer, 4);
//        Token tok_longitude = nmea_tokenizer_get(tzer, 5);
//        Token tok_longitudeHemi = nmea_tokenizer_get(tzer, 6);
//        Token tok_speed = nmea_tokenizer_get(tzer, 7);
//        Token tok_bearing = nmea_tokenizer_get(tzer, 8);
//        Token tok_date = nmea_tokenizer_get(tzer, 9);
//
//        // Allways update
//        nmea_reader_update_fixStatus(r, tok_fixStatus);
//
//        // D("in RMC, fixStatus=%c", tok_fixStatus.p[0]);
//        if (tok_fixStatus.p[0] == 'A')
//        {
//            nmea_reader_update_date(r, tok_date, tok_time);
//
//            nmea_reader_update_latlong(r, tok_latitude, tok_latitudeHemi.p[0], tok_longitude, tok_longitudeHemi.p[0]);
//
//            nmea_reader_update_bearing(r, tok_bearing);
//            nmea_reader_update_speed(r, tok_speed);
//
//            if (r->fix.flags != 0)
//            {
//                // do something?
//                r->fix.flags = 0;
//            }
//#if 0
//            NMEA_DEBUG("[fix]size=%d,status=%c,flags=%x,latitude=%f,longitude=%f,altitude=%f,speed=%f,accuracy=%f,year=%d,mon=%d,day=%d,hour=%d,min=%d,sec=%d",
//                        r->fix.size,
//                        r->fix.status,
//                        r->fix.flags,
//                        r->fix.latitude,
//                        r->fix.longitude,
//                        r->fix.altitude,
//                        r->fix.speed,
//                        r->fix.accuracy,
//                        r->tm.utc_year,
//                        r->tm.utc_mon,
//                        r->tm.utc_day,
//                        r->tm.utc_hour,
//                        r->tm.utc_min,
//                        r->tm.utc_sec);
//#endif
//        }
//    }
//
//    else if (!G_memcmp(tok.p, "LYSBLS", 6))
//    {
//        Token tok_time = nmea_tokenizer_get(tzer, 1);
//        Token tok_projection_lat = nmea_tokenizer_get(tzer, 2);
//        Token tok_projection_lon = nmea_tokenizer_get(tzer, 3);
//        Token tok_projection_altitude = nmea_tokenizer_get(tzer, 4);
//        Token tok_baselinelen = nmea_tokenizer_get(tzer, 5);
//        Token tok_heading = nmea_tokenizer_get(tzer, 6);
//        Token tok_pitch = nmea_tokenizer_get(tzer, 7);
//        Token tok_blsStatus = nmea_tokenizer_get(tzer, 8);
//
//        nmea_reader_update_bls_time(r, tok_time);
//
//        nmea_reader_update_bls_projection(r, tok_projection_lat, tok_projection_lon, tok_projection_altitude);
//
//        nmea_reader_update_bls_status(r, tok_baselinelen, tok_heading, tok_pitch, tok_blsStatus);
//    }
//    else
//    {
//        tok.p -= 2;
//    }
//
//    xSemaphoreGive(pnmea_reader->r_mutex);
//}


/*****************************************************************/
/*****************************************************************/
/*****                                                         *****/
/*****      USER   Interface                           *****/
/*****                                                         *****/
/*****************************************************************/
/*****************************************************************/

uint32_t nmea_init(void *p)
{
    if (pnmea_reader != NULL || NULL == p)
    {
        return sys_FAILED;
    }

    pnmea_reader = (NmeaReader *)p;
//    memset(pnmea_reader, 0, sizeof(NmeaReader));
    pnmea_reader->pos = 0;
    pnmea_reader->overflow = 0;
    pnmea_reader->utc_year = -1;
    pnmea_reader->utc_mon = -1;
    pnmea_reader->utc_day = -1;
    pnmea_reader->fix.timestamp.utc_year = -1;
    pnmea_reader->fix.timestamp.utc_mon = -1;
    pnmea_reader->fix.timestamp.utc_day = -1;
    pnmea_reader->fix.size = sizeof(GpsLocation);

    pnmea_reader->r_mutex = xSemaphoreCreateMutex();
    if (pnmea_reader->r_mutex == NULL)
    {
        pnmea_reader = NULL;
        return sys_FAILED;
    }

    return sys_SUCCESS;
}

/*鍦ㄥ绾跨▼鎯呭喌涓嬶紝璺焠mea_reader_dismiss閰嶅悎浣跨敤*/
NmeaReader *nmea_reader_employ(void)
{

    if (pnmea_reader == NULL)
        return NULL;

    xSemaphoreTake(&pnmea_reader->r_mutex, portMAX_DELAY);

    return pnmea_reader;
}

int nmea_reader_dismiss(void)
{

    if (pnmea_reader == NULL)
        return sys_FAILED;

    xSemaphoreGive(&pnmea_reader->r_mutex);

    return sys_SUCCESS;
}
uint32_t nmea_reader_get_location(GpsLocation *plocation)
{

    if (pnmea_reader == NULL)
    {
        return sys_FAILED;
    }

    xSemaphoreTake(pnmea_reader->r_mutex, portMAX_DELAY);

    memcpy(plocation, &pnmea_reader->fix, sizeof(GpsLocation));

    xSemaphoreGive(pnmea_reader->r_mutex);

    return sys_SUCCESS;
}

uint32_t nmea_reader_get_svstatus(GpsSvStatus *psvstatus)
{

    if (pnmea_reader == NULL)
    {
        return sys_FAILED;
    }

    xSemaphoreTake(pnmea_reader->r_mutex, portMAX_DELAY);

    memcpy(psvstatus, &pnmea_reader->sv_status, sizeof(GpsSvStatus));

    xSemaphoreGive(pnmea_reader->r_mutex);

    return sys_SUCCESS;
}

uint32_t nmea_reader_get_utctime(nmea_gps_time_t *ptime)
{

    if (pnmea_reader == NULL)
    {
        return sys_FAILED;
    }

    xSemaphoreTake(pnmea_reader->r_mutex, portMAX_DELAY);

    memcpy(ptime, &pnmea_reader->fix.timestamp, sizeof(nmea_gps_time_t));

    xSemaphoreGive(pnmea_reader->r_mutex);

    return sys_SUCCESS;
}
