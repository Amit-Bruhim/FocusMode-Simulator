#define _XOPEN_SOURCE 700
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

/* Handler for Email Notification (SIGUSR1)
   Prints the outcome of the email distraction */
void email_handler(int sig)
{
    printf(" - Email notification is waiting.\n[Outcome:] The TA announced: Everyone get 100 on the exercise!\n");
}

/* Handler for Delivery Reminder (SIGUSR2)
   Prints the outcome of the delivery distraction */
void delivery_handler(int sig)
{
    printf(" - You have a reminder to pick up your delivery.\n[Outcome:] You picked it up just in time.\n");
}

/* Handler for Doorbell Ringing (SIGALRM)
   Prints the outcome of the doorbell distraction */
void doorbell_handler(int sig)
{
    printf(" - The doorbell is ringing.\n[Outcome:] Food delivery is here.\n");
}

/* Prints the distraction menu for the user */
void print_menu()
{
    /* Highlight the "Simulate a distraction" text in green */
    printf("\033[0;32mSimulate a distraction:\033[0m\n");
    printf("  1 = Email notification\n"
           "  2 = Reminder to pick up delivery\n"
           "  3 = Doorbell Ringing\n"
           "  q = Quit\n"
           ">> ");
}

/* Runs a single focus round
   - Blocks signals to prevent interruptions
   - Handles user input for simulating distractions
   - Checks and processes any pending distractions at the end */
void start_round(int duration)
{
    /* Block all distraction signals during the focus round */
    sigset_t block;
    sigemptyset(&block);
    sigaddset(&block, SIGUSR1);
    sigaddset(&block, SIGUSR2);
    sigaddset(&block, SIGALRM);
    sigprocmask(SIG_BLOCK, &block, NULL);

    /* Loop through user input for the duration of the round */
    for (int i = 0; i < duration; i++)
    {
        print_menu();
        char c;
        scanf(" %c", &c);
        if (c == 'q') break;

        /* Send corresponding signals based on user choice */
        switch (c)
        {
        case '1': kill(getpid(), SIGUSR1); break;
        case '2': kill(getpid(), SIGUSR2); break;
        case '3': kill(getpid(), SIGALRM); break;
        default: break;
        }
    }

    /* Display pending distractions header in green */
    printf("──────────────────────────────────────────────\n"
           "        \033[0;32mChecking pending distractions...\033[0m    \n"
           "──────────────────────────────────────────────\n");

    /* Check pending signals and unblock them */
    sigset_t waits, mask;
    sigpending(&waits);
    int exist_waiting = 0;

    if (sigismember(&waits, SIGUSR1)) { sigemptyset(&mask); sigaddset(&mask, SIGUSR1); sigprocmask(SIG_UNBLOCK, &mask, NULL); exist_waiting = 1; }
    if (sigismember(&waits, SIGUSR2)) { sigemptyset(&mask); sigaddset(&mask, SIGUSR2); sigprocmask(SIG_UNBLOCK, &mask, NULL); exist_waiting = 1; }
    if (sigismember(&waits, SIGALRM)) { sigemptyset(&mask); sigaddset(&mask, SIGALRM); sigprocmask(SIG_UNBLOCK, &mask, NULL); exist_waiting = 1; }

    /* Print message if no distractions were pending (green) */
    if (!exist_waiting)
        printf("\033[0;32mNo distractions reached you this round.\033[0m\n");
}

/* Sets up signal handlers and runs the specified number of focus rounds */
void runFocusMode(int numOfRounds, int duration)
{
    /* Configure handler for Email Notification */
    struct sigaction sa_usr1;
    sigemptyset(&sa_usr1.sa_mask);
    sa_usr1.sa_flags = 0;
    sa_usr1.sa_handler = email_handler;
    sigaction(SIGUSR1, &sa_usr1, NULL);

    /* Configure handler for Delivery Reminder */
    struct sigaction sa_usr2;
    sigemptyset(&sa_usr2.sa_mask);
    sa_usr2.sa_flags = 0;
    sa_usr2.sa_handler = delivery_handler;
    sigaction(SIGUSR2, &sa_usr2, NULL);

    /* Configure handler for Doorbell Ringing */
    struct sigaction sa_alrm;
    sigemptyset(&sa_alrm.sa_mask);
    sa_alrm.sa_flags = 0;
    sa_alrm.sa_handler = doorbell_handler;
    sigaction(SIGALRM, &sa_alrm, NULL);

    /* Notify user that focus mode has started (green) */
    printf("\033[0;32mEntering Focus Mode. All distractions are blocked.\033[0m\n");

    /* Run each focus round */
    for (int i = 0; i < numOfRounds; i++)
    {
        /* Print round header with green text */
        printf("══════════════════════════════════════════════\n");
        printf("\033[0;32m                Focus Round %d                \033[0m\n", i + 1);
        printf("──────────────────────────────────────────────\n");

        /* Execute a single round */
        start_round(duration);

        /* Return message after round completion (green) */
        printf("──────────────────────────────────────────────\n");
        printf("\033[0;32m             Back to Focus Mode.              \033[0m\n");
        printf("══════════════════════════════════════════════\n");
    }

    /* Notify user that all rounds are complete (green) */
    printf("\033[0;32m\nFocus Mode complete. All distractions are now unblocked.\033[0m\n");
}

/* Program entry point */
int main(int argc, char *argv[])
{
    /* Parse number of rounds and duration from command line */
    int numOfRounds = atoi(argv[2]);
    int duration = atoi(argv[3]);

    /* Start focus mode with the specified parameters */
    runFocusMode(numOfRounds, duration);

    return 0;
}
