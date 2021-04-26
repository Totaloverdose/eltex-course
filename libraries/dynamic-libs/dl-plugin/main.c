#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

#include "plugin_interface.h"


#define LIB_DIR_PATH "./lib"
#define ARRAY_INIT_SIZE 64



struct plugin_func_info {
  void (*p_func_ptr) (long, long);
  char p_func_name[FNAME_MAX_LENGTH];
  char p_func_desc[FDESC_MAX_LENGTH];
};

enum error_type { CRIT_ERR, MEM_ALLOC_ERR, DL_ERR };



void print_menu(struct plugin_func_info* info_array, int f_count);
void command_exit();
void command_not_exist();
void get_two_numbers_for(long* first_num, long* second_num, char* operation_name);
void print_error(int line_num, const char* func_name, enum error_type type, int err_code);
bool has_lib_extension(const char* file_name);
// Returns pointer to the first plugin function if exists
// Pointer to the first dynamic library handle into p_handles_array_ptr
struct plugin_func_info* link_plugins(void*** p_handles_array_ptr,
                                      int* funcs_count_ptr, int* handles_count_ptr);
bool is_valid_library(mode_t file_mode, char* file_name);
bool double_arrays_size(void*** handles_array_ptr,
                        struct plugin_func_info** info_array_ptr,
                        int* array_size_ptr);
void make_info_struct_name(char* dest, int i);
void make_lib_path_name(char* dest, char* lib_name);
void free_memory(void** handles_array, int handles_count, struct plugin_func_info* info_array);
void exec_function(struct plugin_func_info* info_array, int func_index);
long get_num_from_user();





int main()
{
  struct plugin_func_info* func_info_array = NULL;
  void** p_handles_array = NULL;
  int funcs_count = 0;
  int handles_count = 0;
  

  func_info_array = link_plugins(&p_handles_array, &funcs_count, &handles_count);


  while (true) {
    long operation_num = 0;

    
    print_menu(func_info_array, funcs_count);
    
    operation_num = get_num_from_user();
    printf("\n");
    
    if (operation_num > 0 && operation_num <= funcs_count) {
      exec_function(func_info_array, operation_num - 1);
    } else if (operation_num == funcs_count + 1) {
      printf("Bye! :)\n");
      break;
    } else {
      printf("Sorry, command with this number does not exist\n");
    }

    printf("\n");
    
    operation_num = 0;
  }
}





void print_menu(struct plugin_func_info* info_array, int f_count)
{
  printf("You can choose one of these commands:\n");

  for (int i = 0; i < f_count; i++) {
    printf("%d - %s\n", i + 1, info_array[i].p_func_desc);
  }
  printf("%d - Exit from the best calculator\n", f_count + 1);
  
  printf("And your choice: ");
}



void exec_function(struct plugin_func_info* info_array, int func_index)
{
  long num1 = 0;
  long num2 = 0;
  
  get_two_numbers_for(&num1, &num2, info_array[func_index].p_func_name);
  info_array[func_index].p_func_ptr(num1, num2);
}



long get_num_from_user()
{
  char num_buf[20];
  fgets(num_buf, 20, stdin);
  return atol(num_buf);
}



void get_two_numbers_for(long* first_num, long* second_num, char* operation_name)
{
  printf("Enter the first number to %s: ", operation_name);
  *first_num = get_num_from_user();
  printf("Enter the second number to %s: ", operation_name);
  *second_num = get_num_from_user();
}



void print_error(int line_num, const char* func_name, enum error_type type, int err_code)
{
  fprintf (stderr, "Line %d, function %s: ", line_num, func_name);
  switch (type) {
    
  case CRIT_ERR:
    fprintf(stderr, "%s\n", strerror(err_code));
    exit(EXIT_FAILURE);
    break;
  case MEM_ALLOC_ERR:
    fprintf(stderr, "Failed to allocate more memory for plugins\n");
    break;
  case DL_ERR:
    fprintf(stderr, "%s\n", dlerror());
  }
}



bool has_lib_extension(const char* file_name)
{
  const int name_length = strlen(file_name);
  const char* probable_extension = &(file_name[name_length - 3]);

  return (0 == strcmp(probable_extension, ".so"));
}



// Returns pointer to the first plugin function if exists
// Pointer to the first dynamic library handle into p_handles_array_ptr
struct plugin_func_info* link_plugins(void*** plugin_handles_array_ptr,
                                      int* funcs_count_ptr, int* handles_count_ptr)
{
  void** p_handles_array = calloc(ARRAY_INIT_SIZE, sizeof(void*));
  struct plugin_func_info* func_info_array =
    calloc(ARRAY_INIT_SIZE, sizeof(struct plugin_func_info));
  int array_size = ARRAY_INIT_SIZE;
  int funcs_count = 0;
  int handles_count = 0;
  
  DIR* libdir_stream = opendir(LIB_DIR_PATH);
  if (NULL == libdir_stream) {
    print_error(__LINE__ - 2, __FUNCTION__, CRIT_ERR, errno);
  }
  
  
  while (true) {
    
    struct stat lib_stat;
    int retval = 0;    
    char lib_path[FNAME_MAX_LENGTH + 4];

    errno = 0;
    struct dirent* lib_dir_entry = readdir(libdir_stream);
    if (NULL == lib_dir_entry) {
      if (errno) {
        print_error(__LINE__ - 3, __FUNCTION__, CRIT_ERR, errno);
      } else {
        closedir(libdir_stream);
        break;
      }
    }

    make_lib_path_name(lib_path, lib_dir_entry->d_name);
    retval = stat(lib_path, &lib_stat);
    if (retval != 0) {
      print_error(__LINE__ - 2, __FUNCTION__, CRIT_ERR, errno);
    }
    
    if ( is_valid_library(lib_stat.st_mode, lib_dir_entry->d_name) ) {
      if (funcs_count == array_size || handles_count == array_size) {
        bool succeed = double_arrays_size(&p_handles_array, &func_info_array, &array_size);
        if (!succeed) {
          break;
        }
      }

      p_handles_array[handles_count] = dlopen(lib_path, RTLD_NOW);
      if (NULL == p_handles_array[handles_count]) {
        print_error(__LINE__ - 2, __FUNCTION__, DL_ERR, 0);
        continue;
      }
      handles_count++;

      for (int i = 0; ; i++) {
        char info_struct_name[1024];
        make_info_struct_name(info_struct_name, i);

        struct function_info* f_info = dlsym(p_handles_array[handles_count - 1], info_struct_name);
        if (NULL == f_info) {
          break;
        }

        func_info_array[funcs_count].p_func_ptr = dlsym(p_handles_array[handles_count - 1], f_info->f_name);
        if (NULL == func_info_array) {
          print_error(__LINE__ - 2, __FUNCTION__, DL_ERR, 0);
          continue;
        }
        strcpy(func_info_array[funcs_count].p_func_name, f_info->f_name);
        strcpy(func_info_array[funcs_count].p_func_desc, f_info->f_desc);
        funcs_count++;
      }
    }
  }


  
  
  
  *plugin_handles_array_ptr = p_handles_array;
  *funcs_count_ptr = funcs_count;
  *handles_count_ptr = handles_count;
  return func_info_array;
}



bool is_valid_library(mode_t file_mode, char* file_name)
{
  // If it is a regular file
  if (S_IFREG & file_mode) {
    // And it have read and execute permissions
    if ((S_IRUSR & file_mode) && (S_IXUSR & file_mode)) {
      if (has_lib_extension(file_name)) {
        return true;
      }
    }
  }

  return false;
}



bool double_arrays_size(void*** handles_array_ptr,
                        struct plugin_func_info** info_array_ptr,
                        int* array_size_ptr)
{
  int new_array_size = *array_size_ptr * 2;

  if (NULL != handles_array_ptr) {
    void* res_ptr = reallocarray(*handles_array_ptr, new_array_size, sizeof(void**));
    if (NULL == res_ptr) {
      print_error(__LINE__ - 2, __FUNCTION__, MEM_ALLOC_ERR, 0);
      return false;
    }

    *handles_array_ptr = res_ptr;
  }

  if (NULL != info_array_ptr) {
    void* res_ptr =
      reallocarray(*info_array_ptr, new_array_size, sizeof(struct plugin_func_info));
    if (NULL == res_ptr) {
      print_error(__LINE__ - 2, __FUNCTION__, MEM_ALLOC_ERR, 0);              
      return false;
    }

    *info_array_ptr = res_ptr;
  }

  *array_size_ptr = new_array_size;
  return true;
}



void make_info_struct_name(char* dest, int i)
{
  strcpy(dest, "info");
  char info_struct_num[16];
  sprintf(info_struct_num, "%d", i);
  strcat(dest, info_struct_num);
}



void make_lib_path_name(char* dest, char* lib_name)
{
  strcpy(dest, LIB_DIR_PATH);
  strcat(dest, "/");
  strcat(dest, lib_name);
}



void free_memory(void** handles_array, int handles_count, struct plugin_func_info* info_array)
{
  for (int i = 0; i < handles_count; i++) {
    int retval = dlclose(handles_array[i]);
    if (0 != retval) {
      print_error(__LINE__ - 2, __FUNCTION__, DL_ERR, 0);
    }
  }

  free(handles_array);
  free(info_array);
}
