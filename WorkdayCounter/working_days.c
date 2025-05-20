#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

/**
 * @struct Result
 * @brief Stores total days and workdays between two dates.
 */
typedef struct {
    int totalDays;   /**< Total number of days in range */
    int workDays;    /**< Number of working days in range */
} Result;

/**
 * @brief Checks if a given year is a leap year.
 * 
 * @param y Year to check
 * @return true if leap year, false otherwise
 */
bool IsLeapYear(int y) {
    return (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0));
}

/**
 * @brief Checks if a specific date is a workday (not a weekend or holiday).
 * 
 * @param y Year
 * @param m Month
 * @param d Day
 * @return true if it's a workday, false otherwise
 */
bool IsWorkDay(int y, int m, int d) {
    int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (IsLeapYear(y)) daysInMonth[1] = 29;

    // Validate date
    if (y < 2000 || m < 1 || m > 12 || d < 1 || d > daysInMonth[m - 1])
        return false;

    // Czech national holidays (fixed date)
    if ((m == 1 && d == 1) ||
        (m == 5 && (d == 1 || d == 8)) ||
        (m == 7 && (d == 5 || d == 6)) ||
        (m == 9 && d == 28) ||
        (m == 10 && d == 28) ||
        (m == 11 && d == 17) ||
        (m == 12 && (d == 24 || d == 25 || d == 26)))
        return false;

    // Zeller's Congruence approximation for day of the week
    int offset[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    if (m < 3) y--;
    int weekday = (y + y / 4 - y / 100 + y / 400 + offset[m - 1] + d) % 7;

    return weekday >= 1 && weekday <= 5;  // Monday-Friday
}

/**
 * @brief Counts total and working days between two dates (inclusive).
 * 
 * @param y1 Start year
 * @param m1 Start month
 * @param d1 Start day
 * @param y2 End year
 * @param m2 End month
 * @param d2 End day
 * @return Result containing total and workday counts, or -1 for invalid range
 */
Result CountDays(int y1, int m1, int d1, int y2, int m2, int d2) {
    int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    Result result = {0, 0};

    // Leap year adjustment
    if (IsLeapYear(y1) || IsLeapYear(y2)) daysInMonth[1] = 29;

    // Validate input range
    if (y1 > y2 ||
        (y1 == y2 && m1 > m2) ||
        (y1 == y2 && m1 == m2 && d1 > d2) ||
        y1 < 2000 || y2 < 2000 ||
        m1 < 1 || m1 > 12 || m2 < 1 || m2 > 12 ||
        d1 < 1 || d1 > daysInMonth[m1 - 1] ||
        d2 < 1 || d2 > daysInMonth[m2 - 1]) {
        result.totalDays = -1;
        result.workDays = -1;
        return result;
    }

    while (y1 < y2 || (y1 == y2 && m1 < m2) || (y1 == y2 && m1 == m2 && d1 <= d2)) {
        if (IsWorkDay(y1, m1, d1)) result.workDays++;
        result.totalDays++;

        d1++;
        if (IsLeapYear(y1)) daysInMonth[1] = 29; 
        else daysInMonth[1] = 28;
        if (d1 > daysInMonth[m1 - 1]) { d1 = 1; m1++; }
        if (m1 > 12) { m1 = 1; y1++; }
    }

    return result;
}

/**
 * @brief Entry point for test assertions.
 */
int main(int argc, char * argv[]) {
    Result r;

    assert(IsWorkDay(2023, 10, 10));
    assert(!IsWorkDay(2023, 11, 11));
    assert(!IsWorkDay(2023, 11, 17));
    assert(!IsWorkDay(2023, 11, 31));
    assert(!IsWorkDay(2023, 2, 29));
    assert(!IsWorkDay(2004, 2, 29));
    assert(IsWorkDay(2008, 2, 29));
    assert(!IsWorkDay(2001, 2, 29));
    assert(!IsWorkDay(1996, 1, 2));

    r = CountDays(2064, 1, 5, 2064, 1, 1);
    assert(r.totalDays == -1 && r.workDays == -1);

    r = CountDays(2023, 11, 1, 2023, 11, 17);
    assert(r.totalDays == 17 && r.workDays == 12);

    r = CountDays(2023, 11, 1, 2023, 11, 1);
    assert(r.totalDays == 1 && r.workDays == 1);

    r = CountDays(2023, 11, 17, 2023, 11, 17);
    assert(r.totalDays == 1 && r.workDays == 0);

    r = CountDays(2023, 1, 1, 2023, 12, 31);
    assert(r.totalDays == 365 && r.workDays == 252);

    r = CountDays(2024, 1, 1, 2024, 12, 31);
    assert(r.totalDays == 366 && r.workDays == 254);

    r = CountDays(2000, 1, 1, 2023, 12, 31);
    assert(r.totalDays == 8766 && r.workDays == 6072);

    r = CountDays(2001, 2, 3, 2023, 7, 18);
    assert(r.totalDays == 8201 && r.workDays == 5682);

    r = CountDays(2021, 3, 31, 2023, 11, 12);
    assert(r.totalDays == 957 && r.workDays == 666);

    r = CountDays(2001, 1, 1, 2000, 1, 1);
    assert(r.totalDays == -1 && r.workDays == -1);

    r = CountDays(2023, 1, 1, 2023, 2, 29);
    assert(r.totalDays == -1 && r.workDays == -1);

    return EXIT_SUCCESS;
}
