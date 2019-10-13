#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <cfloat>

using std::string;
using std::vector;
using std::map;
using std::pair;
using std::make_pair;
using std::cout;
using std::cin;
using std::cerr;
using std::list;

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

pair<int, int> selectRouteNumber(string line, int start) {

    int number = (int)line[start] - (int)('0');
    int position = start + 1;

    while (position < line.size() && isNumber(line[position])) {
        number *= 10;
        number += (int)(line[position]) - (int)('0');
        position++;
    }

    return make_pair(number, position);
}

pair<int, int> selectValidityTime(string line, int position) {

    if (!isNumber(line[position]) || line[position] == '0')
        return make_pair(-1, -1);
    int number = (int)(line[position]) - (int)('0');
    position ++;

    while (position < line.size() && isNumber(line[position])) {
        number *= 10;
        number += (int)(line[position]) - (int)('0');
        position++;
    }

    return make_pair(number, position);
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
        return make_pair(make_pair(-1,-1),-1);

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
        return make_pair(make_pair(-1,-1),-1);
    }

    if (line[position] != ':')
        return make_pair(make_pair(-1,-1),-1);

    position++;
    if (!isNumber(line[position]) || !isNumber(line[position + 1]))
        return make_pair(make_pair(-1,-1),-1);

    int minute = (int)(line[position]) - (int)('0');
    minute *= 10;
    minute += (int)(line[position + 1]) - (int)('0');
    position += 2;

    if (!areTramsWorking(hour, minute))
        return make_pair(make_pair(-1,-1),-1);

    return make_pair(make_pair(hour, minute), position);
}

pair<double, int> selectPrice(string line, int start) {

    int position = start;
    double price = 0;
    while (isNumber(line[position])) {
        price *= 10;
        price += (double)(line[position]) - (double)('0');
        position ++;
    }

    if (position == start || line[position] != '.')
        return make_pair(-1,-1);
    position ++;

    for (int converter = 10; converter < 101; converter += 90) {
        if (!isNumber(line[position]))
            return make_pair(-1, -1);
        price += ((double) (line[position]) - (double) ('0')) / (double)(converter);
        position ++;
    }

    return make_pair(price, position);
}

pair<string, int> selectBusStop(string line, int position) {

    int length = 0;
    while (isLetter(line[position + length]) || line[position + length] == '_'
           || line[position + length] == '^')
        length ++;

    string busStopName = line.substr((size_t)(position), (size_t)(length));
    position += length;

    return make_pair(busStopName, position);
}

pair<string, int> selectTicketName(string line, int position) {

    int length = 0;
    while (isLetter(line[position + length]) || line[position + length] == ' ')
        length ++;

    length --;
    //the last sign should be ' ' between ticket name and price

    string ticketName = line.substr((size_t)(position), (size_t)(length));
    position += length;

    return make_pair(ticketName, position);
}

bool biggerTime(int hour, int minute, int pHour, int pMinute) {

    if (pHour > hour)
        return false;

    return !(pHour == hour && pMinute >= minute);
}

void loadNewRoute(string line, int numberOfLine,
                  map<int, vector<pair<pair<int, int>, string> > >* timetable) {

    pair<int, int> routeNumber = selectRouteNumber(line, 0);
    int numberOfRoute = routeNumber.first;
    int position = routeNumber.second;

    cout << "route nr " << numberOfRoute << "\n";

    vector<pair<pair<int, int>, string> > route;
    int prevHour = 0, prevMinute = 0;

    while (position < line.size()) {

        if (line[position] != ' ') {
            cerr << numberOfLine << " " << line << "\n";
            return;
        }
        position ++;

        pair<pair<int, int>, int> time = selectTime(line, position);
        if (time.first.first == -1) {
            cerr << numberOfLine << " " << line << "\n";
            return;
        }

        int hour = time.first.first, minute = time.first.second;
        position = time.second;

        if (line[position] != ' ' ||
            !biggerTime(hour, minute, prevHour, prevMinute)) {
            cerr << numberOfLine << " " << line << "\n";
            return;
        }
        prevHour = hour;
        prevMinute = minute;
        position ++;

        pair<string, int> busStop = selectBusStop(line, position);
        string busStopName = busStop.first;
        position = busStop.second;

        route.push_back(make_pair(make_pair(hour, minute), busStopName));
        cout << hour << ":" << minute << " " << busStopName <<"\n";
    }
    (*timetable)[numberOfRoute] = route;
}

void loadNewTicket(string line, int numberOfLine,
                   vector<pair<string, pair<double, int> > >* tickets) {

    pair<string, int> name = selectTicketName(line, 0);
    string ticketName = name.first;
    int position = name.second;

    if (line[position] != ' ') {
        cerr << numberOfLine << " " << line << "\n";
        return;
    }
    position ++;

    pair<double, int> priceResult = selectPrice(line, position);
    double price = priceResult.first;
    position = priceResult.second;

    if (line[position] != ' ' || price == -1) {
        cerr << numberOfLine << " " << line << "\n";
        return;
    }
    position ++;

    pair<int, int> validity = selectValidityTime(line, position);
    int validityTime = validity.first;
    position = validity.second;

    if (position != line.size() || validityTime == 0) {
        cerr << numberOfLine << " " << line << "\n";
        return;
    }

    cout<< ticketName << " " << price << " " << validityTime << "\n";
    (*tickets).push_back(make_pair(ticketName, make_pair(price, validityTime)));
}

bool loadNewQuestion(string line, int numberOfLine, list<pair<string, int>> * question) {
     int position = 2;

     if (line.length() <= 2 || line[1] != ' ') {
         cerr << numberOfLine << " " << line << std::endl;
         return true;
     }

     while (position < line.length()) {
         pair<string, int> element = selectBusStop(line, position);

         if ((position = element.second) == line.length() ) {
             if (question -> empty()) {
                 cerr << numberOfLine << " " << line << std::endl;
                 return true;
             }
             question -> emplace_back(make_pair(element.first, IMPOSSIBLE_RIDE));
             return false;
         }

         if (element.second == 0 || line[position] != ' ' ||
                position == line.length() - 1) {
             cerr << numberOfLine << " " << line << std::endl;
             return true;
         } else {
             position++;
         }

         pair<int, int> route = selectRouteNumber(line, position);

         if (route.second == 0 || line[position = route.second] != ' ' ||
                position >= line.length() - 1) {
             cerr << numberOfLine << " " << line << std::endl;
             return true;
         } else {
             position++;
         }
         question -> emplace_back(element.first, route.first);
     }
     return false;
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
pair <pair<int, int>, pair<int, int>> oneRouteRide(string * startStop,string * finalStop,
        int routeNumber, map<int, vector<pair<pair<int, int>, string>>> * timetable) {
    if (timetable -> count(routeNumber) > 0) {
       vector<pair<pair<int, int>, string> > route = (*timetable)[routeNumber];
       pair<int, int> departure = make_pair(IMPOSSIBLE_RIDE,IMPOSSIBLE_RIDE);
       pair<int, int> arrival = make_pair(IMPOSSIBLE_RIDE, IMPOSSIBLE_RIDE);

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
    minutes += (secondTime -> first - firstTime -> first) * MINUTES_PER_HOUR;
    return minutes + (secondTime -> second - firstTime -> second);
}

/** @brief Function checks duration of entire travel.
 * @param ride Vector, which contains considered ride.
 * @return Pair structure, which contains:
 * On its 1. field duration of ride if its possible, @p IMPOSSIBLE_RIDE otherwise.
 * On its 2. field name of the stop on which passenger must wait, if such exists; otherwise empty string.
 */
pair<int, string> rideTime(list<pair<string, int>> * ride,
        map<int, vector<pair<pair<int, int>, string>>> * timetable) {
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
                &firstStop.first, &secondStop.first, firstStop.second, timetable);

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

/** @brief Counts sum of prices of tickets.
 * @param tickets Vector containing tickets.
 * Ticket representation: <pair<ticket_name, pair<price, validity_time>>>
 * @return Sum of tickets' prices in vector if it contains any ticket.
 * @p DBL_MAX if vector is empty();
 */
double summaryPrice(vector<pair<string, pair<double, int>>> * tickets) {
    if (tickets -> empty())
        return DBL_MAX;

    double sum = 0;
    for (pair<string, pair<double, int>>& ticket : *tickets) {
        sum += ticket.second.first;
    }
    return sum;
}

/** @brief Function chooses the cheapest set of tickets.
 * @param v1 Pointer to first ticket set.
 * @param v2 Pointer to second ticket set.
 * @param v3 Pointer to third ticket set.
 * @return Pointer to the best set if at least one ticket set is not empty.
 * Empty vector pointer otherwise.
 */
vector <pair<string, pair<double, int>>> * chooseBest(vector <pair<string, pair<double, int>>> * v1,
        vector <pair<string, pair<double, int>>> * v2, vector <pair<string, pair<double, int>>> * v3) {
    double price1 = summaryPrice(v1);
    double price2 = summaryPrice(v2);
    double price3 = summaryPrice(v3);
    double minValue = std::min(price1, price2);
    minValue = std::min(minValue, price3);

    if (minValue == DBL_MAX || minValue == price1)
        return v1;
    else if (minValue == price2)
        return v2;
    else
        return v3;
}

/** @brief Modified knapsack problem alghoritm, finds the best set of tickets for given ride time.
 * @param time Demanded ticket validity length.
 * @param tickets Pointer to avaliable tickets, kept in vector.
 * @return Pointer to the best set if such exists.
 * Empty vector pointer otherwise.
 */
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

/** @brief Function writing on standart output names of tickets to buy.
 * If it is impossible to buy tickets, writes ":-|".
 * @param result Pointer to vector with the best tickets set.
 * Empty vector if such set does not exist.
 */
void displayResult(vector<pair<string, pair<double, int>>> result) {
    if (result.empty()) {
        std::cout << ":-|" << std::endl;
    } else {
        std::cout << "! ";
        for (int i = 0; i < result.size(); i++) {
            if (i != 0)
                std::cout << ";";
            std::cout << result[i].first;
        }
        std::cout << std::endl;
    }
}

/** @brief Inquiry about the best tickets set.
 * @param ride Pointer to list containing ride scheme.
 * @param tickets Pointer to tickets pricelist.
 * @param timetable Pointer to trams timetable.
 * @return @p true, if result has been displayed.
 * @p false, if not, due to impossible purchase of tickets or
 * ircorrect ride scheme.
 */
bool ticketsInquiry(list<pair<string, int>> * ride,
        vector<pair<string, pair<double, int>>> * tickets,
        map<int, vector<pair<pair<int, int>, string>>> * timetable) {

    pair<int, string> time = rideTime(ride, timetable);

    if (time.first == IMPOSSIBLE_RIDE) {
        if (time.second.length() > 0)
            cout << ":-( " << time.second << std::endl;
        else
            return false;
    } else {
        displayResult(bestSet(time.first, tickets));
    }
    return true;
}

void loadData(map<int, vector<pair<pair<int, int>, string> > >* timetable,
              vector<pair<string, pair<double, int> > >* tickets) {

    string line;
    int numberOfLine = 1;

    while (getline(cin, line)) {

        if (line.empty())
            continue;
        else if (isLetter(line[0]) || line[0] == ' ')
            loadNewTicket(line, numberOfLine, tickets);
        else if (isNumber(line[0]))
            loadNewRoute(line, numberOfLine, timetable);
        else if (line[0] == '?') {
            list<pair<string, int>> question;
            bool err = loadNewQuestion(line, numberOfLine, &question);
            if (!err) {
                if (!ticketsInquiry(&question, tickets, timetable))
                    cerr << numberOfLine << " " << line << "\n";
            }
        } else
            cerr << numberOfLine << " " << line << "\n";

        numberOfLine ++;
    }

}

int main() {

    map<int, vector<pair<pair<int, int>, string> > > timetable;
    vector<pair<string, pair<double, int> > > tickets;

    loadData(&timetable, &tickets);
    return 0;
}
