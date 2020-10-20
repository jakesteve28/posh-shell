#ifndef SHELL_HEADER
    #define SHELL_HEADER
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <string.h>
    #include <ctype.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <fcntl.h>

    #define READ  0
    #define WRITE 1

    char* args[512];
    int command_pipe[2];
    char line[1024];
    int n = 0; 

    bool redirect_out = false;
    bool redirect_out_append = false;
    bool redirect_in = false;
    char filename_out[1024];
    char filename_in[1024];

    void split(char* cmd);
    int run(char* cmd, int input, bool first, bool last);
    int command(int input, bool first, bool last);
    void cleanup(int n);
    char* skipwhite(char* s);
    void io_out_redirect();
    void io_in_redirect();
    bool eval_io_redirect_syntax();
    bool check_redirect_out();
    bool check_redirect_in();
    void strip_redirects();
    void check_redirects(char * cmd);
    void setup_redirect(char * cmd, int pos, int direction);
    void clear_ws(char * cmd);
    void clear_ntc(char * cmd, int len);
    int get_substring_pos(const char * substr, const char * str);

#endif