/********************************************************************
 * Autogenerated file, do not edit.
 *******************************************************************/

#include <stdint.h>
#include "sli_bt_gattdb_def.h"

#define GATT_HEADER(F) F
#define GATT_DATA(F) F
GATT_DATA(const uint16_t gattdb_uuidtable_16_map[]) =
{
  0x2800,
  0x2801,
  0x2803,
  0x2a00,
  0x2a01,
  0x2902,
};

GATT_DATA(const uint8_t gattdb_uuidtable_128_map[]) =
{
  0xc7, 0x75, 0x96, 0x38, 0xd5, 0x6b, 0x6e, 0xbc, 0xc8, 0x43, 0xa9, 0x46, 0xd2, 0x41, 0x40, 0x11, 
  0xc8, 0x75, 0x96, 0x38, 0xd5, 0x6b, 0x6e, 0xbc, 0xc8, 0x43, 0xa9, 0x46, 0xd2, 0x41, 0x40, 0x11, 
  0xc9, 0x75, 0x96, 0x38, 0xd5, 0x6b, 0x6e, 0xbc, 0xc8, 0x43, 0xa9, 0x46, 0xd2, 0x41, 0x40, 0x11, 
  0xca, 0x75, 0x96, 0x38, 0xd5, 0x6b, 0x6e, 0xbc, 0xc8, 0x43, 0xa9, 0x46, 0xd2, 0x41, 0x40, 0x11, 
};
GATT_DATA(sli_bt_gattdb_attribute_chrvalue_t gattdb_attribute_field_7) = {
  .properties = 0x08,
  .max_len = 1,
  .data = { 0x00, },
};
GATT_DATA(const sli_bt_gattdb_value_t gattdb_attribute_field_5) = {
  .len = 16,
  .data = { 0xc6, 0x75, 0x96, 0x38, 0xd5, 0x6b, 0x6e, 0xbc, 0xc8, 0x43, 0xa9, 0x46, 0xd2, 0x41, 0x40, 0x11, }
};
GATT_DATA(const sli_bt_gattdb_value_t gattdb_attribute_field_4) = {
  .len = 2,
  .data = { 0x00, 0x00, }
};
GATT_DATA(sli_bt_gattdb_attribute_chrvalue_t gattdb_attribute_field_2) = {
  .properties = 0x0a,
  .max_len = 8,
  .data = { 0x54, 0x72, 0x61, 0x63, 0x6b, 0x62, 0x6f, 0x74, },
};
GATT_DATA(const sli_bt_gattdb_value_t gattdb_attribute_field_0) = {
  .len = 2,
  .data = { 0x00, 0x18, }
};

GATT_DATA(const sli_bt_gattdb_attribute_t gattdb_attributes_map[]) = {
  { .handle = 0x01, .uuid = 0x0000, .permissions = 0x801, .caps = 0xffff, .state = 0x00, .datatype = 0x00, .constdata = &gattdb_attribute_field_0 },
  { .handle = 0x02, .uuid = 0x0002, .permissions = 0x801, .caps = 0xffff, .state = 0x00, .datatype = 0x05, .characteristic = { .properties = 0x0a, .char_uuid = 0x0003 } },
  { .handle = 0x03, .uuid = 0x0003, .permissions = 0x803, .caps = 0xffff, .state = 0x00, .datatype = 0x01, .dynamicdata = &gattdb_attribute_field_2 },
  { .handle = 0x04, .uuid = 0x0002, .permissions = 0x801, .caps = 0xffff, .state = 0x00, .datatype = 0x05, .characteristic = { .properties = 0x02, .char_uuid = 0x0004 } },
  { .handle = 0x05, .uuid = 0x0004, .permissions = 0x801, .caps = 0xffff, .state = 0x00, .datatype = 0x00, .constdata = &gattdb_attribute_field_4 },
  { .handle = 0x06, .uuid = 0x0000, .permissions = 0x801, .caps = 0xffff, .state = 0x00, .datatype = 0x00, .constdata = &gattdb_attribute_field_5 },
  { .handle = 0x07, .uuid = 0x0002, .permissions = 0x801, .caps = 0xffff, .state = 0x00, .datatype = 0x05, .characteristic = { .properties = 0x08, .char_uuid = 0x8000 } },
  { .handle = 0x08, .uuid = 0x8000, .permissions = 0x802, .caps = 0xffff, .state = 0x00, .datatype = 0x01, .dynamicdata = &gattdb_attribute_field_7 },
  { .handle = 0x09, .uuid = 0x0002, .permissions = 0x801, .caps = 0xffff, .state = 0x00, .datatype = 0x05, .characteristic = { .properties = 0x10, .char_uuid = 0x8001 } },
  { .handle = 0x0a, .uuid = 0x8001, .permissions = 0x800, .caps = 0xffff, .state = 0x00, .datatype = 0x07, .dynamicdata = NULL },
  { .handle = 0x0b, .uuid = 0x0005, .permissions = 0x803, .caps = 0xffff, .state = 0x00, .datatype = 0x03, .configdata = { .flags = 0x01, .clientconfig_index = 0x00 } },
  { .handle = 0x0c, .uuid = 0x0002, .permissions = 0x801, .caps = 0xffff, .state = 0x00, .datatype = 0x05, .characteristic = { .properties = 0x10, .char_uuid = 0x8002 } },
  { .handle = 0x0d, .uuid = 0x8002, .permissions = 0x800, .caps = 0xffff, .state = 0x00, .datatype = 0x07, .dynamicdata = NULL },
  { .handle = 0x0e, .uuid = 0x0005, .permissions = 0x803, .caps = 0xffff, .state = 0x00, .datatype = 0x03, .configdata = { .flags = 0x01, .clientconfig_index = 0x01 } },
  { .handle = 0x0f, .uuid = 0x0002, .permissions = 0x801, .caps = 0xffff, .state = 0x00, .datatype = 0x05, .characteristic = { .properties = 0x10, .char_uuid = 0x8003 } },
  { .handle = 0x10, .uuid = 0x8003, .permissions = 0x800, .caps = 0xffff, .state = 0x00, .datatype = 0x07, .dynamicdata = NULL },
  { .handle = 0x11, .uuid = 0x0005, .permissions = 0x803, .caps = 0xffff, .state = 0x00, .datatype = 0x03, .configdata = { .flags = 0x01, .clientconfig_index = 0x02 } },
};

GATT_HEADER(const sli_bt_gattdb_t gattdb) = {
  .attributes = gattdb_attributes_map,
  .attribute_table_size = 17,
  .attribute_num = 17,
  .uuid16 = gattdb_uuidtable_16_map,
  .uuid16_table_size = 6,
  .uuid16_num = 6,
  .uuid128 = gattdb_uuidtable_128_map,
  .uuid128_table_size = 4,
  .uuid128_num = 4,
  .num_ccfg = 3,
  .caps_mask = 0xffff,
  .enabled_caps = 0xffff,
};
const sli_bt_gattdb_t *static_gattdb = &gattdb;
