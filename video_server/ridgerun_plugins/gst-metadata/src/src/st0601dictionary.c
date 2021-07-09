/*  
 *  Copyright (C) 2019 RidgeRun, LLC (http://www.ridgerun.com)
 *  All Rights Reserved. 
 *
 *  The contents of this software are proprietary and confidential to RidgeRun, 
 *  LLC.  No part of this program may be photocopied, reproduced or translated 
 *  into another programming language without prior written consent of
 *  RidgeRun, LLC.  The user is free to modify the source code after obtaining 
 *  a software license from RidgeRun.  All source code changes must be provided 
 *  back to RidgeRun without any encumbrance. 
 */
#include "st0601dictionary.h"

#include <string.h>

#include "klv.h"
#include "lds.h"
#include "misbstd.h"

static const Key uas_lds_key = {
  .byte = {
        0x06, 0x0E, 0x2B, 0x34,
        0x02, 0x0B, 0x01, 0x01,
        0x0E, 0x01, 0x03, 0x01,
      0x01, 0x00, 0x00, 0x00}
};

#define TAG_NAME_MAX_LENGTH 64
typedef struct
{
  guchar name[TAG_NAME_MAX_LENGTH];
  guint length;
} TagStruct;

static TagStruct uas_tags[] = {
  [UAS_TAG_NOTAG] = {"Unknown", -1},
  [UAS_TAG_CHECKSUM] = {"Checksum", 2},
  [UAS_TAG_TIMESTAMP] = {"Timestamp", 8},
  [UAS_TAG_MISSION_ID] = {"Mission ID", 0},
  [UAS_TAG_PLATFORM_TAIL_NUMBER] = {"Tail number", 0},
  [UAS_TAG_PLATFORM_HEADING_ANGLE] = {"Platform Heading Angle", 2},
  [UAS_TAG_PLATFORM_PITCH_ANGLE] = {"Platform Pitch Angle", 2},
  [UAS_TAG_PLATFORM_ROLL_ANGLE] = {"Platform Roll Angle", 2},
  [UAS_TAG_PLATFORM_TRUE_AIRSPEED] = {"Platform True Airspeed", 1},
  [UAS_TAG_PLATFORM_INDICATED_AIRSPEED] = {"Platform Indicated Airspeed", 1},
  [UAS_TAG_PLATFORM_DESIGNATION] = {"Platform Designation", 0},
  [UAS_TAG_IMAGE_SOURCE_SENSOR] = {"Image Source Sensor", 0},
  [UAS_TAG_IMAGE_COORDINATE_SYSTEM] = {"Image Coordinate System", 0},
  [UAS_TAG_SENSOR_LATITUDE] = {"Sensor Latitude", 4},
  [UAS_TAG_SENSOR_LONGITUDE] = {"Sensor Longitude", 4},
  [UAS_TAG_SENSOR_TRUE_ALTITUDE] = {"Sensor True Altitude", 2},
  [UAS_TAG_SENSOR_HOR_FIELD_OF_VIEW] = {"Sensor Hor Field of View", 2},
  [UAS_TAG_SENSOR_VER_FIELD_OF_VIEW] = {"Sensor Ver Field of View", 2},
  [UAS_TAG_SENSOR_REL_AZIMUTH_ANGLE] = {"Sensor Relative Azimuth Angle", 4},
  [UAS_TAG_SENSOR_REL_ELEVATION_ANGLE] = {"Sensor Relative Elevation Angle", 4},
  [UAS_TAG_SENSOR_REL_ROLL_ANGLE] = {"Sensor Relative Roll Angle", 4},
  [UAS_TAG_SLANT_RANGE] = {"Slant Range", 4},
  [UAS_TAG_TARGET_WIDTH] = {"Target Width", 2},
  [UAS_TAG_FRAME_CENTER_LATITUDE] = {"Frame Center Latitude", 4},
  [UAS_TAG_FRAME_CENTER_LONGITUDE] = {"Frame Center Longitude", 4},
  [UAS_TAG_FRAME_CENTER_ELEVATION] = {"Frame Center Elevation", 2},
  [UAS_TAG_OFF_CORN_LAT_POINT_1] = {"Offset Corn Lat Point 1", 2},
  [UAS_TAG_OFF_CORN_LONG_POINT_1] = {"Offset Corn Long Point 1", 2},
  [UAS_TAG_OFF_CORN_LAT_POINT_2] = {"Offset Corn Lat Point 2", 2},
  [UAS_TAG_OFF_CORN_LONG_POINT_2] = {"Offset Corn Long Point 2", 2},
  [UAS_TAG_OFF_CORN_LAT_POINT_3] = {"Offset Corn Lat Point 3", 2},
  [UAS_TAG_OFF_CORN_LONG_POINT_3] = {"Offset Corn Long Point 3", 2},
  [UAS_TAG_OFF_CORN_LAT_POINT_4] = {"Offset Corn Lat Point 4", 2},
  [UAS_TAG_OFF_CORN_LONG_POINT_4] = {"Offset Corn Long Point 4", 2},
  [UAS_TAG_ICING_DETECTED] = {"Icing detected", 1},
  [UAS_TAG_WIND_DIRECTION] = {"Wind direction", 2},
  [UAS_TAG_WIND_SPEED] = {"Wind Speed", 1},
  [UAS_TAG_STATIC_PRESSURE] = {"Static pressure", 2},
  [UAS_TAG_DENSITY_ALTITUDE] = {"Density Altitude", 2},
  [UAS_TAG_OUTSIDE_AIR_TEMPERATURE] = {"Outside Air Temperature", 1},
  [UAS_TAG_TARGET_LOCATION_LATITUDE] = {"Target Location Latitude", 4},
  [UAS_TAG_TARGET_LOCATION_LONGITUDE] = {"Target Location Longitude", 4},
  [UAS_TAG_TARGET_LOCATION_ELEVATION] = {"Target Location Elevation", 2},
  [UAS_TAG_TARGET_TRACK_GATE_WIDTH] = {"Target Track Gate Width", 1},
  [UAS_TAG_TARGET_TRACK_GATE_HEIGHT] = {"Target Track Gate Height", 1},
  [UAS_TAG_TARGET_ERROR_EST_CE90] = {"Target Error Estimate CE90", 2},
  [UAS_TAG_TARGET_ERROR_EST_LE90] = {"Target Error Estimate CL90", 2},
  [UAS_TAG_GENERIC_FLAG_DATA] = {"Generic Flag Data", 1},
  [UAS_TAG_SECURITY_LOCAL_SET] = {"Security Local Set", 0},
  [UAS_TAG_DIFFERENTIAL_PRESSURE] = {"Differential Pressure", 2},
  [UAS_TAG_PLATFORM_ANGLE_OF_ATTACK] = {"Platform Angle of Attack", 2},
  [UAS_TAG_PLATFORM_VERTICAL_SPEED] = {"Platform Vertical Speed", 2},
  [UAS_TAG_PLATFORM_SIDESLIP_ANGLE] = {"Platform Sideslip Angle", 2},
  [UAS_TAG_AIRFIELD_BAR_PRESSURE] = {"Airfield Barometric Pressure", 2},
  [UAS_TAG_AIRFIELD_ELEVATION] = {"Airfield Elevation", 2},
  [UAS_TAG_RELATIVE_HUMIDITY] = {"Relative Humidity", 1},
  [UAS_TAG_PLATOFORM_GROUND_SPEED] = {"Platform Ground Speed", 1},
  [UAS_TAG_GROUND_RANGE] = {"Ground Range", 4},
  [UAS_TAG_PLATOFORM_FUEL_REMAINING] = {"Platform Fuel Remaining", 2},
  [UAS_TAG_PLATFORM_CALL_SIGN] = {"Platform Call Sign", 0},
  [UAS_TAG_WEAPON_LOAD] = {"Weapon Load", 2},
  [UAS_TAG_WEAPON_FIRED] = {"Weapon Fired", 1},
  [UAS_TAG_LASER_PRF_CODE] = {"Laser PRF Code", 2},
  [UAS_TAG_SENS_FIELD_OF_VIEW_NAME] = {"Sensor Field of View Name", 1},
  [UAS_TAG_PLATFORM_MAGN_HEADING] = {"Platform Magnetic Heading", 2},
  [UAS_TAG_UAS_DL_LS_VERSION_NUMBER] = {"UAS DataLink LS Version Number", 1},
  [UAS_TAG_DEPRECATED1] = {"Deprecated", -1},
  [UAS_TAG_ALTERNATE_PLAT_LATITUDE] = {"Alternate Platform Latitude", 4},
  [UAS_TAG_ALTERNATE_PLAT_LONGITUDE] = {"Alternate Platform Longitude", 4},
  [UAS_TAG_ALTERNATE_PLAT_ALTITUDE] = {"Alternate Platform Altitude", 2},
  [UAS_TAG_ALTERNATE_PLAT_NAME] = {"Alternate Platform Name", 0},
  [UAS_TAG_ALTERNATE_PLAT_HEADING] = {"Alternate Platform Heading", 2},
  [UAS_TAG_EVENT_START_TIME_UTC] = {"Event Start Time - UTC", 8},
  [UAS_TAG_RVT_LOCAL_SET] = {"RVT Local Set", 0},
  [UAS_TAG_VMTI_LOCAL_SET] = {"VMTI Local Set", 0},
  [UAS_TAG_SENSOR_ELLIPSOID_SET] = {"Sensor Ellipsoid Height", 2},
  [UAS_TAG_ALT_PLAT_ELLIPSOID_HEIGHT] = {"Alternate Plat Ellipsoid Height", 2},
  [UAS_TAG_OPERATIONAL_MODE] = {"Operational Mode", 1},
  [UAS_TAG_FRAME_CENT_HEIGHT_ABOVE_ELLIPSOID] =
      {"Frame Center Height Above Ellipsoid", 2},
  [UAS_TAG_SENSOR_NORTH_VELOCITY] = {"Sensor North Velocity", 2},
  [UAS_TAG_SENSOR_EAST_VELOCITY] = {"Sensor East Velocity", 2},
  [UAS_TAG_IM_HOR_PIX_PACK] = {"Image Horizontal Pixel Pack", 0},
  [UAS_TAG_CORN_LAT_POINT_1_FULL] = {"Corner Latitude Point 1 (Full)", 4},
  [UAS_TAG_CORN_LONG_POINT_1_FULL] = {"Corner Longitude Point 1 (Full)", 4},
  [UAS_TAG_CORN_LAT_POINT_2_FULL] = {"Corner Latitude Point 2 (Full)", 4},
  [UAS_TAG_CORN_LONG_POINT_2_FULL] = {"Corner Longitude Point 2 (Full)", 4},
  [UAS_TAG_CORN_LAT_POINT_3_FULL] = {"Corner Latitude Point 3 (Full)", 4},
  [UAS_TAG_CORN_LONG_POINT_3_FULL] = {"Corner Longitude Point 3 (Full)", 4},
  [UAS_TAG_CORN_LAT_POINT_4_FULL] = {"Corner Latitude Point 4 (Full)", 4},
  [UAS_TAG_CORN_LONG_POINT_4_FULL] = {"Corner Longitude Point 4 (Full)", 4},
  [UAS_TAG_PLATFORM_PITCH_ANGLE_FULL] = {"Platform Pitch Angle (Full)", 4},
  [UAS_TAG_PLATFORM_ROLL_ANGLE_FULL] = {"Platform Roll Angle (Full)", 4},
  [UAS_TAG_PLAT_ANGLE_OF_ATTACK_FULL] = {"Platform Angle of Attack (Full)", 4},
  [UAS_TAG_PLAT_SIDESLIP_ANGLE_FULL] = {"Platform Sideslip Angle (Full)", 4},
  [UAS_TAG_MIIS_CODE_IDENTIFIER] = {"MIIS Core Identifier", 0},
  [UAS_TAG_SAR_MOTION_IMAG_LOCAL_SET] = {"SAR Motion Imagery Local Set", 0},
  [UAS_TAG_TARGET_WIDTH_EXTENDED] = {"Target Width Extended", 0},
  [UAS_TAG_RANGE_IMAGE_LOCAL_SET] = {"Range Image Local Set", 0},
  [UAS_TAG_GEO_REGISTRATION_LOCAL_SET] = {"Geo-Registration Local Set", 0},
  [UAS_TAG_COMPOSITE_IMAGING_LOCAL_SET] = {"Composite Image Local Set", 0},
  [UAS_TAG_SEGMENT_LOCAL_SET] = {"Segment Local Set", 0},
  [UAS_TAG_AMEND_LOCAL_SET] = {"Amend Local Set", 0},
  [UAS_TAG_SDCC_FLP] = {"SDCC-FLP", 0},
  [UAS_TAG_DENSITY_ALTITUDE_EXTENDED] = {"Density Altitude Extended", 0},
  [UAS_TAG_SENS_ELLIPSOID_HEIGHT_EXT] = {"Ellipsoid Height Extended", 0},
  [UAS_TAG_ALT_PLAT_ELLIP_HEIGHT_EXT] =
      {"Alternate Plat Ellipsoid Height Extended", 0},
  [UAS_TAG_STREAM_DESIGNATOR] = {"Stream Designator", 0},
  [UAS_TAG_OPERATIONAL_BASE] = {"Operational Base", 0},
  [UAS_TAG_BROADCAST_SOURCE] = {"Broadcast Source", 0},
  [UAS_TAG_RANGE_TO_RECOVERY_LOCATION] = {"Range To Recovery Location", 0},
  [UAS_TAG_TIME_AIRBORNE] = {"Time Airborne", 0},
  [UAS_TAG_PROPULSION_UNIT_SPEED] = {"Propulsion Unit Speed", 0},
  [UAS_TAG_PLATFORM_COURSE_ANGLE] = {"Platform Course Angle", 0},
  [UAS_TAG_ALTITUDE_AGL] = {"Altitude AGL", 0},
  [UAS_TAG_RADAR_ALTIMETER] = {"Radar Altimeter", 0},
  [UAS_TAG_CONTROL_COMMAND] = {"Control Command", 0},
  [UAS_TAG_CONTROL_COMMAND_VER_LIST] = {"Control Command Verification List", 0},
  [UAS_TAG_SENSOR_AZIMUTH_RATE] = {"Sensor Azimuth Rate", 0},
  [UAS_TAG_SENSOR_ELEVATION_RATE] = {"Sensor Elevation rate", 0},
  [UAS_TAG_SENSOR_ROLL_RATE] = {"Sensor Roll Rate", 0},
  [UAS_TAG_ON_BOARD_MI_STOR_PERC_FULL] =
      {"On-board MI Storage Percent Full", 0},
  [UAS_TAG_ACTIVE_WAVELENGTH_LIST] = {"Active Wavelength List", 0},
  [UAS_TAG_COUNTRY_CODES] = {"Country Codes", 0},
  [UAS_TAG_NUMBER_NAVSATS_IN_VIEW] = {"Number of NAVSATs in View", 1},
  [UAS_TAG_POSITIONING_METHOD_SOURCE] = {"Positioning Method Source", 1},
  [UAS_TAG_PLATFORM_STATUS] = {"Platform Status", 1},
  [UAS_TAG_SENSOR_CONTROL_MODE] = {"Sensor Control", 1},
  [UAS_TAG_SENSOR_FRAMERATE_PACK] = {"Sensor Frame Rate Pack", 0},
  [UAS_TAG_EAVELENGTHS_LIST] = {"Wavelength List", 0},
  [UAS_TAG_TARGET_ID] = {"Target ID", 0},
  [UAS_TAG_AIRBASE_LOCATIONS] = {"Airbase Location", 0},
  [UAS_TAG_TAKE_OFF_TIME] = {"Take-off Time", 0},
  [UAS_TAG_TRANSMISSION_FREQUENCY] = {"Transmission Frequency", 0},
  [UAS_TAG_ON_BOARD_MI_STOR_CAPACITY] = {"On-board MI Storage Capacity", 0},
  [UAS_TAG_ZOOM_PERSENTAGE] = {"Zoom Percentage", 0},
  [UAS_TAG_COMMUNICATIONS_METHOD] = {"Communitcation Method", 0},
  [UAS_TAG_LEAP_SECONDS] = {"Leap Seconds", 0},
  [UAS_TAG_CORRECTION_OFFSET] = {"Correction Offset", 0},
  [UAS_TAG_PAYLOAD_LIST] = {"Payload List", 0},
  [UAS_TAG_ACTIVE_PAYLOADS] = {"Active Payloads", 0},
  [UAS_TAG_WEAPONS_STORES] = {"Weapons Stores", 0},
  [UAS_TAG_WAYPOINT_LIST] = {"Waypoint List", 0}
};

#define UAS_NUM_TAGS ((sizeof(uas_tags) / sizeof(TagStruct)) - 1)

static MisbReturn
st0601_is_valid_timestamp (Lds * lds)
{
  g_return_val_if_fail (lds != NULL, MISB_RET_INVALID_ARGUMENT);

  if (lds_tag (lds) != UAS_TAG_TIMESTAMP)
    return MISB_RET_NO_TIMESTAMP;
  if (lds_length (lds) != 0x08)
    return MISB_RET_NO_TIMESTAMP;
  if (lds_value (lds) == NULL)
    return MISB_RET_NO_TIMESTAMP;

  return MISB_RET_OK;
}

static MisbReturn
st0601_is_valid_checksum (Klv * klv, Lds * lds)
{
  guint16 checksum;
  guint16 lds_checksum;
  guchar *value;

  g_return_val_if_fail (lds != NULL, MISB_RET_NO_CHECKSUM);
  g_return_val_if_fail (klv != NULL, MISB_RET_NO_CHECKSUM);

  if (lds_tag (lds) != UAS_TAG_CHECKSUM)
    return MISB_RET_NO_CHECKSUM;
  if (lds_length (lds) != 0x02)
    return MISB_RET_NO_CHECKSUM;
  if (lds_value (lds) == NULL)
    return MISB_RET_NO_CHECKSUM;

  checksum = klv_utils_calculate_checksum (klv_raw_data (klv),
      klv_packet_length (klv) - 2);
  value = lds_value (lds);
  lds_checksum = value[1] | (value[0] << 8);

  if (checksum != lds_checksum)
    return MISB_RET_WRONG_CHECKSUM;

  return MISB_RET_OK;
}

MisbReturn
st0601_validate_key (Key key)
{
  if (memcmp (&key, &uas_lds_key, sizeof (uas_lds_key)) != 0) {
    GST_DEBUG ("Provided key is not UAS key");
    return MISB_RET_WRONG_KEY;
  }
  return MISB_RET_OK;
}

static MisbReturn
st0601_try_fix (MisbReturn flag, Lds ** lds, Klv ** new_klv)
{
  guint64 timestamp;

  g_return_val_if_fail (lds != NULL, MISB_RET_INVALID_ARGUMENT);
  g_return_val_if_fail (new_klv != NULL, MISB_RET_INVALID_ARGUMENT);

  if ((flag == MISB_RET_NO_CHECKSUM) || (flag == MISB_RET_WRONG_CHECKSUM)) {
    GST_WARNING ("Invalid checksum");
    return flag;
  }

  if (flag == MISB_RET_NO_TIMESTAMP) {
    /*Lets just add a timestamp */

    GST_LOG ("No timestamp present, adding one");

    timestamp = klv_utils_get_current_timestamp ();     /*Timestamp in microseconds */
    GST_LOG ("Adding current timestamp: %s",
        klv_utils_timestamp_to_string (timestamp));
    lds_add_first (lds, UAS_TAG_TIMESTAMP, 8, (guchar *) & timestamp);

    /*Lets remove the checksum */
    lds_remove_last (*lds);

    /*And create a newone */
    lds_add_checksum (*lds, uas_lds_key);

    if (!klv_from_lds (uas_lds_key, *lds, new_klv)) {
      GST_WARNING ("Could not create new klv");
      return MISB_RET_NO_CHECKSUM;
    }

    GST_INFO ("Size of output buffer is %u bytes",
        klv_packet_length (*new_klv));
  }

  return MISB_RET_FIXED;
}

static void
st0601_dump (Klv * klv, Lds * lds)
{
  guint i, j;
  Lds *tmp_lds;
  guint tag;
  guchar *value;
  Key key;

  g_return_if_fail (klv != NULL);
  g_return_if_fail (lds != NULL);

  GST_LOG ("Showing UAS content");

  j = 0;
  tmp_lds = lds;
  klv_key (klv, &key);

  g_print ("=========================\n");

  g_print ("key:\t");
  for (i = 0; i < MISB_KEY_SIZE; i++)
    g_print ("%01X ", key.byte[i]);

  g_print ("\nLength:\t%u\n", klv_length (klv));
  g_print ("Value:\n");

  while (tmp_lds) {

    g_print ("--------\n--------\n");
    g_print ("[%u]\n", j);

    tag = lds_tag (tmp_lds);
    g_print ("Tag:\t[%u](%s)\n",
        tag,
        (tag >
            UAS_NUM_TAGS) ? uas_tags[UAS_TAG_NOTAG].name : uas_tags[tag].name);

    /*Length */
    g_print ("Length:\t%u\n", lds_length (tmp_lds));

    /*Print value */
    value = lds_value (tmp_lds);
    g_print ("Value:");
    for (i = 0; i < lds_length (tmp_lds); i++) {
      if (i % 16 == 0)
        g_print ("\n\t");

      g_print ("%02X ", value[i]);
    }
    g_print ("\n");

    tmp_lds = lds_next (tmp_lds);
    j++;
  }
}

MisbReturn
st0601_validate_data (gpointer data, gpointer * new_data, gboolean dump)
{
  Lds *tmp_lds, *lds;
  Klv *klv;
  MisbReturn ret;

  g_return_val_if_fail (data != NULL, MISB_RET_INVALID_ARGUMENT);

  klv = (Klv *) data;
  lds = NULL;
  tmp_lds = NULL;
  ret = MISB_RET_OK;

  if (!lds_get (klv_value (klv), klv_length (klv), &lds)) {
    GST_WARNING ("Could not get LDS");
    return MISB_RET_NO_LDS;
  }

  /*Last element has to be the checksum */
  tmp_lds = lds_last (lds);
  ret = st0601_is_valid_checksum (klv, tmp_lds);

  if (ret != MISB_RET_OK)
    goto release_resources;

  /*First element has to be the timestamp */
  tmp_lds = lds_first (lds);
  ret = st0601_is_valid_timestamp (tmp_lds);

  if (ret == MISB_RET_OK) {
    if (dump)
      st0601_dump (klv, lds);
    goto release_resources;
  }

  if (!new_data)
    goto release_resources;

  ret = st0601_try_fix (ret, &lds, (Klv **) new_data);
  if (ret == MISB_RET_FIXED && dump)
    st0601_dump ((Klv *) (*new_data), lds);

release_resources:
  lds_release (lds);
  return ret;
}

void
st0601_get (MisbStd * std)
{
  g_return_if_fail (std != NULL);

  std->validate_key = st0601_validate_key;
  std->validate_data = st0601_validate_data;
}
