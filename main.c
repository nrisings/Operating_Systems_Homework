#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <string.h>
#include <wait.h>
#include <signal.h>
#include <sys/types.h>
typedef enum {
    MONDAY = 0, //7             0
    TUESDAY, //8            1
    WEDNESDAY,//10 -- 25    2
    THURSDAY,//9 -- 34      3
    FRIDAY, //7 -- 41       4
    SATURDAY, // 9          5
    SUNDAY //7              6
}
Week_days;

//Week_days week_days_arr[] = { MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY };
const char* week_days_str[7] = { "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY", "SUNDAY" };
typedef struct Farm_day
{
    const int MAX_CAPACITY;
    int applied;
}
farm_day;

typedef struct Applicant
{
    char name[20];
    int working_days[7];
}
applicant;

typedef struct node
{
    applicant data;
    struct node* next;
    struct node* prev;
}
Node;

farm_day WORKERS_CAPACITY[] = { { 9, 0 }, { 2, 0 }, { 5, 0 }, { 7, 0 }, { 10, 0 }, { 4, 0 }, { 6, 0 } };   // This array stores how many workers the farm needs on each day
const char* get_day(int i) {
    switch (i)
    {
    case 0:
        return "MONDAY";
    case 1:
        return "TUESDAY";
    case 2:
        return "WEDNESDAY";
    case 3:
        return "THURSDAY";
    case 4:
        return "FRIDAY";
    case 5:
        return "SATURDAY";
    case 6:
        return "SUNDAY";
    default:
        return "";
    }
}
Node* new_node(char* name)
{
    Node* node = (Node*)malloc(sizeof(Node));
    strncpy(node->data.name, name, sizeof(node->data.name));
    node->next = NULL;
    node->prev = NULL;
    return node;
}
Node* insert_at_tail(Node** head_ref, Node** tail_ref, char* name)
{
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL)
    {
        printf("Memory allocation error\n");
        exit(1);
    }
    strcpy(new_node->data.name, name);
    memset(new_node->data.working_days, 0, sizeof(new_node->data.working_days));
    new_node->next = NULL;
    new_node->prev = NULL;
    if (*head_ref == NULL)
    {
        *head_ref = new_node;
        *tail_ref = new_node;
    }
    else
    {
        (*tail_ref)->next = new_node;
        new_node->prev = *tail_ref;
        *tail_ref = new_node;
    }
    return new_node;
}
Node* find_tail(Node* head)
{
    if (head == NULL)
    {
        return NULL;
    }

    // Traverse the list until we reach the beginning
    Node* current = head;
    while (current->next != NULL)
    {
        current = current->next;
    }

    return current;

}
void print_node(Node* node)
{
    printf("\n Applicant name : %s \n Applicant's working days : ", node->data.name);
    for (int i = 0; i < 7; i++)
    {
        if (node->data.working_days[i] == 1)
        {
            printf("%s ", week_days_str[i]);
        }
    }

}
void print_d_linked_list(Node* head)
{

    if (head == NULL)
    {
        printf("Error: linked list is empty\n");
        return;
    }
    Node* current = head;
    while (current != NULL)
    {
        printf("\n Applicant name : %s \n Applicant's working days : ", current->data.name);
        for (int i = 0; i < 7; i++)
        {
            if (current->data.working_days[i] == 1)
            {
                printf("%s ", week_days_str[i]);
            }
        }
        printf("\n");
        current = current->next;
    }

}
Node* read_d_linked_list_from_file(FILE* file)
{
    Node* head = NULL;
    Node* tail = NULL;
    char name[20];
    int working_days[7];
    Node* current_node = NULL;

    // Loop through the file and insert a new node for each element
    while (fgets(name, sizeof(name), file) != NULL)
    {
        if (name[strlen(name) - 1] == '\n')
        {
            name[strlen(name) - 1] = '\0';
        }

        current_node = insert_at_tail(&head, &tail, name);

        char line[50];
        if (fgets(line, sizeof(line), file) != NULL)
        {
            // Parse the line to extract the applicant's availability
            int num_values = sscanf(line, "%d %d %d %d %d %d %d",
                &working_days[0], &working_days[1], &working_days[2], &working_days[3], &working_days[4], &working_days[5], &working_days[6]);
            memcpy(current_node->data.working_days, working_days, sizeof(int) * 7);
            for (int i = 0; i < 7; i++)
            {
                if (current_node->data.working_days[i] == 1)
                {
                    WORKERS_CAPACITY[i].applied++;
                }
            }

        }
    }
    return head;
}
Node* find_node_by_name(char* name, Node* head)
{
    Node* currentNode = head;
    bool found = false;
    while (currentNode != NULL && !found)
    {
        if (strcmp(currentNode->data.name, name) == 0)
        {
            found = true;
        }
        else
        {
            currentNode = currentNode->next;
        }
    }
    if (found == true)
    {
        return currentNode;
    }
    else
    {
        return NULL;
    }
}
bool delete_node(Node* node, Node** head_ref, Node** tail_ref)
{
    bool deleted = false;

    if (*head_ref == NULL)
    {
        // The list is empty, nothing to delete
        return deleted;
    }
    // the node to delete is the first node
    if (*head_ref == node)
    {
        *head_ref = node->next;
        deleted = true;

    }
    //the node to delete is the last node
    if (*tail_ref == node)
    {
        *tail_ref = node->prev;
        deleted = true;

    }
    // Update the next pointer of the previous node
    if (node->prev != NULL)
    {
        node->prev->next = node->next;
        deleted = true;
    }

    // Case 3: Update the previous pointer of the next node
    if (node->next != NULL)
    {
        node->next->prev = node->prev;
        deleted = true;
    }

    // Free the memory occupied by the node to delete
    free(node);
    return deleted;
}
void fill_working_days(Node* node, Node** head_ref, Node** tail_ref)
{
    int days[7];
    for (int i = 0; i < 7; i++)
    {
        node->data.working_days[i] = -1;
        days[i] = -1;
    }

    char line[100];
    bool ok_working_days = false;

    int x = -1;
    printf("Please enter the number of the days you would like to work ");
    scanf("%d", &x);
    while (!ok_working_days)
    {


        bool days_found = true;
        if (7 >= x && x > 0)
        {
            char print_str[30];
            int len = 0;
            for (int i = 0; i < x; i = i + 2)
            {
                strcat(print_str, "%s ");
                len = len + 3;
            }
            print_str[len] = '\0';

            printf("Please enter your working days in format: DAY1 DAY2 DAY3 ... : ");
            char days_s[7][15];
            switch (x)
            {
            case 1:
                scanf("%s ", days_s[0]);
                //printf("%s ", days_s[0]);
                break;
            case 2:
                scanf("%s %s", days_s[0], days_s[1]);
                // printf("%s %s", days_s[0], days_s[1]);
                break;
            case 3:
                scanf("%s %s %s", days_s[0], days_s[1], days_s[2]);
                // printf("%s %s %s", days_s[0], days_s[1],days_s[2]);
                break;
            case 4:
                scanf("%s %s %s %s ", days_s[0], days_s[1], days_s[2], days_s[3]);
                //printf("%s %s %s %s ", days_s[0], days_s[1], days_s[2], days_s[3]);
                break;
            case 5:
                scanf("%s %s %s %s %s", days_s[0], days_s[1], days_s[2], days_s[3], days_s[4]);
                //printf("%s %s %s %s %s", days_s[0], days_s[1], days_s[2], days_s[3],days_s[4]);
                break;
            case 6:
                scanf("%s %s %s %s %s %s", days_s[0], days_s[1], days_s[2], days_s[3], days_s[4], days_s[5]);
                // printf("%s %s %s %s %s %s", days_s[0], days_s[1], days_s[2], days_s[3],days_s[4],days_s[5]);
                break;
            case 7:
                scanf("%s %s %s %s %s %s %s", days_s[0], days_s[1], days_s[2], days_s[3], days_s[4], days_s[5], days_s[6]);
                // printf("%s %s %s %s %s %s %s", days_s[0], days_s[1], days_s[2], days_s[3],days_s[4],days_s[5],days_s[6]);
                break;
            }
            // filling days array and checking if there are any days that are full
            //working_days[i] = {  1 if the applicant will work during the day
            //   *                      -1 if the applicant will not work during the day}
            int full_days[7] = { -1, -1, -1, -1, -1, -1, -1 };
            int nb_full_days = 0;
            //bool days_found
            for (int i = 0; i < x; i++)
            {
                bool found = false;
                for (int j = 0; j < 7; j++)
                {
                    if (strcmp(days_s[i], get_day(j)) == 0)
                    {
                        days[j] = 1;
                        if (WORKERS_CAPACITY[j].MAX_CAPACITY > WORKERS_CAPACITY[j].applied)

                        {
                            node->data.working_days[j] = 1;
                            WORKERS_CAPACITY[j].applied++;
                        }
                        else
                        {
                            // update the array that will store the days the worker wanted to work in but they are full
                            full_days[j] = 1;
                            nb_full_days++;
                            //newnode->data.working_days[i] = 0;
                        }
                        found = true;
                    }
                }
                if (!found)
                {
                    days_found = false;
                    printf("\n the data you entered is not valid \n");
                }

            }

            if (days_found == true)
            {
                if (nb_full_days > 0)
                {
                    printf("Unfortunately ,your application for these days is rejected :");
                    for (int i = 0; i < 7; i++)
                    {
                        if (full_days[i] == 1)
                        {
                            node->data.working_days[i] = -1;
                            printf(" %s \n", week_days_str[i]);
                        }

                    }
                }
                if (nb_full_days == x)
                {
                    delete_node(node, head_ref, tail_ref);
                }
            }
        }
        else
        {
            printf("\n Please enter a valid number of the days you would like to work ");
            scanf("%d", &x);
        }
        if (days_found)
        {
            ok_working_days = true;
        }


    }
}
Node* find_worker_node(Node** head_ref){
    char name[20];
    bool ok_name = false;
    Node* worker_node = NULL;

    printf("Please enter your name : \n");
    int result_name = scanf("%s", name);
    name[20] = '\0';
    while (!ok_name)
    {

        if (result_name == 1)
        {
            if (find_node_by_name(name, *head_ref) != NULL) //node is found
            {
                worker_node = find_node_by_name(name, *head_ref);
                ok_name = true;
            }
            else // node is not found then name is not valid
            {
                printf("Please enter a valid applicant name : \n");
                int result_name = scanf("%s", name);
                name[20] = '\0';
            }

        }
        else
        {
            printf("Please enter a valid input name : \n");
            int result_name = scanf("%s", name);
            name[20] = '\0';

        }
    }
    return worker_node;
}
void modify_name(Node* worker_node, Node** applicants_head_ref){
    char name[20];
    bool ok_name = false;
    printf("Please enter your new name : \n");
    int result_name = scanf("%s", name);
    name[20] = '\0';
    while (!ok_name)
    {

        if (result_name == 1)
        {
            strncpy(worker_node->data.name, name, sizeof(worker_node->data.name));
            ok_name = true;

        }
        else
        {
            printf("Please enter a valid name : \n");
            int result_name = scanf("%s", name);
            name[20] = '\0';
        }
    }
}
bool new_applicant(Node** head_ref, Node** tail_ref){
    char name[20];
    bool ok_name = false;
    printf("Please enter your name : \n");
    int result_name = scanf("%s", name);
    name[20] = '\0';
    while (!ok_name)
    {

        if (result_name == 1)
        {
            Node* newnode = insert_at_tail(head_ref, tail_ref, name);
            fill_working_days(newnode, head_ref, tail_ref);
            ok_name = true;

        }
        else
        {
            printf("Please enter a valid name : \n");
            printf("Please enter your name : \n");

            result_name = scanf("%s", name);
            name[20] = '\0';
        }
    }
    return ok_name;
}
void overwrite_d_linked_list_in_file(FILE* file, Node* head){
    if (file == NULL)
    {
        printf("Failed to open file for writing.\n");
        return;
    }
    Node* current = head;
    // Loop through the list and write each element to the file
    while (current != NULL)
    {
        fprintf(file, "%s\n", current->data.name);
        fprintf(file, "%d %d %d %d %d %d %d\n", current->data.working_days[0], current->data.working_days[1], current->data.working_days[2], current->data.working_days[3], current->data.working_days[4], current->data.working_days[5], current->data.working_days[6]);
        current = current->next;
    }
}
// implementation of :    modify_working_days(worker_node ,applicants_head_ref);
void Welcome_msg(){
    printf("Welcome to the \"Drink My Friend\" vineyard.\n");
    printf("As we all know, grapes love care and work, but most of all they love labour, which is usually rewarded with a good harvest.\n");
    return;
}
Node* get_workers_of_the_day1(int day_num, int* nb_workers, Node** head_ref, Node** tail_ref) {

    Node* current_node = *head_ref;
    Node* new_head = NULL;
    Node* new_tail = NULL;
    *nb_workers = 0;

    // Iterate through the nodes to find the workers who work on the given day_num
    while (current_node != NULL) {
        if (current_node->data.working_days[day_num] == 1) {
            insert_at_tail(&new_head, &new_tail, current_node->data.name);
            printf("%s\n" ,current_node->data.name );
            (*nb_workers)++;
        }
        current_node = current_node->next;
    }
    print_d_linked_list(new_head);
    *head_ref = new_head;
    *tail_ref = new_tail;

    return new_head;

}
char** get_workers_of_the_day(int day_num, int* nb_workers, Node** head_ref, Node** tail_ref) {
    Node* current_node = *head_ref;
    char** worker_names = NULL;
    *nb_workers = 0;

    // Iterate through the nodes to find the workers who work on the given day_num
    while (current_node != NULL) {
        if (current_node->data.working_days[day_num] == 1) {
            worker_names = realloc(worker_names, (*nb_workers + 1) * sizeof(char*));
            worker_names[*nb_workers] = strdup(current_node->data.name);
            printf("%s\n", worker_names[*nb_workers]);
            (*nb_workers)++;
        }
        current_node = current_node->next;
    }

    *head_ref = NULL;
    *tail_ref = NULL;

    return worker_names;
}

int get_day_number_from_admin() {
    int day_num = -1;
    printf("which day\'s are needed today \n");
    printf("NOTE : Please enter the day\'s as shown below :\n");
    for (int i = 0; i < 7; i++) {
        printf("%i : %s \n", i, week_days_str[i]);
    }
    bool ok_day = false;
    do {
        scanf("%d", &day_num);
        if (day_num < 0 || 7 <= day_num) {
            printf("Please enter a valid day number as shown in the menu : \n");
            scanf("%d", &day_num);
        }
        else {
            ok_day = true;
            return day_num;
        }
    } while (!ok_day);
    return day_num;
}
volatile sig_atomic_t BUS1_signal_received = 0; // Flag to indicate if BUS 1 signal is received
volatile sig_atomic_t BUS2_signal_received = 0; // Flag to indicate if BUS 2 signal is received
void sig_BUS1_handler(int sig_number) {
    BUS1_signal_received = 1;
    printf("Vineyard recieved signal %i that Bus 1 is ready\n", sig_number);
}
void sig_BUS2_handler(int sig_number) {
    BUS2_signal_received = 1;
    printf("Vineyard recieved signal %i that Bus 2 is ready\n", sig_number);
}
struct messg {
    long mtype;//this is a free value e.g for the address of the message
    int m_successfully_transported; //this is the message itself
};
// sendig a message
int send(int mqueue, int key, int bus_nb, int nb)
{
    const struct messg m = { bus_nb, nb };
    int status;

    status = msgsnd(mqueue, &m, sizeof(nb), 0);
    // a 3rd parameter may be : sizeof(m.mtext)
    // a 4th parameter may be  IPC_NOWAIT, equivalent with  0-val
    if (status < 0)
        perror("msgsnd error\n");
    return 0;
}
// receiving a message.
int receive(int mqueue, int key, int bus_nb)
{
    struct messg m;
    int status;
    // the last parameter is the number of the message
   // if it is 0, then we read the first one
   // if  >0 (e.g. 5), then message type 5 will be read

    status = msgrcv(mqueue, &m, sizeof(int), bus_nb, 0);

    if (status < 0)
        perror("msgsnd error");
    else
        printf("The code of the message is : %ld, Bus %d successfully transported %d workers \n", m.mtype, bus_nb, m.m_successfully_transported);
    return 0;
}
int main(int argc, char* argv[])
{
    //creating the applicants node from file
    const char* filename = "applicants.txt";

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    Node* head = read_d_linked_list_from_file(file);
    fclose(file);

    Node* tail = find_tail(head);
    Node** head_ref = &head;
    Node** tail_ref = &tail;
    // Signal handling configuration

    int login = -1;
    bool log_in = false;
    //   	file = fopen(filename, "w");
    //    if (file == NULL) {
    //    printf("Failed to open file %s\n", filename);
    //    return 1;}
    bool quit = false;
    while (!quit)
    {
        log_in = false;
        printf("login as : please write the number of the operation you would like to perform as follows \n");
        printf("0 if you would like to quit the application \n");
        printf("1 if you are logging in as 'Drink My Friend' vineyard admin to start transporting workers \n");
        printf("2 if you are logging in as a new applicant\n");
        printf("3 if you are logging in as an old applicant and would like to modify your name\n");
        printf("4 if you are logging in as an old applicant and would like to modify your working days\n");
        printf("5 if you are logging in as an old applicant and would like to quit your work ( delete your old application) \n");
        scanf("%d", &login);
        while (!log_in)
        {

            Node* worker_node;
            switch (login)
            {
                case -1:
                    printf("Please choose a valid number from 0 to 5 : \n");
                    fflush(stdin); // flush any extra characters in input buffer
                    scanf("%d", &login);
                    printf("%d",login);
                    break;// error case
                case 0:
                    log_in = true;
                    quit = true;
                    return 0;
                    break;
                case 1:
                    // code to list the applicants
                    printf("here is a list of the applicants and their working days: \n");
                    print_d_linked_list(head);
                    printf("\nSuccessful login as 'Drink My Friend' vineyard admin \n");
                    log_in = true;
                    signal(SIGUSR1, sig_BUS1_handler);
                    signal(SIGUSR2, sig_BUS2_handler);

                    //creating the message queue:
                    int messg, status;
                    key_t key = ftok(argv[0], 1);
                    printf("The key for message queue : %d\n", key);
                    messg = msgget(key, 0600 | IPC_CREAT);
                    if (messg < 0) {
                        perror("msgget error\n");
                        return 1;
                    }

                    // Creating the unnamed pipes
                    int pipe_child1_fd[2];
                    int pipe_child2_fd[2];
                    // Opening the 3 pipe files
                    if (pipe(pipe_child1_fd) == -1 || pipe(pipe_child2_fd) == -1) {
                        perror("Pipe opening error!\n");
                        exit(EXIT_FAILURE);
                    }

                    // Creating parent-child processes
                    pid_t pid_child1 = fork();

                    if (pid_child1 < 0) {
                        perror("Fork 1 error\n");
                        exit(EXIT_FAILURE);
                    }
                    else if (pid_child1 > 0) {
                        // Parent process
                        pid_t pid_child2 = fork();
                        if (pid_child2 < 0) {
                            perror("Fork 2 error\n");
                            exit(EXIT_FAILURE);
                        }
                        else if (pid_child2 > 0) {
                            // also Parent process
                            printf("Vineyard process starts \n");

                            close(pipe_child1_fd[0]);
                            close(pipe_child2_fd[0]);

                            int day_number = get_day_number_from_admin();
                            printf("day number is %d\n", day_number);
                            printf("Vineyard process starts for %s :\n", week_days_str[day_number]);

                            // Iterate through the nodes to find the workers who work on the given day_num
                            int nb_workers_V = 0;
                            Node* current_node = head;
                            char list_of_workers1[256];
                            list_of_workers1[0] = '\0';
                            //char** worker_names = NULL;
                            int i = 0;
                            while (current_node != NULL && i <= 5) {
                                if (current_node->data.working_days[day_number] == 1) {
                                    //worker_names = realloc(worker_names, (nb_workers_V + 1) * sizeof(char*));
                                    //worker_names[nb_workers_V] = strdup(current_node->data.name);
                                    //printf("%s \n", current_node->data.name);
                                    nb_workers_V++;
                                    strcat(list_of_workers1, current_node->data.name);
                                    strcat(list_of_workers1, "\n");
                                }
                                current_node = current_node->next;
                                i++;
                            }
                            //Wait for the signal from child 1
                            if (!BUS1_signal_received) {
                                printf("Vineyard waiting for signal form bus 1 !\n");
                                pause();
                            }
                            //.write the number of workers and the name of workers for that day via pipe
                            char str[10];
                            sprintf(str, "%d", nb_workers_V);
                            write(pipe_child1_fd[1], str, 10);
                            write(pipe_child1_fd[1], list_of_workers1, strlen(list_of_workers1) + 1);
                            //send to bus 2 the nb of workers
                            //Wait for the signal from child 1
                            if (!BUS2_signal_received) {
                                    printf("Vineyard waiting for signal from bus 2 !\n");
                                    pause();
                            }
                                //.write the number of workers to be transported by bus 2  and the name of workers for that day via pipe
                            if (nb_workers_V <=5 ){
                                nb_workers_V = 0;
                                char str[10];
                                sprintf(str, "%d", nb_workers_V);
                                write(pipe_child2_fd[1], str, 10);
                            }else
                            {
                                //Receive signal from the BUS 2
                                // Wait for Bus 1 to send SIGUSR2 signal
                                // Iterate through the nodes to find the workers who work on the given day_num
                                nb_workers_V = 0;
                                char list_of_workers2[256];
                                list_of_workers2[0] = '\0';
                                int i = 0;
                                while (current_node != NULL && i <= 5) {
                                    if (current_node->data.working_days[day_number] == 1) {
                                        nb_workers_V++;
                                        strcat(list_of_workers1, current_node->data.name);
                                        strcat(list_of_workers1, "\n");
                                    }
                                    current_node = current_node->next;
                                    i++;
                                }
                                char str[10];
                                sprintf(str, "%d", nb_workers_V);
                                write(pipe_child2_fd[1], str, 10);
                                write(pipe_child2_fd[1], list_of_workers1, strlen(list_of_workers1) + 1);
                            }
                            //11.Receive the number of the successfully transported workers x from bus1   and bus 2+ writes it to screen
                            receive(messg, key, 1);
                            receive(messg, key, 2);
                            //13.Parent waits for the end of the child 1 and 2 ,
                            sleep(5);
                            //waitpid(pid_child1, NULL, 0);
                            //waitpid(pid_child2, NULL, 0);
                            waitpid(-1,NULL,0);
                            fflush(NULL);
                            printf("Parent finished!\n");
                        }
                        else {
                            // Bus 2 process
                            sleep(3);
                            printf("Bus 2 woke up  \n");
                            //5.Send the signal via the pipe that bus is ready
                            kill(getppid(), SIGUSR2);
                            //8.Receive the list of the workers names for that day via the pipe
                            //a.reciecve the number of workers-----------------------------------
                            printf("Bus 2 is ready to go  \n");
                            char str_bus2[10];
                            read(pipe_child2_fd[0], str_bus2, 10);
                            int nb_workers = atoi(str_bus2);
                            //b.reciecve the name of workers
                            int nb_workers_available;
                            int nb_successfully_transported = 0;
                            int nb_workers_to_bus2 = 0;
                            if (nb_workers == 0) {
                                send(messg, key, 2, nb_workers_to_bus2);
                                printf("Bus 2 is not needed so it is terminating ");
                                close(pipe_child2_fd[0]); // finally we close the used read end
                                exit(0);	// force exit, not necessary
                            }else if (nb_workers > 5) {
                                nb_workers_to_bus2 = 5;
                                //nb_workers_available = nb_workers - 5;
                            }else {
                                nb_workers_to_bus2 = nb_workers;
                                //nb_workers_available = 0;
                             }
                            char str_l[256];
                            str_l[0] = '\0';
                            read(pipe_child2_fd[0], str_l, 256);
                            printf("Bus 2 successfully transported the following workers :\n%s", str_l);
                            //10.Send the number of the successfully transported workers to parent via message queue
                            send(messg, key, 1, nb_workers_to_bus2);
                            //12.Terminate
                            //close the read end of the pipe once we are no longuer using it
                            close(pipe_child2_fd[0]); // finally we close the used read end
                            exit(0);	// force exit, not necessary
                        }
                    }else {
                        //BUS  1
                        sleep(3);
                        printf("Bus 1 woke up \n");
                        //5.Send the signal via the pipe that bus is ready

                        kill(getppid(), SIGUSR1);
                        //8.Receive the list of the workers names for that day via the pipe

                        //a.reciecve the number of workers
                        sleep(3);
                        printf("Bus 1 is ready to go \n");
                        char str_bus1[10];
                        read(pipe_child1_fd[0], str_bus1, sizeof(str_bus1));
                        int nb_workers = atoi(str_bus1);
                        //b.reciecve the name of workers
                        int nb_workers_available;
                        int nb_successfully_transported = 0;
                        int nb_workers_to_bus1 = 0;
                        if (nb_workers > 5) {
                            nb_workers_to_bus1 = 5;
                            //nb_workers_available = nb_workers - 5;
                        }
                        else {
                            nb_workers_to_bus1 = nb_workers;
                            //nb_workers_available = 0;
                        }
                        char str_l[256];
                        str_l[0] = '\0';
                        read(pipe_child1_fd[0], str_l, 256);
                        printf("Bus 1 successfully transported the following workers :\n%s", str_l);
                        //10.Send the number of the successfully transported workers to parent via message queue
                        send(messg, key, 1, nb_workers_to_bus1);

                        //12.Terminate
                        //close the read end of the pipe once we are no longuer using it
                        close(pipe_child1_fd[0]); // finally we close the used read end
                        exit(EXIT_SUCCESS);	// force exit, not necessary
                    }
                    break;
                case 2:
                    new_applicant(head_ref, tail_ref);    //applicant login
                                                                                //print_d_linked_list(*applicants_head_ref);
                    file = fopen(filename, "w");
                    overwrite_d_linked_list_in_file(file, *head_ref);
                    fclose(file);
                    printf("Successful log in as a new applicant\n");
                    log_in = true;
                    break;
                case 3:
                    // log in an old applicant to change the name
                    worker_node = find_worker_node(head_ref);
                    modify_name(worker_node, head_ref);
                    file = fopen(filename, "w");
                    overwrite_d_linked_list_in_file(file, *head_ref);
                    fclose(file);
                    printf("Successful log in as a worker to modify your name \n");
                    log_in = true;
                    break;
                case 4:
                    // log in an old applicant to change the working days
                    worker_node = find_worker_node(head_ref);
                    fill_working_days(worker_node, head_ref,tail_ref);
                    file = fopen(filename, "w");
                    overwrite_d_linked_list_in_file(file, *head_ref);
                    fclose(file);
                    printf("Successful log in as a worker to modify your working days \n");
                    log_in = true;
                    break;
                case 5:
                    // log in an old applicant to delete application
                    worker_node = find_worker_node(head_ref);
                    log_in = delete_node(worker_node,head_ref,tail_ref);
                    file = fopen(filename, "w");
                    overwrite_d_linked_list_in_file(file, *head_ref);
                    fclose(file);
                    printf("Application Successfully deleted\n");
                    log_in = true;
                    break;
            }
        }
    }

        return (0);
}
