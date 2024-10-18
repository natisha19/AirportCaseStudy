enum type{
		Private,Domestic,International
	};
void add_flight_details(int n, char(*pflight_num)[10], int* (number_of_seats)[10], char(*psource)[10],char(*pdest)[10], float (*start_time)[10],float (*end_time)[10], enum type(* t)[10]);
void display_flight_details(int n, char(*pflight_num)[10], int* (number_of_seats)[10], char(*psource)[10],char(*pdest)[10], float (*start_time)[10],float (*end_time)[10], enum type(* t)[10]t);