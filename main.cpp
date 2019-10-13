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


/**@brief check is the sign a letter
 * The function check is the sign a letter from English alphabet
 * @param sign - sign to check
 * @return @p true if the sign is a letter, @p false otherwise
 */
bool isLetter(char sign) {
    if ((int) (sign) >= (int) ('a') && (int) (sign) <= (int) ('z'))
        return true;
    else return ((int) (sign) >= (int) ('A') && (int) (sign) <= (int) ('Z'));
}

/**@brief check is the sign a cipher
 * The function check is the sign a cipher
 * @param sign - sign to check
 * @return @p true if the sign is a cipher, @p false otherwise
 */
bool isNumber(char sign) {
    return ((int)(sign) >= (int)('0') && (int)(sign) <= (int)('9'));
}

/**@brief select route number from text from a given place
 * The function selects route number from text from a given place. It should be
 * not-negative integer. It may have '0' on the beginning which is ignored.
 * @param line - text to read in
 * @param start - place where selecting starts
 * @return pair structure, which contains route number and position
 * in text after the end of this information
 */
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

/**@brief select from text from a given place how long the ticket is valid
 * The function selects from text from a given place how long the ticket is valid.
 * It should be positive integer without '0' on the beginning.
 * @param line - text to read in
 * @param position - place where selecting starts
 * @return pair structure, which contains the number how long the ticket is valid
 * and position in text after the end of this information
 */
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

/**@brief check are trams working at a given time
 * The function check are trams working at a given time. It means between 5:55
 * and 21:21 (including them).
 * @param hour - hours of time
 * @param minute - minutes of time
 * @return @p true if trams are working, @p false otherwise
 */
bool areTramsWorking (int hour, int minute) {

    if (hour < 5 || hour > 21)
        return false;

    if (minute < 0 || minute > 59)
        return false;

    if (hour == 5 && minute < 55)
        return false;

    return !(hour == 21 && minute > 21);
}

/**@brief select time from text from a given place
 * The function select time from text from a given place. Time should has format hh:mm
 * or h:mm, where 'h' means hour cipher and 'm' means minute cipher. If the hour has only
 * one cipher it should be h:mm and if has two ciphers hh:mm.
 * @param line - text to select from
 * @param start - place where selecting starts
 * @return pair structure, which contains next pair (which contains hour and minute
 * of selected time) and position in text after the end of this information
 */
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

/**@brief select price from text from a given place
 * The function select price with two decimal places (after '.') from text.
 * from a given place.
 * @param line - text to select from
 * @param start - place where selecting starts
 * @return pair structure, which contains price and position
 * in text after the end of this information
 */
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

/**@brief select bus stop name from text from a given place
 * The function select bus stop name from text from a given place.
 * The name consist of letters from an English alphabet, sign '_'
 * and '^'. It can't be empty.
 * @param line - text to select from
 * @param position - place where selecting starts
 * @return pair structure, which contains bus stop name and
 * position in text after the end of this information
 */
pair<string, int> selectBusStop(string line, int position) {

    int length = 0;
    while (isLetter(line[position + length]) || line[position + length] == '_'
           || line[position + length] == '^')
        length ++;

    string busStopName = line.substr((size_t)(position), (size_t)(length));
    position += length;

    return make_pair(busStopName, position);
}

/**@brief select ticket name from text from a given place
 * The function select ticket name from text from a given place. Name should
 * consist of letters from English alphabet and signs ' '. It can't be empty.
 * @param line - text to select from
 * @param position - place where selecting starts
 * @return pair structure, which contains ticket name and position in text
 * after the end of this information
 */
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

/**@brief check is the first time later than the second
 * The function compare two times and check is the first
 * time later than the second one.
 * @param hour - hour of first time
 * @param minute - minute of first time
 * @param pHour - hour of second time (should be previous)
 * @param pMinute - minute of second time (should be previous)
 * @return @p true if the first time is later than the second,
 * @p false otherwise
 */
bool biggerTime(int hour, int minute, int pHour, int pMinute) {

    if (pHour > hour)
        return false;

    return !(pHour == hour && pMinute >= minute);
}

/**@brief check has route with given number already exist
 * The function check has route with given number already exist.
 * @param numberOfRoute - route number
 * @param timetable - reference to map with all previous routes
 * @return @p true if the route with the given number already exist,
 * @p false otherwise
 */
bool routeAlreadyExist(int numberOfRoute,
                       map<int, vector<pair<pair<int, int>, string> > >* timetable) {

    map<int, vector<pair<pair<int, int>, string> > >::iterator it;

    for (it = (*timetable).begin(); it != (*timetable).end(); ++it) {
        if ((*it).first == numberOfRoute)
            return true;
    }
    return false;
}

/**@brief check has the ticket with given name already exist
 * The function check has the ticket with given name already exist.
 * @param ticketName - ticket name
 * @param tickets - reference to vector with all previous tickets
 * @return @p true if the ticket with the given name already exist,
 * @p false otherwise
 */
bool ticketAlreadyExist(string * ticketName,
                        vector<pair<string, pair<double, int> > >* tickets) {

    for (pair<string, pair<double, int> >& ticket : *tickets) {
        if (ticket.first == (*ticketName))
            return true;
    }
    return false;
}

/**@brief check is the route visiting given bus stop more than once
 * The function check is the given bus stop visited for the second
 * time by the same route
 * @param busStopName - given bus stop
 * @param route - vector with previous way of the route
 * @return @true if the stop is visiting by the second time, @p false otherwise
 */
bool busStopRevisited(string * busStopName,
                      vector<pair<pair<int, int>, string> > route) {

    for (pair<pair<int, int>, string>& busStop : route) {
        if (busStop.second == (*busStopName))
            return true;
    }
    return false;
}

/**@brief analyzing the line is it the correct form to add new route
 * The function analyze has the line correct form and if has add new
 * route to timetable.
 * @param line - text with input
 * @param numberOfLine - number of line in input
 * @param timetable - reference to map where the routes are adding
 */
void loadNewRoute(string line, int numberOfLine,
                  map<int, vector<pair<pair<int, int>, string> > >* timetable) {

    pair<int, int> routeNumber = selectRouteNumber(line, 0);
    int numberOfRoute = routeNumber.first;
    int position = routeNumber.second;

    if (routeAlreadyExist(numberOfRoute, timetable)) {
        cerr << numberOfLine << " " << line << "\n";
        return;
    }

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

        if (busStopName == "" ||
            busStopRevisited(&busStopName, route)) {
            cerr << numberOfLine << " " << line << "\n";
            return;
        }

        route.emplace_back(make_pair(make_pair(hour, minute), busStopName));
    }

    if (route.empty()) {
        cerr << numberOfLine << " " << line << "\n";
        return;
    }

    (*timetable)[numberOfRoute] = route;
}

/**@brief analyzing the line is it the correct form to add new ticket
 * The function analyze has the line correct form and if has add new
 * ticket to vector.
 * @param line - text with input
 * @param numberOfLine - number of line in input
 * @param tickets - vector with all tickets
 */
void loadNewTicket(string line, int numberOfLine,
                   vector<pair<string, pair<double, int> > >* tickets) {

    pair<string, int> name = selectTicketName(line, 0);
    string ticketName = name.first;
    int position = name.second;

    if (line[position] != ' ' || ticketAlreadyExist(&ticketName, tickets)) {
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
    tickets -> emplace_back(make_pair(ticketName, make_pair(price, validityTime)));
}


/** @brief Function, which reads line started with '?' sign. (Inquiry line)
 * @param line String containing line of input started with '?'.
 * @param numberOfLine Number of line, counting started at 1.
 * @param question Pointer to list, which will be filled in with input line
 * Fill in pattern: list<pair<stopName, routeNumber>>. On the last element routeNumber
 * equals @p IMPOSSIBLE_RIDE.
 * @return @p true if error was signaled on cerr, @p false otherwise.
 */
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
             question -> emplace_back(make_pair(element.first, IMPOSSIBLE_RIDE)); //last element
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
    pair<string, int> firstStop, secondStop = make_pair("", IMPOSSIBLE_RIDE);
    pair<int, int> departure, arrival;
    arrival = make_pair(IMPOSSIBLE_RIDE, IMPOSSIBLE_RIDE);

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
        else if (biggerTime(departure.first, departure.second, arrival.first, arrival.second) &&
                arrival.first != IMPOSSIBLE_RIDE) {

            cout << departure.first << departure.second << std::endl;
            cout << arrival.first << arrival.second << std::endl;

            return pair<int, string>(IMPOSSIBLE_RIDE, firstStop.first);
        }

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
vector <pair<string, pair<double, int>>> * chooseBest(
        vector <pair<string, pair<double, int>>> * v1,
        vector <pair<string, pair<double, int>>> * v2,
        vector <pair<string, pair<double, int>>> * v3) {
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

/** @brief Function writing on standart output names of tickets to buy.
 * If it is impossible to buy tickets, writes ":-|".
 * @param result Pointer to vector with the best tickets set.
 * Empty vector if such set does not exist.
 */
size_t displayResult(vector<pair<string, pair<double, int>>> result) {
    if (result.empty()) {
        std::cout << ":-|" << std::endl;
        return 0;
    } else {
        std::cout << "! ";
        for (int i = 0; i < result.size(); i++) {
            if (i != 0)
                std::cout << ";";
            std::cout << result[i].first;
        }
        std::cout << std::endl;
        return result.size();
    }
}


/** @brief Modified knapsack problem alghoritm, finds the best set of tickets for given ride time.
 * @param time Demanded ticket validity length.
 * @param tickets Pointer to avaliable tickets, kept in vector.
 * @return Vector with the best set if such exists.
 * Empty vector otherwise.
 */
vector<pair<string, pair<double, int>>> bestSet(int time, vector<pair<string,
        pair<double, int>>> * tickets) {
    //Second table dimension equals 3, because of such tickets amount limitation.
    vector<pair<string, pair<double, int>>> optSolution[time][3];

    for (int i = 0; i < time; i++) {
        for (pair<string, pair<double, int>>& ticket : *tickets) {

            if (ticket.second.second >= i + 1 && summaryPrice(&optSolution[i][0]) >= ticket.second.first) {
                optSolution[i][0].clear();
                optSolution[i][0].push_back(ticket);
            }
            if (i - ticket.second.second >= 0 && !optSolution[i - ticket.second.second][0].empty() &&
                    summaryPrice(&optSolution[i - ticket.second.second][0]) + ticket.second.first <=
                    summaryPrice(&optSolution[i][1])) {
                optSolution[i][1].clear();
                optSolution[i][1] = optSolution[i - ticket.second.second][0];
                optSolution[i][1].push_back(ticket);
            }
            if (i - ticket.second.second >= 0 && !optSolution[i - ticket.second.second][1].empty() &&
                    summaryPrice(&optSolution[i - ticket.second.second][1]) + ticket.second.first <=
                    summaryPrice(&optSolution[i][2])) {
                optSolution[i][2].clear();
                optSolution[i][2] = optSolution[i - ticket.second.second][1];
                optSolution[i][2].push_back(ticket);
            }
        }
    }
    return *chooseBest(&optSolution[time - 1][0], &optSolution[time - 1][1], &optSolution[time - 1][2]);
}

/** @brief Inquiry about the best tickets set.
 * @param ride Pointer to list containing ride scheme.
 * @param tickets Pointer to tickets pricelist.
 * @param timetable Pointer to trams timetable.
 * @return @p true, if result has been displayed.
 * @p false, if not, due to impossible purchase of tickets or
 * ircorrect ride scheme.
 */
bool ticketsInquiry(list<pair<string, int>> * ride, size_t * ticketsAmount,
        vector<pair<string, pair<double, int>>> * tickets,
        map<int, vector<pair<pair<int, int>, string>>> * timetable) {

    pair<int, string> time = rideTime(ride, timetable);

    if (time.first == IMPOSSIBLE_RIDE) {
        if (time.second.length() > 0)
            cout << ":-( " << time.second << std::endl;
        else
            return false;
    } else {
        (*ticketsAmount) += displayResult(bestSet(time.first, tickets));
    }
    return true;
}

/** @brief Function which reads whole input and realize all instructions.
 * @param timetable Pointer to trams timetable.
 * @param tickets Pointer to tickets pricelist.
 */
void reactOnInput(map<int, vector<pair<pair<int, int>, string> > >* timetable,
              vector<pair<string, pair<double, int> > >* tickets) {

    string line;
    int numberOfLine = 1;
    size_t ticketsAmount = 0;

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
                if (!ticketsInquiry(&question, &ticketsAmount, tickets, timetable))
                    cerr << numberOfLine << " " << line << "\n";
            }
        } else
            cerr << numberOfLine << " " << line << "\n";

        numberOfLine ++;
    }
    cout << ticketsAmount << std::endl;
}

int main() {

    map<int, vector<pair<pair<int, int>, string> > > timetable;
    vector<pair<string, pair<double, int> > > tickets;

    reactOnInput(&timetable, &tickets);

    return 0;
}
