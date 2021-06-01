enum error_type { GEN_ERR, MEM_ALLOC_ERR, DL_ERR };

// Function gets:
// 1) The code line number, in which an error occured
// 2) The function name, in which an error occured
// 3) The error type (general, memory allocation or dynamic library error)
// 4) The error code (usually the value of errno or any number if not needed)
void print_error(int line_num, const char* func_name, enum error_type type, int err_code);
