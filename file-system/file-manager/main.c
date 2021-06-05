/* Won't separate modules for time saving (have ideas for splitting though) */
// Can't allocate memory to read bin directory

#include <termios.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <signal.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>
#include <locale.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "print_error.h"

#define FILE_COLOR COLOR_WHITE
#define DIR_COLOR COLOR_BLUE
#define LINK_COLOR COLOR_RED
#define EXEC_FILE_COLOR COLOR_GREEN
#define BGR_COLOR COLOR_CYAN
#define SELECTED_FILE_COLOR COLOR_BLACK
#define SELECTED_BGR_COLOR COLOR_GREEN

#define LEFT_WIN_START_COL 1
#define RIGHT_WIN_START_COL COLS / 2 + 1


enum file_type { FILE_T, DIR_T, LINK_T };
enum color_pair_type {
  FILE_COLOR_PAIR = 1, DIR_COLOR_PAIR, LINK_COLOR_PAIR, EXEC_FILE_COLOR_PAIR, SELECTED_FILE_COLOR_PAIR
};

struct file_data {
  char f_name[NAME_MAX];
  enum file_type f_type;
  bool has_exec_perm;
  bool has_read_perm;
};

void sig_winch(int signo);
void reinit_screen();
void main_init();
void print_down_menu();
void reinit_windows();
void redraw_and_print_selection();
void print_background(WINDOW** win, int rows_count, int cols_count);
void enter_selected_dir();
void make_file_list(struct file_data** w_files_data, unsigned long* w_file_arr_size,
                    unsigned long* w_files_count, const char* w_dir_path);
enum file_type get_file_type(mode_t f_mode);
bool file_has_read_permission(mode_t f_mode);
bool file_has_exec_permission(mode_t f_mode);
void print_file_list(WINDOW* window, struct file_data* w_files_data, int w_files_count);
void move_down();
void move_up();
void switch_window();
void init_pairs();
bool is_accessable_directory(struct file_data fdata);
void check_row_number_correct();
void print_working_paths();


WINDOW* left_window;
WINDOW* right_window;
WINDOW* left_subwin;
WINDOW* right_subwin;
WINDOW* selection_win;

bool window_is_switched;
int current_row;
int current_min_row;
int current_max_row;

struct file_data* lw_files_data;
struct file_data* rw_files_data;
unsigned long lw_file_arr_size;
unsigned long lw_files_count;
unsigned long rw_file_arr_size;
unsigned long rw_files_count;
char* lw_cur_dir;
char* rw_cur_dir;

int main()
{  
  char* init_dir = malloc(PATH_MAX);
  getcwd(init_dir, PATH_MAX);
  
  left_window = NULL;
  right_window = NULL;
  left_subwin = NULL;
  right_subwin = NULL;
  selection_win = NULL;

  window_is_switched = false;
  current_row = 0;

  lw_files_data = NULL;
  rw_files_data = NULL;
  lw_file_arr_size = 0;
  lw_files_count = 0;
  rw_file_arr_size = 0;
  rw_files_count = 0;
  
  lw_cur_dir = malloc(PATH_MAX);
  rw_cur_dir = malloc(PATH_MAX);
  strcpy(lw_cur_dir, init_dir);
  strcpy(rw_cur_dir, init_dir);
  

  wchar_t key = 'q';
  
  
  main_init();
  reinit_screen();

  while ((key = getch()) != 'q') {
    
    switch (key) {
    case KEY_DOWN:
    case 's':
      move_down();
      break;
    case KEY_UP:
    case 'w':
      move_up();
      break;
    case KEY_RIGHT:
    case 'd':
    case KEY_LEFT:
    case 'a':
      switch_window();
      break;
    case KEY_ENTER:
    case 'e':
    case '\n':
      enter_selected_dir();
      break;
    default:
      continue;
    }

    reinit_screen();
  }

  delwin(selection_win);
  delwin(left_subwin);
  delwin(right_subwin);
  delwin(left_window);
  delwin(right_window);
  endwin();
  
  free(init_dir);
  free(lw_cur_dir);
  free(rw_cur_dir);
  free(lw_files_data);
  free(rw_files_data);
  
  exit(EXIT_SUCCESS);
}

void sig_winch(int signo)
{
  struct winsize size;
  ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
  resizeterm(size.ws_row, size.ws_col);
  
  reinit_screen();
  getch();
}

void main_init()
{
  setlocale(LC_CTYPE, "");
  initscr();
  signal(SIGWINCH, sig_winch);
  cbreak();
  curs_set(false);
  noecho();
  keypad(stdscr, true);

  start_color();
  refresh();
}

void reinit_screen()
{
  reinit_windows();
  
  make_file_list(&rw_files_data, &rw_file_arr_size, &rw_files_count, rw_cur_dir);
  print_file_list(right_subwin, rw_files_data, rw_files_count);
  
  make_file_list(&lw_files_data, &lw_file_arr_size, &lw_files_count, lw_cur_dir);
  print_file_list(left_subwin, lw_files_data, lw_files_count);

  check_row_number_correct();
  redraw_and_print_selection();

  print_working_paths();
  print_down_menu();
}

void reinit_windows()
{
  int start_x, start_y, width, height;


  delwin(left_subwin);
  delwin(right_subwin);
  delwin(left_window);
  delwin(right_window);

  clear();
  refresh();

  init_pairs();

  start_x = start_y = LEFT_WIN_START_COL;
  width = COLS / 2 - 2;
  height = LINES - 2;
  left_window = newwin(height, width, start_y, start_x);
  left_subwin = derwin(left_window, height - 2, width - 2, 1, 1);

  start_x = RIGHT_WIN_START_COL;
  right_window = newwin(height, width, start_y, start_x);
  right_subwin = derwin(right_window, height - 2, width - 2, 1, 1);
  
  wattron(left_window, COLOR_PAIR(FILE_COLOR_PAIR));
  wattron(right_window, COLOR_PAIR(FILE_COLOR_PAIR));

  print_background(&left_window, width, height);
  print_background(&right_window, width, height);
  
  wrefresh(left_window);
  wrefresh(right_window);
}

void print_down_menu()
{
  move(LINES - 1, 1);
  printw("← → — change column"
         "\t"
         "↑ — Up"
         "\t"
         "↓ — Down");
  move(LINES - 1, COLS - 9);
  printw("q — Exit");
  refresh();
}

void print_background(WINDOW** win, int rows_count, int cols_count)
{
  for(int i = 0; i < cols_count; i++) {
    for (int j = 0; j < rows_count; j++) {
      waddch(*win, ' ');
    }
  }
}

void redraw_and_print_selection()
{
  unsigned long selection_width = COLS / 2 - 4;
  char selected_file_name[NAME_MAX];
  unsigned long file_name_length;
  unsigned long count_to_print;
  
  if (NULL != selection_win) {
    delwin(selection_win);
  }

  if (window_is_switched) {
    selection_win = derwin(right_subwin, 1, selection_width, current_row, 0);
    strcpy(selected_file_name, rw_files_data[current_row].f_name);
  } else {
    selection_win = derwin(left_subwin, 1, selection_width, current_row, 0);
    strcpy(selected_file_name, lw_files_data[current_row].f_name);
  }

  wattron(selection_win, COLOR_PAIR(SELECTED_FILE_COLOR_PAIR));

  print_background(&selection_win, 1, selection_width);

  wmove(selection_win, 0, 0);
  file_name_length = strlen(selected_file_name);
  count_to_print = file_name_length < selection_width ? file_name_length : selection_width;
  for (unsigned int i = 0; i < count_to_print; i++) {
    waddch(selection_win, selected_file_name[i]);
  }
    
  wrefresh(selection_win);
}

void make_file_list(struct file_data** w_files_data, unsigned long* w_file_arr_size,
                    unsigned long* w_files_count, const char* w_dir_path)
{  
  if (NULL == *w_files_data) {
    *w_file_arr_size = 64;
    *w_files_data = calloc(*w_file_arr_size, sizeof(struct file_data));
  }

  struct dirent** dir_entries;
  int entries_count = scandir(w_dir_path, &dir_entries, NULL, alphasort);
  if (0 > entries_count) {
    print_error(__LINE__ - 2, __FUNCTION__, GEN_ERR, errno);
    exit(EXIT_FAILURE);
  }

  *w_files_count = 0;

  for (int i = 0; i < entries_count; i++) {
    char cur_f_name[NAME_MAX];
    strcpy(cur_f_name, dir_entries[i]->d_name);
    if (0 == strcmp(cur_f_name, ".")) {
      free(dir_entries[i]);
      continue;
    }

    if (0 == strcmp(w_dir_path, "/") && 0 == strcmp(cur_f_name, "..")) {
      free(dir_entries[i]);
      continue;
    }

    char cur_file_path[PATH_MAX];
    strcpy(cur_file_path, w_dir_path);
    strcat(cur_file_path, "/");
    strcat(cur_file_path, cur_f_name);

    if (*w_file_arr_size < *w_files_count) {
      *w_file_arr_size *= 2;
      *w_files_data = reallocarray(*w_files_data, *w_file_arr_size, sizeof(struct file_data));
    }

    struct stat cur_f_stat;
    int retval = stat(cur_file_path, &cur_f_stat);
    if (0 > retval) {
      if (ENOENT != errno) {
        print_error(__LINE__ - 2, __FUNCTION__, GEN_ERR, errno);
        fprintf(stderr, "%s\n", cur_file_path);
        exit(EXIT_FAILURE);
      } else {
        remove(cur_file_path);
        continue;
      }
    }

    strcpy((*w_files_data)[*w_files_count].f_name, cur_f_name);
    (*w_files_data)[*w_files_count].f_type = get_file_type(cur_f_stat.st_mode);
    (*w_files_data)[*w_files_count].has_read_perm = file_has_read_permission(cur_f_stat.st_mode);
    (*w_files_data)[*w_files_count].has_exec_perm = file_has_exec_permission(cur_f_stat.st_mode);
    
    (*w_files_count)++;
    free(dir_entries[i]);
  }

  
  free(dir_entries);
}


enum file_type get_file_type(mode_t f_mode)
{
  if (S_IFREG & f_mode) {
    return FILE_T;
  } else if (S_IFDIR & f_mode) {
    return DIR_T;
  } else if (S_IFLNK & f_mode) {
    return LINK_T;
  }
  
  return FILE_T;
}

bool file_has_read_permission(mode_t f_mode)
{
  return S_IRUSR & f_mode;
}

bool file_has_exec_permission(mode_t f_mode)
{
  return S_IXUSR & f_mode;
}

void print_file_list(WINDOW* window, struct file_data* w_files_data, int w_files_count)
{
  for (int i = 0; i < w_files_count; i++) {
    switch (w_files_data[i].f_type) {
    case FILE_T:
      if (!w_files_data[i].has_exec_perm) {
        wattron(window, COLOR_PAIR(FILE_COLOR_PAIR));
      } else {
        wattron(window, COLOR_PAIR(EXEC_FILE_COLOR_PAIR));
      }
      break;

    case DIR_T:
      if (w_files_data[i].has_read_perm) {
        wattron(window, COLOR_PAIR(DIR_COLOR_PAIR));
      } else {
        wattron(window, COLOR_PAIR(FILE_COLOR_PAIR));
      }
      break;

    case LINK_T:
      wattron(window, COLOR_PAIR(LINK_COLOR_PAIR));
      break;

    default:
      wattron(window, COLOR_PAIR(FILE_COLOR_PAIR));
      break;
    }

    wmove(window, i, 0);
    unsigned long width = COLS / 2 - 4; // COLS cannot be less than 14 in my case
    // If file name length more than we have space, then we print what fits
    if (width >= strlen(w_files_data[i].f_name)) {
      wprintw(window, "%s", w_files_data[i].f_name);
    } else {
      for (unsigned long j = 0; j < width; j++) {
        waddch(window, w_files_data[i].f_name[j]);
      }
    }
  }

  wrefresh(window);
}

void move_down()
{
  int max_row = window_is_switched ? rw_files_count - 1 : lw_files_count - 1;

  if (current_row < max_row) {
    current_row++;
  }
}

void move_up()
{
  if (current_row > 0) {
    current_row--;
  }
}

void switch_window()
{
  window_is_switched = !window_is_switched;
  int max_row = window_is_switched ? rw_files_count - 1 : lw_files_count - 1;
  if (current_row > max_row) {
    current_row = max_row;
  }
}

void init_pairs()
{
  init_pair(FILE_COLOR_PAIR, FILE_COLOR, BGR_COLOR);
  init_pair(DIR_COLOR_PAIR, DIR_COLOR, BGR_COLOR);
  init_pair(LINK_COLOR_PAIR, LINK_COLOR, BGR_COLOR);
  init_pair(EXEC_FILE_COLOR_PAIR, EXEC_FILE_COLOR, BGR_COLOR);
  init_pair(SELECTED_FILE_COLOR_PAIR, SELECTED_FILE_COLOR, SELECTED_BGR_COLOR);
}

void enter_selected_dir()
{
  char selected_dir_name[NAME_MAX];
  char new_dir_buf[PATH_MAX];
  char* w_cur_dir;

  if (window_is_switched) {
    if (!is_accessable_directory(rw_files_data[current_row])) {
      return;
    }
    
    strcpy(selected_dir_name, rw_files_data[current_row].f_name);
    w_cur_dir = rw_cur_dir;
  } else {
    if (!is_accessable_directory(lw_files_data[current_row])) {
      return;
    }
    
    strcpy(selected_dir_name, lw_files_data[current_row].f_name);
    w_cur_dir = lw_cur_dir;
  }

  strcpy(new_dir_buf, w_cur_dir);
  int dn_length = strlen(new_dir_buf) - 1;
  if (0 == strcmp(selected_dir_name, "..")) {   
    while ('/' != new_dir_buf[dn_length]) {
      new_dir_buf[dn_length] = 0;
      dn_length--;
    }
    if (0 != strcmp(new_dir_buf, "/")) {
      new_dir_buf[dn_length] = 0;
    }
  } else {
    if ('/' != new_dir_buf[dn_length]){
      strcat(new_dir_buf, "/");
    }
    strcat(new_dir_buf, selected_dir_name);
  }

  if (0 == access(new_dir_buf, R_OK | X_OK)) {
    strcpy(w_cur_dir, new_dir_buf);
  }
}

bool is_accessable_directory(struct file_data fdata)
{
  return DIR_T == fdata.f_type && fdata.has_read_perm && fdata.has_exec_perm;
}

void check_row_number_correct()
{
  int max_row = window_is_switched ? rw_files_count - 1 : lw_files_count - 1;

  if (current_row >= max_row) {
    current_row = max_row;
  }
}

void print_working_paths()
{
  move(0, LEFT_WIN_START_COL);
  printw("%s", lw_cur_dir);
  move(0, RIGHT_WIN_START_COL);
  printw("%s", rw_cur_dir);
  refresh();
}
