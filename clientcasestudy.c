#include<stdio.h>
#include"servercasestudy.h"
int main()
{
	char flight_num[1000][10], source[1000][10], dest[1000][10], ask;
	int number_of_seats[1000][10], choice, n;
	float start_time[1000][10], end_time[1000][10];
	enum type t[1000][10];
	do
	{
	printf("1. Add flight details\n2. Display flight details\nEnter your choice: ");
	scanf("%d",&choice);
	switch(choice)
	{
		case 1:
            printf("Enter the number of flight details you want to add: ");
            scanf("%d",&n);			  
		    add_flight_details(n,flight_num,number_of_seats,source,dest,start_time,end_time,t);break;
		case 2: display_flight_details(n,flight_num,number_of_seats,source,dest,start_time,end_time,t);break;
	}
	printf("\nDo you want to continue? (y/n): ");
    scanf(" %c", &ask);
	}while(ask == 'y' || ask == 'Y');
	return 0;
}
