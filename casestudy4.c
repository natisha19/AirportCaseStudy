#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ADMIN_PASS "pass"

typedef enum
{
    PRIVATE,
    DOMESTIC,
    INTERNATIONAL
} FlightType;

typedef struct
{
    char flight_number[100];
    int number_of_seats;
    float cost_per_seat;
    char src[100];
    char dest[100];
    char start_time[100];
    char end_time[100];
    FlightType type;
} FlightData;

typedef struct FlightNode
{
    FlightData data;
    struct FlightNode *next;
} FlightNode;

typedef struct
{
    char name[100];
    int age;
    char address[100];
} Passenger;

typedef struct
{
    char flight_number[100];
    char src[100];
    char dest[100];
    FlightType type;
    int no_of_tickets;
    float cost_per_ticket;
    Passenger *passengers;
} Booking;

// globals
static FlightNode *flight_list = NULL;
static unsigned int no_of_bookings = 0;
static Booking *bookings = NULL;

bool does_file_exist(const char *filename)
{
    FILE *file;
    if (file = fopen(filename, "r"))
    {
        fclose(file);
        return true;
    }
    return false;
}

void read_flight_data_from_file()
{
    FILE *fp = fopen("data.csv", "r");

    if (!fp)
    {
        printf("Data file not found.\n");
        return;
    }

    char line[1024];

    // skip header
    fgets(line, 1024, fp);
    
    while (fgets(line, 1024, fp))
    {
        FlightNode *new = (FlightNode *)malloc(sizeof(FlightNode));
        new->next = NULL;

        FlightData *data = &new->data;

        char *token = strtok(line, ",");
        strcpy(data->flight_number, token);

        token = strtok(NULL, ",");
        data->number_of_seats = atoi(token);

        token = strtok(NULL, ",");
        data->cost_per_seat = atof(token);

        token = strtok(NULL, ",");
        strcpy(data->src, token);

        token = strtok(NULL, ",");
        strcpy(data->dest, token);

        token = strtok(NULL, ",");
        strcpy(data->start_time, token);

        token = strtok(NULL, ",");
        strcpy(data->end_time, token);

        token = strtok(NULL, ",");
        data->type = atoi(token);

        if (flight_list == NULL)
        {
            flight_list = new;
        }
        else
        {
            FlightNode *current = flight_list;
            while (current->next != NULL)
            {
                current = current->next;
            }
            current->next = new;
        }
    }

    fclose(fp);
}

void add_flight_details()
{
    printf("Enter flight details: \n");

    if (!does_file_exist("data.csv"))
    {
        FILE *fp = fopen("data.csv", "w");
        fprintf(fp, "flight_number,number_of_seats,cost_per_seat,src,dest,start_time,end_time,type\n");
        fclose(fp);
    }

    FILE *fp = fopen("data.csv", "a");

    char ch;
    do
    {
        FlightNode *new = (FlightNode *)malloc(sizeof(FlightNode));
        new->next = NULL;

        FlightData *data = &new->data;
        printf("Enter the flight number: ");
        scanf("%s", (data->flight_number));
        printf("Enter the number of seats: ");
        scanf("%d", &(data->number_of_seats));
        printf("Enter cost per seat: ");
        scanf("%f", &(data->cost_per_seat));
        printf("Enter the source: ");
        scanf(" %[^\n]s", (data->src));
        printf("Enter the destination: ");
        scanf(" %[^\n]s", (data->dest));
        printf("Enter the start time: ");
        scanf(" %s", (data->start_time));
        printf("Enter the end time: ");
        scanf(" %s", (data->end_time));
        printf("Enter the type of flight (0: PRIVATE, 1: DOMESTIC, 2: INTERNATIONAL): ");
        int type;
        scanf("%d", &type);
        data->type = type;

        if (flight_list == NULL)
        {
            flight_list = new;
        }
        else
        {
            FlightNode *current = flight_list;
            while (current->next != NULL)
            {
                current = current->next;
            }
            current->next = new;
        }

        // add to data file
        fprintf(fp, "%s,%d,%.2f,%s,%s,%s,%s,%d\n", data->flight_number, data->number_of_seats,
                data->cost_per_seat, data->src, data->dest, data->start_time, data->end_time, data->type);

        printf("Do you want to add more flight details? (y/n)");
        scanf(" %c", &ch);
    } while (ch == 'y' || ch == 'Y');

    fclose(fp);
}

void display_flight_details(FlightData *data)
{
    printf("Flight number: %s\n", data->flight_number);
    printf("Number of seats: %d\n", data->number_of_seats);
    printf("Cost per seat: Rs. %.2f\n", data->cost_per_seat);
    printf("Source: %s\n", data->src);
    printf("Destination: %s\n", data->dest);
    printf("Start time: %s\n", data->start_time);
    printf("End time: %s\n", data->end_time);
    printf("Type: %s\n", data->type == PRIVATE ? "PRIVATE" : data->type == DOMESTIC ? "DOMESTIC"
                                                                                    : "INTERNATIONAL");
}

void display_all_flight_details()
{

    FlightNode *current = flight_list;
    while (current != NULL)
    {
        FlightData *data = &current->data;
        display_flight_details(data);
        printf("\n");
        current = current->next;
    }
}

FlightData *find_flight_with_src_dest_type(FlightNode *list, const char *src, const char *dest, FlightType type)
{
    FlightNode *current = list;

    while (current != NULL)
    {
        FlightData *data = &current->data;
        if (strcmp(data->src, src) == 0 && strcmp(data->dest, dest) == 0 && data->type == type)
        {
            return data;
        }
        current = current->next;
    }

    return NULL;
}

void display_flights_filtered_by_src_dest()
{
    char src[100], dest[100];
    printf("Enter source: ");
    scanf(" %[^\n]s", src);
    printf("Enter destination: ");
    scanf(" %[^\n]s", dest);

    printf("\n");

    FlightNode *current = flight_list;
    bool found_one = false;

    while (current != NULL)
    {
        FlightData *data = &current->data;
        if (strcmp(data->src, src) == 0 && strcmp(data->dest, dest) == 0)
        {
            found_one = true;
            display_flight_details(data);
        }
        current = current->next;
    }

    if (!found_one)
    {
        printf("No flights found for the given source and destination.\n");
    }
}

void book_tickets()
{
    // book n tickets
    int n;
    printf("Enter the number of tickets: ");
    scanf("%d", &n);

    if (n <= 0)
    {
        printf("Invalid number of tickets.\n");
        return;
    }

    char src[100], dest[100];
    int type;

    printf("Enter source: ");
    scanf(" %[^\n]s", src);
    printf("Enter destination: ");
    scanf(" %[^\n]s", dest);
    printf("Enter flight type (0: PRIVATE, 1: DOMESTIC, 2: INTERNATIONAL): ");
    scanf("%d", &type);
    printf("\n");

    // search for flight with given src and dest
    FlightData *flight = find_flight_with_src_dest_type(flight_list, src, dest, type);

    if (flight == NULL)
    {
        printf("No flights found for the given source and destination.\n");
        return;
    }

    if (bookings == NULL)
    {
        bookings = (Booking *)malloc(sizeof(Booking));
    }
    else
    {
        bookings = (Booking *)realloc(bookings, (no_of_bookings + 1) * sizeof(Booking));
    }
    no_of_bookings++;

    Booking *booking = &bookings[no_of_bookings - 1];
    booking->passengers = (Passenger *)malloc(n * sizeof(Passenger));
    booking->no_of_tickets = n;
    booking->type = flight->type;
    strcpy(booking->src, flight->src);
    strcpy(booking->dest, flight->dest);
    strcpy(booking->flight_number, flight->flight_number);
    booking->cost_per_ticket = flight->cost_per_seat;

    printf("Flight found: \n");
    display_flight_details(flight);
    printf("\n");

    for (int i = 0; i < n; i++)
    {
        Passenger *passenger = &booking->passengers[i];
        printf("Enter passenger %d details: \n", i + 1);
        printf("Enter name: ");
        scanf(" %[^\n]s", passenger->name);
        printf("Enter age: ");
        scanf("%d", &passenger->age);
        printf("Enter address: ");
        scanf(" %[^\n]s", passenger->address);
    }

    printf("Tickets booked successfully.\n");
}

void display_booking_summary()
{
    printf("Booking summary: \n");

    if (bookings == NULL || no_of_bookings == 0)
    {
        printf("No bookings found.\n");
        return;
    }

    for (int i = 0; i < no_of_bookings; i++)
    {
        Booking booking = bookings[i];
        printf("\nBooking no. %d:\n", i + 1);
        printf("Flight number: %s\n", booking.flight_number);
        printf("Source: %s\n", booking.src);
        printf("Destination: %s\n", booking.dest);
        printf("Type: %s\n", booking.type == PRIVATE ? "PRIVATE" : booking.type == DOMESTIC ? "DOMESTIC"
                                                                                            : "INTERNATIONAL");
        printf("No. of tickets: %d\n", booking.no_of_tickets);
        printf("Passengers: \n");
        for (int j = 0; j < booking.no_of_tickets; j++)
        {
            Passenger passenger = booking.passengers[j];
            printf("%d) ", j + 1);
            printf("Name: %s\t", passenger.name);
            printf("Age: %d\t", passenger.age);
            printf("Address: %s\t", passenger.address);

            float cost = booking.cost_per_ticket;

            if (passenger.age > 65)
            {
                cost *= 0.8;
            }

            printf("Ticket cost: Rs. %.2f\n", cost);
        }
    }
}

void show_admin_options()
{
    char ch;

    do
    {
        printf("Enter your choice: \n");
        printf("1.Add flight details.\n");
        printf("2.Display flight details.\n");
        printf("3.Exit to login prompt.\n");

        int choice;
        scanf("%d", &choice);
        fflush(stdin);

        switch (choice)
        {
        case 1:
            printf("\n");
            add_flight_details();
            break;
        case 2:
            printf("\n");
            printf("All flight details: \n\n");
            display_all_flight_details();
            break;
        case 3:
            printf("Exiting to login prompt.\n");
            return;
        default:
            printf("Invalid input");
        }

        printf("\nWould you like to continue? (y/n) ");
        scanf(" %c", &ch);
    } while (ch == 'y' || ch == 'Y');
}

void show_passenger_options()
{
    char ch;

    do
    {
        printf("Enter your choice: \n");
        printf("1.Book tickets.\n");
        printf("2.Find flights based on source and destination.\n");
        printf("3.Display booking summary.\n");
        printf("4.Exit to login prompt.\n");

        int choice;
        scanf("%d", &choice);
        fflush(stdin);

        switch (choice)
        {
        case 1:
            printf("\n");
            book_tickets();
            break;
        case 2:
            printf("\n");
            display_flights_filtered_by_src_dest();
            break;
        case 3:
            printf("\n");
            display_booking_summary();
            break;
        case 4:
            printf("Exiting to login prompt.\n");
            return;
        default:
            printf("Invalid input");
        }

        printf("\nWould you like to continue? (y/n) ");
        scanf(" %c", &ch);
    } while (ch == 'y' || ch == 'Y');
}

int main()
{   
    read_flight_data_from_file();

    char ch;
    bool should_exit = false;

    while (!should_exit)
    {
        printf("Login options:\n");
        printf("A. Admin\n");
        printf("U. User\n");
        printf("X. Exit\n");

        char choice;
        scanf(" %c", &choice);

        if (choice == 'A')
        {
            char pwd[100];
            printf("Enter password: \n");
            scanf(" %[^\n]s", pwd);

            if (strcmp(pwd, ADMIN_PASS) == 0)
            {
                printf("Login successful.\n\n");
                show_admin_options();
            }
            else
            {
                printf("Invalid password.\n");
            }
        }
        else if (choice == 'U')
        {
            show_passenger_options();
        }
        else if (choice == 'X')
        {
            printf("Exiting.\n");
            should_exit = true;
        }
        else
        {
            printf("Invalid input.\n");
        }
    }

    return 0;
}

