#define _XOPEN_SOURCE 700
#include <unistd.h>

#include <stdio.h>
#include <signal.h>

void email_handler(int sig)
{
    printf(" - Email notification is waiting.\n[Outcome:] The TA announced: Everyone get 100 on the exercise!\n");
}
void delivery_handler(int sig)
{
    printf(" - You have a reminder to pick up your delivery.\n[Outcome:] You picked it up just in time.\n");
}
void doorbell_handler(int sig)
{
    printf(" - The doorbell is ringing.\n[Outcome:] Food delivery is here.\n");
}

void print_menu()
{
    printf("\nSimulate a distraction:\n"
           "  1 = Email notification\n"
           "  2 = Reminder to pick up delivery\n"
           "  3 = Doorbell Ringing\n"
           "  q = Quit\n"
           ">> ");
}

void start_round(int duration)
{
    sigset_t block;
    sigemptyset(&block);
    sigaddset(&block, SIGUSR1);
    sigaddset(&block, SIGUSR2);
    sigaddset(&block, SIGALRM);
    sigprocmask(SIG_BLOCK, &block, NULL);
    for (int i = 0; i < duration; i++)
    {
        print_menu();
        char c;
        scanf(" %c", &c);
        if (c == 'q')
        {
            break;
        }
        switch (c)
        {
        case '1':
            kill(getpid(), SIGUSR1);
            break;
        case '2':
            kill(getpid(), SIGUSR2);
            break;
        case '3':
            kill(getpid(), SIGALRM);
            break;

        default:
            break;
        }
    }
    printf("──────────────────────────────────────────────\n"
           "        Checking pending distractions...      \n"
           "──────────────────────────────────────────────\n");
    sigset_t waits, mask;
    sigpending(&waits);
    int exist_waiting = 0;
    if(sigismember(&waits, SIGUSR1)){
        sigemptyset(&mask);
        sigaddset(&mask, SIGUSR1);
        sigprocmask(SIG_UNBLOCK, &mask, NULL);
        exist_waiting = 1;
    }
    if(sigismember(&waits, SIGUSR2)){
        sigemptyset(&mask);
        sigaddset(&mask, SIGUSR2);
        sigprocmask(SIG_UNBLOCK, &mask, NULL);
        exist_waiting = 1;
    }
    if(sigismember(&waits, SIGALRM)){
        sigemptyset(&mask);
        sigaddset(&mask, SIGALRM);
        sigprocmask(SIG_UNBLOCK, &mask, NULL);
        exist_waiting = 1;
    }
    if(!exist_waiting){
        printf("No distractions reached you this round.\n");
    }
}

void runFocusMode(int numOfRounds, int duration)
{
    struct sigaction sa_usr1;
    sigemptyset(&sa_usr1.sa_mask);
    sa_usr1.sa_flags = 0;
    sa_usr1.sa_handler = email_handler;
    sigaction(SIGUSR1, &sa_usr1, NULL);

    struct sigaction sa_usr2;
    sigemptyset(&sa_usr2.sa_mask);
    sa_usr2.sa_flags = 0;
    sa_usr2.sa_handler = delivery_handler;
    sigaction(SIGUSR2, &sa_usr2, NULL);

    struct sigaction sa_alrm;
    sigemptyset(&sa_alrm.sa_mask);
    sa_alrm.sa_flags = 0;
    sa_alrm.sa_handler = doorbell_handler;
    sigaction(SIGALRM, &sa_alrm, NULL);

    // signal(SIGUSR1, email_handler);
    // signal(SIGUSR2, delivery_handler);
    // signal(SIGALRM, doorbell_handler);
    printf("Entering Focus Mode. All distractions are blocked.\n");
    // run numOfRounds times
    for (int i = 0; i < numOfRounds; i++)
    {
        printf("══════════════════════════════════════════════\n");
        printf("                Focus Round %d                \n", i + 1);
        printf("──────────────────────────────────────────────\n");

        start_round(duration);

        printf("──────────────────────────────────────────────\n");
        printf("             Back to Focus Mode.              \n");
        printf("══════════════════════════════════════════════\n");
    }
    printf("\nFocus Mode complete. All distractions are now unblocked.\n");
}