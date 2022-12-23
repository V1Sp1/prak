#include "exec.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <math.h>

#include "error.h"
#include "intcmds.h"

void null_bckgrnd(bckgrnd *bc)
{
    *bc = NULL;
}

void add_to_bckgrnd(bckgrnd *bc, int pid)
{
    bckgrnd tmp = (bckgrnd)malloc(sizeof(struct backlist));
    tmp->pid = pid;
    tmp->next = NULL;
    if(*bc == NULL){
        *bc = tmp;
        return;
    }
    while((*bc)->next != NULL){
        bc = &((*bc)->next);
    }
    (*bc)->next = tmp;
}

void clear_bckgrndlist(bckgrnd *bc)
{
    bckgrnd tmp;
    for(tmp = *bc; tmp != NULL; tmp = *bc){
        *bc = tmp->next;
        free(tmp);
    }
}

void print_bckgrnd(bckgrnd bc)
{
    for(; bc != NULL; bc = bc->next){
        printf("pid: %d\n", bc->pid);
    }
    fflush(stdout);
}

int rmelem_bckgrnd(bckgrnd *bc, int pid)
{
    bckgrnd tmp;
    if(*bc == NULL){
        return 0;
    }
    if((*bc)->next == NULL){
        if((*bc)->pid == pid){
            free(*bc);
            *bc = NULL;
            return 1;
        }
    }
    for(; (*bc)->next != NULL; bc = &((*bc)->next)){
        if((*bc)->next->pid == pid){
            tmp = (*bc)->next;
            (*bc)->next = tmp->next;
            free(tmp);
            return 1;
        }
    }
    return 0;
}

int clear_bck_zombie(bckgrnd *bc, struct darr *bckgrndend)
{
    int pid, tmp;
    char *buf;
    do {
        pid = waitpid(-1, NULL, WNOHANG);
        if(rmelem_bckgrnd(bc, pid)){
            tmp = floor(log10(pid)) + 1 + strlen(" finished") + 1;
            buf = malloc(sizeof(char) * tmp);
            sprintf(buf, "%d finished", pid);
            add_to_darr(bckgrndend, buf);
        } else {
            return 0;
        }
    } while(pid > 0);
    return 1;
}

void redirect(tree tr, int prev_type, int pfd[2], int fd)
{
    if(tr->type == PIPE){
        if(fd != -1){             /*not first in conv*/
            dup2(fd, 0);
            close(fd);
        }
        dup2(pfd[1], 1);
        close(pfd[0]);
        close(pfd[1]);
    } else if(prev_type == PIPE){
        dup2(fd, 0);        /*end of conv*/
        close(fd);
    }
    if(tr->infile != NULL){
        fd = open(tr->infile, O_RDONLY);
        if(fd == -1){
            perror(tr->infile);
            fflush(stderr);
            exit(1);
        }
        dup2(fd, 0);
        close(fd);
    }
    if(tr->outfile != NULL){
        if(tr->append){
            fd = open(tr->outfile, O_CREAT | O_APPEND | O_WRONLY, PERMS);
        } else {
            fd = creat(tr->outfile, PERMS);
        }
        if(fd == -1){
            perror(tr->outfile);
            fflush(stderr);
            exit(1);
        }
        dup2(fd, 1);
        close(fd);
    }
}

int exec_tree(tree tr, struct darr li, bckgrnd *bc, struct darr *bckgrndend)
{
    int fd = -1;
    int pfd[2];
    int bckgrndshell = 0;
    int prev_backgrnd = 0;
    int i, pid, status, tmp;
    enum type_of_next prev_type = NXT;
    int prev_correct_exit = 1;
    int need_prev_res = 0;
    int conv_len = 1;
    char *buf;
    static char *n_intcmds[] = {"exit", "cd", "help", "print_lex", "print_tree", "jobs"};
    enum {SHEXIT, SHCD, SHHELP, SHPRINT_LEX, SHPRINT_TREE, SHJOBS, USSUAL} intcmd;
    if((bckgrndend != NULL) && (bckgrndend->type != darr_of_pchar) && (li.type != darr_of_pchar)){
        return -1;
    }
    if((tr != NULL) && (tr->argv != NULL)){
        for(intcmd = SHEXIT; intcmd < USSUAL; ++intcmd){
            if(!strcmp(tr->argv[0], n_intcmds[intcmd])){
                break;
            }
        }
        switch(intcmd){
            case SHEXIT:
                if(tr->next == NULL){
                    return -1;
                }
                break;
            case SHCD:
                if(tr->next == NULL){
                    intrn_cd(tr->argv);
                }
                return 1;
            case SHHELP:
                intrn_help();
                return 1;
            case SHPRINT_LEX:
                intrn_print_lex(li);
                return 1;
            case SHPRINT_TREE:
                intrn_print_tree(tr); 
                return 1;
            case SHJOBS:
                intrn_jobs(*bc);
                return 1;
            case USSUAL:
                break;
            default:
                return -1;

        }
    }
    for(; tr != NULL; prev_type = tr->type, prev_backgrnd = tr->backgrnd, tr = tr->next){
        if(bckgrndshell){
            if(!tr->backgrnd){
                return -1;
            }
        } else {
            if(tr->backgrnd){
                clear_bck_zombie(bc, bckgrndend);
                if(prev_backgrnd && (prev_type != NXT)){
                    continue;  /*bckgrndsheel exec commands*/
                }
                if((tr->next != NULL) && (tr->type != NXT) && (tr->next->backgrnd)){
                    pid = fork();
                    if(pid == -1){
                        //error = -1, analyz errno
                        return -1;
                    }
                    if(pid > 0){
                        add_to_bckgrnd(bc, pid);
                        continue;
                    }
                    bckgrndshell = 1;
                }
            }
        }
        if(need_prev_res){
            if((tr->type != AND) && (tr->type != OR)){
                need_prev_res = 0;
            }
            if((prev_type == AND) && !prev_correct_exit){
                continue;
            }
            if((prev_type == OR) && prev_correct_exit){
                continue;
            }
        }
        if(tr->type == PIPE){
            tmp = pipe(pfd);
            if(tmp != 0){
                //error = -1, analyz errno
                return -1;
            }
            ++conv_len;
        }
        pid = fork();
        if(pid == -1){
            //error = -1, analyz errno
            return -1;
        }
        if(pid == 0){
            redirect(tr, prev_type, pfd, fd);
            if(tr->argv != NULL){
                execvp(tr->argv[0], tr->argv);
                perror(tr->argv[0]);
                fflush(stderr);
                return -1;
            } else {
                exec_tree(tr->psubcmd, li, bc, NULL);
                /*TODO: doing smth*/
                return -1;
            }
        }
        if((tr->type != PIPE) && (prev_type == PIPE)){ /*last in conv*/
            close(fd);
            fd = -1;
        }
        switch(tr->type){
            case NXT:
                if(!tr->backgrnd || bckgrndshell){
                    for(i = 0; i < conv_len; ){
                        pid = wait(NULL);
                        if(rmelem_bckgrnd(bc, pid)){
                            tmp = floor(log10(pid)) + 1 + strlen(" finished") + 1;
                            buf = malloc(sizeof(char) * tmp);
                            sprintf(buf, "%d finished", pid);
                            add_to_darr(bckgrndend, buf);
                            continue; 
                        }
                        ++i;
                    }
                } else {
                    /*if usual command*/
                    add_to_bckgrnd(bc, pid);
                }
                need_prev_res = 0;
                conv_len = 1;
                break;
            case AND:
            case OR:
                for(i = 0; i < conv_len; ){
                    pid = wait(&status);
                    if(rmelem_bckgrnd(bc, pid)){
                        tmp = floor(log10(pid)) + 1 + strlen(" finished") + 1;
                        buf = malloc(sizeof(char) * tmp);
                        sprintf(buf, "%d finished", pid);
                        add_to_darr(bckgrndend, buf);
                        continue; 
                    }
                    ++i;
                }
                need_prev_res = 1;
                conv_len = 1;
                if(WIFEXITED(status) && (WEXITSTATUS(status) == 0)){
                    prev_correct_exit = 1;
                } else {
                    prev_correct_exit = 0;
                }
                break;
            case PIPE:
                if(fd != -1){ /*not first in conv*/
                    close(fd);
                }
                close(pfd[1]);
                fd = pfd[0];
                need_prev_res = 0;
                break;
            default:
                return -1;
        }
    }
    if(bckgrndshell){
        return -1;
    }
    return 1;
}
