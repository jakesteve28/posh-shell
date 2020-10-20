#include "shell.h"
#include "poshrc/poshrc.c"

void printnode(void * n){
    printf("%s", (char*)n);
}

int command(int input, bool first, bool last) {
    pid_t pid;
    int default_out = STDOUT_FILENO;
    int default_in = STDIN_FILENO;
	int fd[2];
 	int res = pipe(fd);	
    if(res == -1){
        perror("pipe error\n");
        exit(EXIT_FAILURE);
    }
	if (0 == (pid = fork())) {
		if (first && !last && input == 0) {
            if(redirect_in){
                int fin = open(filename_in, O_RDONLY, 0);
                dup2(fin, STDIN_FILENO);
                close(fin);
            }
			dup2( fd[WRITE], default_out);
		} else if (!first && !last && input != 0) {
			dup2(input, default_in);
			dup2(fd[WRITE], default_out);
		} else {
            if(redirect_in){
                int fin = open(filename_in, O_RDONLY, 0);
                dup2(fin, STDIN_FILENO);
                close(fin);
            }
            if(redirect_out){
                int fout = open(filename_out, O_CREAT | O_TRUNC | O_WRONLY, 0777);
                dup2(fout, STDOUT_FILENO);
                close(fout);
            } else if(redirect_out_append){
                int fout = open(filename_out, O_CREAT | O_APPEND | O_WRONLY, 0777);
                dup2(fout, STDOUT_FILENO);
                close(fout);
            }
			dup2( input, default_in );
		}
		if (execvp( args[0], args) == -1)
            perror("execvp error\n");
			exit(EXIT_FAILURE); 
	} 
    if(pid < 0) {
        perror("fork error\n");
        exit(EXIT_FAILURE); 
    }
	if (input != 0) 
		close(input);
	close(fd[WRITE]);
	if (last)
		close(fd[READ]);
	return fd[READ];
}

void wait_all(int n) {
	int i;
	for (i = 0; i < n; ++i) 
		wait(NULL); 
}

int main() {
    FILE * historyfile = fopen(".posh_history", "a+");
    LinkedList * history = gethistory(historyfile);
	while (true) {
		printf("posh $ ");
        redirect_out = false;
        redirect_in = false;
        filename_in[0] = '\0'; 
        filename_out[0] = '\0';
		fflush(NULL);
		if (!fgets(line, 1024, stdin)) 
			return 0;
		int input = 0;
		bool first = true;
		char * cmd = line;
        if(0 == strcmp(cmd, "history\n")){
            printList(history, printnode);
            continue;
        }
        addhistory(history, historyfile, cmd);
        check_redirects(cmd);
		char * next_pipe = strchr(cmd, '|');
		while (next_pipe != NULL) {
			*next_pipe = '\0';
			input = run(cmd, input, first, false);
			cmd = next_pipe + 1;
			next_pipe = strchr(cmd, '|'); 
			first = false;
		}
		input = run(cmd, input, first, true);
		wait_all(n);
		n = 0;
	}
	return 0;
}
 
int run(char* cmd, int input, bool first, bool last) {
	split(cmd);
	if (args[0] != NULL) {
		if (strcmp(args[0], "exit") == 0) 
			exit(0);
		n += 1;
		return command(input, first, last);
	}
	return 0;
}
 
char * skip_whitespace(char* s) {
	while (isspace(*s)) ++s;
	return s;
}
 
void strip_nl(char * s){
    int i = strlen(s), j = 0;
    while(j < i){
        if(s[j] == '\n')
            s[j] = '\0';
        j++;
    }
}

void split(char * cmd) {
	cmd = skip_whitespace(cmd);
	char * next_space = strchr(cmd, ' ');
	int i = 0;
	while(next_space != NULL) {
		next_space[0] = '\0';
		args[i] = cmd;
		++i;
		cmd = skip_whitespace(next_space + 1);
		next_space = strchr(cmd, ' ');
	}
	if (cmd[0] != '\0') {
		args[i] = cmd;
		next_space = strchr(cmd, '\n');
		next_space[0] = '\0';
		++i; 
	}
	args[i] = NULL;
}

int get_substring_pos(const char * substr, const char * str){
    char * pos = strstr(str, substr);
    if(pos == NULL) return -1; 
    return pos - str;
}

void clear_ntc(char * cmd, int len){
    int i = 0;
    while(i < len){
        if(cmd[i] == '\0')
            cmd[i] = ' ';
        i++;
    }
}

void clear_ws(char * cmd){
    char * dest = cmd;
    while(*cmd != '\0'){
        while(*cmd == ' ' && *(cmd + 1) == ' ')
            cmd++;
        *dest++ = *cmd++;
    }
    *dest = '\0';
}

void setup_redirect(char * cmd, int pos, int direction){
    //direction 0 if in, 1 if out
    if(pos == -1){
        return;
    }
    char buf[1024], fname[1024];
    int full_length = strlen(cmd);
    char * tok_redir;
    char * tok_fname;
    strcpy(buf, cmd);
    tok_redir = strtok(&buf[pos], " ");
    tok_fname = strtok(NULL, " ");
    int len_fname = strlen(tok_fname), count = 0;
    while(((tok_redir - buf) + count) < (tok_fname - buf)){
        buf[(tok_redir - buf) + count] = ' ';
        count++;
    }
    count = 0;
    while(count < (len_fname + 1)){
        fname[count] = buf[tok_fname - buf];
        buf[tok_fname - buf] = ' ';
        tok_fname++; 
        count++;
    }
    buf[full_length] = '\0';
    clear_ntc(buf, full_length);
    clear_ws(buf);
    clear_ws(fname);
    strip_nl(fname);
    if(direction == 1){
        strcpy(filename_out, fname);
    }
    else if(direction == 0){
        strcpy(filename_in, fname);
    }
    strcpy(cmd, buf);
}

void check_redirects(char * cmd){
    int out = -1, in = -1;
    out = get_substring_pos(" >> ", cmd);
    if(out != -1)
        redirect_out_append = true;
    if(out == -1){
        out = get_substring_pos(" > ", cmd);
        if(out != -1){
            redirect_out_append = false;
            redirect_out = true;
        } 
    }
    in = get_substring_pos(" < ", cmd);
    if(in != -1){
        redirect_in = true;
    } else redirect_in = false;
    if(out != -1 && in != -1){
        if(in < out){
            setup_redirect(cmd, in, 0);
            out = get_substring_pos(" >> ", cmd);
            if(out == -1){
                out = get_substring_pos(" > ", cmd);
            }
            setup_redirect(cmd, out, 1);
        } else if(in > out) { 
            setup_redirect(cmd, out, 0);
            in = get_substring_pos(" < ", cmd);
            setup_redirect(cmd, in, 1);
        } 
    } else if(in != -1){
        setup_redirect(cmd, in, 0);
    } else if(out != -1){
        setup_redirect(cmd, out, 1);
    }
}
