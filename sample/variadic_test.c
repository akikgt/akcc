typedef struct {
  int gp_offset;
  int fp_offset;
  void *overflow_arg_area;
  void *reg_save_area;
} va_list[1];

#define va_start __builtin_va_start
#define va_end __builtin_va_end

int vsprintf(char *str, const char *format, va_list ap);
// variadic test
void my_vsprintf(char *p, char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vsprintf(p, fmt, ap);
  va_end(ap);
} 

int main() {
    int e1 = ({
    char buf[128];
    my_vsprintf(buf, "test%d %d %d %d", 1, 2, 3, 4);

    printf("%s\n", buf);
    });
}