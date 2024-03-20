#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define SUN 0
#define MON 1
#define TUE 2
#define WED 3
#define THU 4
#define FRI 5
#define SAT 6

/**
 * Prompt the user for a date, and find the day of the week it falls on.
 *
 * Based on John Conway's Doomsday algorithm and the "odd + 11"
 * method described by Chamberlain Fong and Michael K. Walters.
 * https://en.wikipedia.org/wiki/Doomsday_rule
 *
 * Program written by Arthur Zarins, 2024
 * */

/* function headers */
bool leapYear(int year);
int findAnchorDay(int year);
int findDoomsday(int month, int year);
int findDayOfWeek(int month, int day, int year);
void convertToStr(int day, char name[]);
bool validDate(int month, int day, int year);

int main(){
	int month, day, year;

	//get user input of month day year
	printf("enter date (m/d/y): ");
	scanf("%d/%d/%d", &month, &day, &year);

	if(validDate(month,day,year) == 0){
		printf("The specified date does not exist!\n");
		return 1;
	}

	int dayOfWeek = findDayOfWeek(month, day, year);
	char dayName[10];
	convertToStr(dayOfWeek, dayName);

	printf("day of week: %s\n", dayName);
	return 0;
}

/* write the day of the week to string "name"*/
void convertToStr(int day, char name[]){
	char weekdays[7][10] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
	if(day >= 7 || day < 0){
	       strcpy(name, "ERROR");	
	} else {
		strcpy(name, weekdays[day]); 
	}
}

/* determine if a date exists */
bool validDate(int month, int day, int year){
	int monthDays[] = {31,28, 31,30, 31,30, 31,31, 30,31, 30,31};
	if(leapYear(year) == 1){
		monthDays[1] = 29; //Update February on leap year
	}
	if(month > 12 || month <= 0) return false;
	int daysInMonth = monthDays[month-1];
	if(day > daysInMonth) return false;
	if(day <= 0) return false;
	return true; // passed all checks
}

/* returns if a given year is a leap year on the Gregorian calendar*/
bool leapYear(int year){
	if(year % 400 == 0){
		return true;
	} else if (year % 100 == 0){
		return false;
	} else if (year % 4 == 0){
		return true;
	}
	return false; //not divisible by 4
}

/* determine the anchor day for the century of a given year */
int findCenturyAnchorDay(int year){
	int c = year / 100; /*round down to start of century */
	int r = c % 4;
	if(r == 0){
		return TUE;
	} else if (r == 1){
		return SUN;
	} else if (r == 2){
		return FRI;
	}
	return WED;	
}

/* 
 * determine the anchor day for the year 
 * using the "odd + 11" method
 * */
int findAnchorDay(int year){
	// take year's last 2 digits
	int t = year % 100;
	if(t % 2 == 1) t += 11;
	t = t / 2;
	if(t % 2 == 1) t += 11;
	t = 7 - (t % 7);

	int centA = findCenturyAnchorDay(year);
	// the year's anchor is t days ahead from the century's anchor
	return (centA + t) % 7;
}

/*
 * Find the doomsday for the month
 * "month" is an integer between 1 and 12
 * */
int findDoomsday(int month, int year){
	int doomsdays[] = {3, 28, 14, 4, 9, 6, 11, 8, 5, 10, 7, 12};
	//change Jan and Feb's doomsdays on a leap year
	if(leapYear(year) == 1){
		doomsdays[0] = 4;
		doomsdays[1] = 29; //Leap day
	}
	return doomsdays[month-1];
}

/* Find numerical day of week using above methods */
int findDayOfWeek(int month, int day, int year){
	// calculate anchor and doomsday
	int anchor = findAnchorDay(year);
	int doomsday = findDoomsday(month, year);

	// calculate weekday, keep it in range 0-6 (inclusive)	
	int weekday = (anchor + (day - doomsday)) % 7;
	while(weekday < 0) weekday += 7;

	return weekday;
}
