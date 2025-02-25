
#include "STU_JSON.H"

// #include "json.h"
// #include "json.h"
// #include "json_object.h"
// #include "C:/STU/devel/ReMoM/_deps/json-c-build/json_config.h"
// C:\STU\devel\ReMoM\_deps\json-c-src
// NO-WORKIE  #include "../../_deps/json-c-src/json_object.h"

#include <stdio.h>



// json_object *generate_log_object(char *log_level, char *msg, char *timestamp){
//    // {}
//    json_object *child = json_object_new_object();
//    //  {"loglevel": ... }
//    json_object_object_add(child, "loglevel", json_object_new_string(log_level));
//    // {"loglevel":"...", "msg":"..."}
//    json_object_object_add(child, "msg", json_object_new_string(msg));
//    // {"loglevel":"...", "msg":"...", "timestamp":"..."}
//    json_object_object_add(child, "timestamp", json_object_new_string(timestamp));
//    return child
// }



// int main(void)
// {
// 
//    json_object * root = json_object_new_object();
// 
//     if (!root)
//         return 1;
// 
//     // {"module":"log"}
//     json_object_object_add(root, "module", json_object_new_string("log"));
// 
//     // {"module":"log","version":1.0 }
//     json_object_object_add(root, "version", json_object_new_string("1.0"));
// 
// 
//     // {"module":"log","version":1.0, "logs":[]}
//     json_object *logs = json_object_new_array();
// 
//     json_object_object_add(root, "logs", logs);
// 
// 
//     // {"module":"log","version":1.0, "logs":[{"loglevel":"INFO", "msg":"Info about car", "timestamp":"actual system time"}]}
//     char *log1 = generate_log_object("INFO", "Info about car", "system time");
// 
//     json_object_array_add(logs, log1);
// 
// }
