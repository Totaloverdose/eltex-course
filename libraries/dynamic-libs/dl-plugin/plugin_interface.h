/**
 * The best calculator plugin interface
 * Each plugin has to be in the ./lib directory relative to the app binary
 * Follow instructions below to make your plugins work
 * Just include this header file to your plugin
 * And have a nice day
 **/

#ifndef PLUGIN_INTERFACE
#define PLUGIN_INTERFACE

#define FNAME_MAX_LENGTH 256
#define FDESC_MAX_LENGTH 1024

/**
 * Add this structure's instance for each function in your plugin
 **/
// Have to be named "infoX" to work, where X is serial number from 0
// Contains the plugin's function name and description
// Doesn't have to have a return value
// Have to have two arguments type of long
struct function_info {
  char f_name[FNAME_MAX_LENGTH];
  char f_desc[FDESC_MAX_LENGTH];
};

#endif
