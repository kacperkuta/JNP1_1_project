#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <cfloat>

using std::string;
using std::vector;
using std::pair;
using std::list;

vector<vector<pair<pair<int, int>, string> > > routes;
std::map<int, vector<pair<pair<int, int>, string> >* > timetable;

#define IMPOSSIBLE_RIDE -1
#define MINUTES_PER_HOUR 60
#define MAX_TICKETS 3

bool isLetter(char sign) {
    if ((int) (sign) >= (int) ('a') && (int) (sign) <= (int) ('z'))
        return true;
    else return ((int) (sign) >= (int) ('A') && (int) (sign) <= (int) ('Z'));
}

bool isNumber(char sign) {
    return ((int)(sign) >= (int)('0') && (int)(sign) <= (int)('9'));
}

pair<int, int> selectNumberOfRoute(string line, int start) {

    int number = (int)line[start] - (int)('0');
    int position = start + 1;

    while (isNumber(line[position])) {
        number *= 10;
        number += (int)(line[position]) - (int)('0');
        position++;
    }

    return std::make_pair(number, position);
}

bool areTramsWorking (int hour, int minute) {

    if (hour < 5 || hour > 21)
        return false;

    if (minute < 0 || minute > 59)
        return false;

    if (hour == 5 && minute < 55)
        return false;

    return !(hour == 21 && minute > 21);
}

pair<pair<int, int>, int> selectTime(string line, int start) {

    if (!isNumber(line[start]) || line[start] == '0')
        return std::make_pair(std::make_pair(-1,-1),-1);

    int hour, position;
    if (line[start + 1] == ':') {
        hour = (int) (line[start]) - (int) ('0');
        position = start + 1;
    }
    else if (isNumber(line[start + 1])) {
        hour = (int)(line[start]) - (int)('0');
        hour *= 10;
        hour += (int)(line[start + 1]) - (int)('0');
        position = start + 2;
    }
    else {
        return std::make_pair(std::make_pair(-1,-1),-1);
    }

    if (line[position] != ':')
        return std::make_pair(std::make_pair(-1,-1),-1);

    position++;
    if (!isNumber(line[position]) || !isNumber(line[position + 1]))
        return std::make_pair(std::make_pair(-1,-1),-1);

    int minute = (int)(line[position]) - (int)('0');
    minute *= 10;
    minute += (int)(line[position + 1]) - (int)('0');
    position += 2;

    if (!areTramsWorking(hour, minute))
        return std::make_pair(std::make_pair(-1,-1),-1);

    return std::make_pair(std::make_pair(hour, minute), position);
}

pair<string, int> selectBusStop(string line, int position) {

    int length = 0;
    while (isLetter(line[position + length]) || line[position + length] == '_'
           || line[position + length] == '^')
        length ++;

    string busStopName = line.substr((size_t)(position), (size_t)(length));
    position += length;

    return std::make_pair(busStopName, position);
}

bool biggerTime(int hour, int minute, int pHour, int pMinute) {

    if (pHour > hour)
        return false;

    return !(pHour == hour && pMinute >= minute);
}

void loadNewRoute(string line, int numberOfLine) {

    pair<int, int> result = selectNumberOfRoute(line, 0);
    int numberOfRoute = result.first;
    int position = result.second;

    std::cout << "route nr " << numberOfRoute << "\n";

    vector<pair<pair<int, int>, string> > route;
    int prevHour = 0, prevMinute = 0;

    while (position < line.size()) {

        if (line[position] != ' ') {
            std::cerr << numberOfLine << " " << line << "\n";
            return;
        }
        position ++;

        pair<pair<int, int>, int> time = selectTime(line, position);
        if (time.first.first == -1) {
            std::cerr << numberOfLine << " " << line << "\n";
            return;
        }

        int hour = time.first.first, minute = time.first.second;
        position = time.second;

        if (line[position] != ' ' ||
            !biggerTime(hour, minute, prevHour, prevMinute)) {
            std::cerr << numberOfLine << " " << line << "\n";
            return;
        }
        prevHour = hour;
        prevMinute = minute;
        position ++;

        pair<string, int> busStop = selectBusStop(line, position);
        string busStopName = busStop.first;
        position = busStop.second;

        route.push_back(std::make_pair(std::make_pair(hour, minute), busStopName) );
        std::cout << hour << ":" << minute << " " << busStopName << " " << route.size() <<"\n";
    }
    routes.push_back(route);
    timetable[numberOfRoute] = &route;
    std::cout << routes.size() << " " << timetable.size() << "\n";

}

void loadNewTicket(string line, int numberOfLine) {
    std::cout << numberOfLine << ": loadNewRoute: " << line << "\n";
}

void loadNewQuestion(string line, int numberOfLine) {
    std::cout << numberOfLine << ": loadNewQuestion: " << line << "\n";
}

void loadData() {

    string line;
    int numberOfLine = 0;

    while (std::getline(std::cin, line)) {

        if (line.empty())
            continue;
        else if (isLetter(line[0]) || line[0] == ' ')
            loadNewTicket(line, numberOfLine);
        else if (isNumber(line[0]))
            loadNewRoute(line, numberOfLine);
        else if (line[0] == '?')
            loadNewQuestion(line, numberOfLine);
        else
            std::cerr << numberOfLine << " " << line << "\n";

        numberOfLine ++;
    }

}

//To pisałem ja

/** @brief Function finds start and final time of single tram ride.
 * @param startStop First stop of ride.
 * @param finalStop Last stop of ride.
 * @param routeNumber Number of tram route, which is used to operate this ride.
 * @return Pair of pairs structure.
 * Frist pair contains hour and minutes of departure if ride has been found,
 * @p IMPOSSIBLE_RIDE on both elements otherwise.
 * Second pair contains hour and minutes of arrival if ride has been found,
 * UB otherwise.
 */
pair <pair<int, int>, pair<int, int>> oneRouteRide(string * startStop,string * finalStop, int routeNumber) {
    if (timetable.count(routeNumber) > 0) {
       vector<pair<pair<int, int>, string> > route = *(timetable.at(routeNumber));
       pair<int, int> departure = pair(IMPOSSIBLE_RIDE,IMPOSSIBLE_RIDE);
       pair<int, int> arrival = pair(IMPOSSIBLE_RIDE, IMPOSSIBLE_RIDE);

       for (pair<pair<int,int>, string>& stop : route) {
           if (stop.second == *startStop)
               departure = stop.first;
           else if (stop.second == *finalStop)
               arrival = stop.first;
       }
       if (arrival.first == IMPOSSIBLE_RIDE)
           return pair <pair<int, int>, pair<int, int>>(arrival, departure);
       return pair <pair<int, int>, pair<int, int>>(departure, arrival);
    } else {
        pair<int, int> first = pair<int, int>(IMPOSSIBLE_RIDE, IMPOSSIBLE_RIDE);
        pair<pair<int, int>, pair<int, int>> result;
        result.first = first;
        return result;
    }
}

/** @brief Function which counts time difference in minutes between two times.
 * @param firstTime First time.
 * @param secondTime Second time.
 * @return Time difference in minutes between times above.
 */

int timeDifference(pair<int, int> * firstTime, pair<int, int> * secondTime){
    int minutes = 0;
    minutes += ((*secondTime).first - (*firstTime).first) * MINUTES_PER_HOUR;
    return minutes + (*secondTime).second - (*firstTime).second;
}

/** @brief Function checks duration of entire travel.
 * @param ride Vector, which contains considered ride.
 * @return Pair structure, which contains:
 * On its 1. field duration of ride if its possible, @p IMPOSSIBLE_RIDE otherwise.
 * On its 2. field name of the stop on which passenger must wait, if such exists; otherwise empty string.
 */
pair<int, string> rideTime(list<pair<string, int>> * ride) {
    int duration = 0;
    string stopOfBreak;
    pair<string, int> firstStop, secondStop = pair<string, int>("", IMPOSSIBLE_RIDE);
    pair<int, int> departure, arrival;


    for (pair<string, int>& stop : (*ride)) {
        firstStop = secondStop;
        secondStop = stop;
        if (firstStop.second == IMPOSSIBLE_RIDE)
            continue;

        pair<pair<int, int>, pair<int, int>> passageTime = oneRouteRide(
                &firstStop.first, &secondStop.first, firstStop.second);

        departure = passageTime.first;

        if (departure.first == IMPOSSIBLE_RIDE || biggerTime(arrival.first, arrival.second, departure.first, departure.second))
            return pair<int, string>(IMPOSSIBLE_RIDE, "");
        else if (biggerTime(departure.first, departure.second, arrival.first, arrival.second))
            return pair<int, string>(IMPOSSIBLE_RIDE, firstStop.first);

        arrival = passageTime.second;
        duration += timeDifference(&departure, &arrival);
    }
    return pair<int, string> (duration, "");
}

double summaryPrice(vector<pair<string, pair<double, int>>> * tickets) {
    if ((*tickets).empty())
        return DBL_MAX;

    double sum = 0;
    for (pair<string, pair<double, int>>& ticket : *tickets) {
        sum += ticket.second.first;
    }
    return sum;
}

vector <pair<string, pair<double, int>>> * chooseBest(vector <pair<string, pair<double, int>>> * v1,
        vector <pair<string, pair<double, int>>> * v2, vector <pair<string, pair<double, int>>> * v3){
    double price1 = summaryPrice(v1);
    double price2 = summaryPrice(v2);
    double price3 = summaryPrice(v3);
    double minValue = std::min(price1, price2, price3);

    if (minValue == DBL_MAX || minValue == price1)
        return v1;
    else if (minValue == price2)
        return v2;
    else
        return v3;
}

vector<pair<string, pair<double, int>>> bestSet(int time, vector<pair<string, pair<double, int>>> * tickets) {
    vector<pair<string, pair<double, int>>> optSolution[time][MAX_TICKETS];
//Przemyslec jak to lepiej zainicjalizować.

    for (int i = 0; i < time; i++) {
        for (int j = 0; j < MAX_TICKETS; j++) {
            optSolution[i][j] = vector<pair<string, pair<double, int>>>();
        }
    }
    for (int i = 0; i < time; i++) {
        for (pair<string, pair<double, int>>& ticket : *tickets) {

            if (ticket.second.second >= i + 1 && summaryPrice(&optSolution[i][0]) >= ticket.second.first) {
                optSolution[i][0].push_back(ticket);
            }
            if (i - ticket.second.second >= 0 && !optSolution[i - ticket.second.second][0].empty() &&
                    summaryPrice(&optSolution[i - ticket.second.second][0]) + ticket.second.first <=
                    summaryPrice(&optSolution[i][1])) {
                optSolution[i][1] = optSolution[i - ticket.second.second][0];
                optSolution[i][1].push_back(ticket);
            }
            if (i - ticket.second.second >= 0 && !optSolution[i - ticket.second.second][1].empty() &&
                summaryPrice(&optSolution[i - ticket.second.second][1]) + ticket.second.first <=
                summaryPrice(&optSolution[i][2])) {
                optSolution[i][2] = optSolution[i - ticket.second.second][1];
                optSolution[i][2].push_back(ticket);
            }
        }
    }
    return *chooseBest(&optSolution[time - 1][0], &optSolution[time - 1][1], &optSolution[time - 1][2]);
}

void displayResult(vector<pair<string, pair<double, int>>> * result) {
    if (result->empty()) {
        std::cout << ":-|" << std::endl;
    } else {
        std::cout << "! ";
        for (int i = 0; i < result->size(); i++) {
            if (i != 0)
                std::cout << ";";
            std::cout << (*result)[i].first;
        }
        std::cout << std::endl;
    }
}

int main() {

    vector<int> a = vector<int>();

    std::cout << a.empty();

    return 0;
}
