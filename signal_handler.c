#include "shell.h"

// Mask signal
void mask(int sig)
{
    sigset_t new_set;
    sigaddset(&new_set, sig);
    if (sigprocmask(SIG_BLOCK, &new_set, NULL) < 0) {
        perror("sigprocmask");
        exit(1);
    }
}

// Unmask signal
void unmask(int sig)
{
    sigset_t new_set;
    sigaddset(&new_set, sig);
    if (sigprocmask(SIG_UNBLOCK, &new_set, NULL) < 0) {
        perror("sigprocmask");
        exit(1);
    }
}

/* Signal handler for SIGINT (Ctrl-C) */
// Ignore Ctrl-C on shell & Interrupt fg process
void sigint_handler(int sig)
{
    int status;

    char *msg = "\nCtrl-C pressed!!\n";
    write(STDOUT_FILENO, msg, strlen(msg));

    if (getpid() == shell_pid) {
        #ifdef DEBUG
            write(STDERR_FILENO, "waiting...", strlen("waiting..."));
        #endif

        wait(NULL);
        //TODO: Are there need to wait all?
        // Maybe NO, because fg is always only one
        // while(waitpid(-1, NULL, WNOHANG) > 0){} // this will make zombie

        #ifdef DEBUG
            write(STDERR_FILENO, "s: killed child", strlen("s: killed child"));
        #endif // DEBUG
    }
    else if (getpid() != shell_pid) {
        //TODO: Why is this unreachable?
        //TODO: Why no need to make handler kill fg process?
        // Maybe because child process doesn't have changed signal handler, but why?

        #ifdef DEBUG
            write(STDERR_FILENO, "c: killing itself", strlen("c: killing itself"));
        #endif // DEBUG

        kill(getpid(), SIGKILL);
    }
}

// Change signal handler for SIGINT
void change_sigint_handler(void)
{
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = sigint_handler;
    if (sigaction(SIGINT, &act, NULL) < 0) {
        perror("sigaction");
        exit(1);
    }
}

/* Signal handler for SIGTSTP (Ctrl-Z) */
// Ignore Ctrl-Z on shell & Stop fg process
void sigtstp_handler(int sig)
{
    char *msg = "\nCtrl-Z pressed!!\n";
    write(STDOUT_FILENO, msg, strlen(msg));

    //TODO: Why no need to make handler stop fg process?

    add_job(child_pid, "CMD", "Stopped");
}

// Change signal handler for SIGTSTP
void change_sigtstp_handler(void)
{
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = sigtstp_handler;
    if (sigaction(SIGTSTP, &act, NULL) < 0) {
        perror("sigaction");
        exit(1);
    }
}
