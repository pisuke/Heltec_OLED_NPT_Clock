
/* http://worldtimeapi.org/api/ip
 * {"abbreviation":"BST",
 * "client_ip":"86.17.220.24",
 * "datetime":"2020-07-25T14:51:01.689928+01:00",
 * "day_of_week":6,
 * "day_of_year":207,
 * "dst":true,
 * "dst_from":"2020-03-29T01:00:00+00:00",
 * "dst_offset":3600,
 * "dst_until":"2020-10-25T01:00:00+00:00",
 * "raw_offset":0,
 * "timezone":"Europe/London",
 * "unixtime":1595685061,
 * "utc_datetime":"2020-07-25T13:51:01.689928+00:00",
 * "utc_offset":"+01:00",
 * "week_number":30
 * }
 */

//
//const size_t capacity = JSON_OBJECT_SIZE(15) + 350;
//DynamicJsonDocument doc(capacity);
//
//const char* json = "{\"abbreviation\":\"BST\",\"client_ip\":\"86.17.220.24\",\"datetime\":\"2020-07-25T14:51:01.689928+01:00\",\"day_of_week\":6,\"day_of_year\":207,\"dst\":true,\"dst_from\":\"2020-03-29T01:00:00+00:00\",\"dst_offset\":3600,\"dst_until\":\"2020-10-25T01:00:00+00:00\",\"raw_offset\":0,\"timezone\":\"Europe/London\",\"unixtime\":1595685061,\"utc_datetime\":\"2020-07-25T13:51:01.689928+00:00\",\"utc_offset\":\"+01:00\",\"week_number\":30}";
//
//deserializeJson(doc, json);
//
//const char* abbreviation = doc["abbreviation"]; // "BST"
//const char* client_ip = doc["client_ip"]; // "86.17.220.24"
//const char* datetime = doc["datetime"]; // "2020-07-25T14:51:01.689928+01:00"
//int day_of_week = doc["day_of_week"]; // 6
//int day_of_year = doc["day_of_year"]; // 207
//bool dst = doc["dst"]; // true
//const char* dst_from = doc["dst_from"]; // "2020-03-29T01:00:00+00:00"
//int dst_offset = doc["dst_offset"]; // 3600
//const char* dst_until = doc["dst_until"]; // "2020-10-25T01:00:00+00:00"
//int raw_offset = doc["raw_offset"]; // 0
//const char* timezone = doc["timezone"]; // "Europe/London"
//long unixtime = doc["unixtime"]; // 1595685061
//const char* utc_datetime = doc["utc_datetime"]; // "2020-07-25T13:51:01.689928+00:00"
//const char* utc_offset = doc["utc_offset"]; // "+01:00"
//int week_number = doc["week_number"]; // 30
 
