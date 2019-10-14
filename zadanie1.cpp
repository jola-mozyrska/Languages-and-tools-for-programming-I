#include <string>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <unordered_map>
#include <vector>
#include <climits>
#include <regex>


//na set
// vector na mape
// komentarze
// zmienne w srodku
// long longi na const
// czekaj na false

std::unordered_map<std::string, long long> stop_name_to_stop_number;
std::unordered_map<long long, bool> present_tram_number;
std::vector<std::unordered_map<long long, int>> schedule_for_stops;

bool check_if_tram_at_stop(long long &tram_number, std::string &stop) {
    if (present_tram_number.count(tram_number) == 0)
        return false;

    auto stop_number = stop_name_to_stop_number[stop];
    auto schedule = schedule_for_stops[stop_number];
    if (schedule.count(tram_number) == 0)
        return false;

    return true;
}

void add_tram(long long &tram_number, std::vector<int> &schedule_time,
              std::vector<std::string> &tram_stops) {
    present_tram_number[tram_number] = true;

    for (long long i = 0; i < tram_stops.size(); ++i) {
        auto stop_name = tram_stops[i];
        auto stop_time = schedule_time[i];
        std::string destination = "*";
        if (i + 1 < tram_stops.size())
            destination = tram_stops[i + 1];

        long long stop_number = 0;
        if (stop_name_to_stop_number.count(stop_name) == 0) {
            stop_number = stop_name_to_stop_number.size();
            stop_name_to_stop_number[stop_name] = stop_number;
            schedule_for_stops.push_back({});
        }

        stop_number = stop_name_to_stop_number[stop_name];
        schedule_for_stops[stop_number][tram_number] = stop_time;
    }
}

std::vector<std::string> ticket_name;
std::vector<double> ticket_price;
std::vector<long long> ticket_time;

const int mx_time = 950;
const long long INF = std::numeric_limits<double>::infinity();
std::vector<long long> cost(mx_time, INF);
std::vector<long long> final_cost;
std::vector<std::vector<long long>> proposed_tickets(mx_time + 4);
std::vector<std::vector<long long>> final_proposed_tickets(mx_time + 4);

void add_ticket(std::string &name, long long &price, long long &minutes) {
    if (minutes > mx_time)
        minutes = mx_time;
    ticket_name.push_back(name);
    ticket_price.push_back(price);
    ticket_time.push_back(minutes);

    cost[0] = 0;

    long long ticket_nr = ticket_name.size() - 1;
    for (long long i = mx_time; i >= minutes; --i) {
        if (proposed_tickets[i - minutes].size() < 3) {
            if (cost[i] > cost[i - minutes] + price) {
                cost[i] = cost[i - minutes] + price;
                proposed_tickets[i] = proposed_tickets[i - minutes];
                proposed_tickets[i].push_back(ticket_nr);
            } else if (cost[i] == cost[i - minutes] + price
                       && proposed_tickets[i - minutes].size() + 1 <
                          proposed_tickets[i].size()) {

                proposed_tickets[i] = proposed_tickets[i - minutes];
                proposed_tickets[i].push_back(ticket_nr);
            }

        }
    }

    for (int i = mx_time - 1; i > 0; --i) {
        if (final_cost[i] < final_cost[i + 1]) {
            final_cost[i] = final_cost[i + 1];
            final_proposed_tickets[i] = final_proposed_tickets[i + 1];
        }
    }
}

bool ask_for_tickets(std::vector<std::string> &stops,
                     std::vector<long long> &trams_numbers) {
    long long n = trams_numbers.size();
    trams_numbers.push_back(trams_numbers[n - 1]);
    auto previous_tram = trams_numbers[0];
    auto first_stop_number = stop_name_to_stop_number[stops[0]];
    auto previous_departure_time = schedule_for_stops[first_stop_number][trams_numbers[0]];
    for (long long i = 0; i <= n; ++i) {
        auto stop_name = stops[i];
        auto stop_number = stop_name_to_stop_number[stop_name];
        auto arrival_time = schedule_for_stops[stop_number][previous_tram];
        auto departure_time = schedule_for_stops[stop_number][trams_numbers[i]];

        if (arrival_time < previous_departure_time)
            return false;

        // wypisac to tutaj, czy poczekac, czy nie rzuci false?
        if (arrival_time < departure_time) {
            std::cout << ":-( " << stop_name << "\n";
            return true;
        }

        if (arrival_time > departure_time)
            return false;

        previous_tram = trams_numbers[i];
        previous_departure_time = departure_time;
    }

    auto last_stop_number = stop_name_to_stop_number[stops[n]];
    auto first_time = schedule_for_stops[first_stop_number][trams_numbers[0]];
    auto last_time = schedule_for_stops[last_stop_number][trams_numbers[n]];
    auto summary_time = last_time - first_time + 1;

    std::vector<long long> final_tickets = proposed_tickets[summary_time];

    bool semicolon = false;
    std::cout << "!";
    for (auto id: final_tickets) {
        if (semicolon)
            std::cout << ";";
        else
            semicolon = true;
        std::cout << " " << ticket_name[id];
    }
    std::cout << "\n";

    return true;
}

//int main() {
//    long long x = 1;
//    std::string p = "b";
//
//    std::vector<int> schedule_time = {1, 2, 3};
//    std::vector<std::string> tram_stops = {"a", "b", "c"};
//    add_tram(x, schedule_time, tram_stops);
////std::cout << (int)check_if_tram_at_stop(x, p);
//
//    std::vector<std::string> stops = {"a", "c", "b"};
//    std::vector<long long> trams_numbers = {1, 1};
//    std::cout << ask_for_tickets(stops, trams_numbers);
//}

const int lower_time_limit = 355;
const int upper_time_limit = 1281;

const int minutes_in_hour = 60;
const int pennys_in_price_integral = 100;

const std::regex assistant_regex("[0-9]+|[a-zA-Z_^]+");
const std::regex question_regex("[?]\\s[a-zA-Z_^]+(?:\\s[0-9]+\\s[a-zA-Z_^]+)+");
const std::regex course_regex("[0-9]+(?:\\s[1-9]?[0-9]:[0-9]{2}\\s[a-zA-Z_^]+)+");
const std::regex ticket_regex(
        "([a-zA-Z]+(?:\\s[a-zA-Z]+)*)\\s([0-9]+)[.]([0-9]{2})\\s([0-9]+)");

void print_error(std::string line, int line_number) {
    std::cerr << "Error in line " << line_number << ": " << line << "\n";
}

int number_of_whitespaces(std::string line) {
    int counter = 0;

    for (size_t i = 0; i < line.size(); i++) {
        if (line[i] == ' ') {
            counter++;
        }
    }

    return counter;
}

int time_to_minutes(int hours, int minutes) {
    return hours * minutes_in_hour + minutes;
}

long long price_to_pennys(long price_integral, int price_fractional) {
    return price_integral * pennys_in_price_integral + price_fractional;
}

void question_about_ticket(std::string line, int line_number,
                           std::vector<std::string> &stops,
                           std::vector<long long> &trams_numbers) {
    bool error = false;
    int number_of_stops = 0;

    if (regex_match(line, question_regex)) {
        std::sregex_iterator i = std::sregex_iterator(line.begin(),
                                            line.end(), assistant_regex);
        number_of_stops = (number_of_whitespaces(line) + 1) / 2;
        stops = std::vector<std::string>(number_of_stops);
        trams_numbers = std::vector<long long>(number_of_stops - 1);
        int j = 0;
        stops.at(j) = (*i).str();
        i++;
        while ((i != std::sregex_iterator()) && !error) {
            trams_numbers.at(j) = stoll((*i).str());
            if (is_tram_number_correct(trams_numbers.at(j))) {
                i++;
                stops.at(j + 1) = (*i).str();
                if (!check_if_tram_at_stop(trams_numbers.at(j), stops.at(j))
                    || !check_if_tram_at_stop(trams_numbers.at(j),
                                              stops.at(j + 1))) {
                    error = true;
                }
                i++;
                j++;
            } else {
                error = true;
            }
        }
        if (!ask_for_tickets(stops, trams_numbers)) {
            error = true;
        }
    } else {
        error = true;
    }

    if (error) {
        print_error(line, line_number);
    }
}

void add_tram(std::string line, int line_number, long long &tram_number,
              std::vector<std::string> &stops, std::vector<int> &times) {
    bool error = false;
    int number_of_stops = 0;

    if (regex_match(line, course_regex)) {
        std::sregex_iterator i = std::sregex_iterator(line.begin(),
                                            line.end(), assistant_regex);
        tram_number = stoll((*i).str());
        i++;
        if (is_tram_number_correct(tram_number)) {
            number_of_stops = number_of_whitespaces(line) / 2;
            stops = std::vector<std::string>(number_of_stops);
            times = std::vector<int>(number_of_stops);
            int j = 0, current_time = 0, previous_time = 0, hour = 0;
            while ((i != std::sregex_iterator()) && !error) {
                hour = stoi((*i).str());
                i++;
                current_time = time_to_minutes(hour, stoi((*i).str()));
                if ((current_time >= lower_time_limit)
                    && (current_time <= upper_time_limit)) {
                    i++;
                    times.at(j) = current_time;
                    stops.at(j) = (*i).str();
                    i++;
                    j++;
                    if (current_time <= previous_time) {
                        error = true;
                    }
                    previous_time = current_time;
                } else {
                    error = true;
                }
            }
        } else {
            error = true;
        }
    } else {
        error = true;
    }

    if (error) {
        print_error(line, line_number);
    }
}

void add_ticket(std::string line, int line_number,
                std::string &ticket_name, long long &price, long long &validity) {
    std::smatch result;

    if (regex_match(line, result, ticket_regex)) {
        std::smatch::iterator i = result.begin();
        long price_integral;
        i++;
        ticket_name = *i;
        i++;
        price_integral = stol((*i).str());
        i++;
        price = price_to_pennys(price_integral, stoi((*i).str()));
        i++;
        validity = stoll(*i);
    } else {
        print_error(line, line_number);
    }
}

void choose_function(std::string line, int line_number) {
    char first_sign = line[0];
    std::string ticket_name = "";
    long long price = 0;
    long long validity = 0;
    long long tram_number = 0;
    std::vector<std::string> stops;
    std::vector<long long> trams_numbers;
    std::vector<int> times;

    if (first_sign == '?') {
        question_about_ticket(line, line_number, stops, trams_numbers);
    } else if ((first_sign >= '0') && (first_sign <= '9')) {
        add_tram(line, line_number, tram_number, stops, times);
    } else if (((first_sign >= 'A') && (first_sign <= 'Z'))
               || ((first_sign >= 'a') && (first_sign <= 'z'))) {
        add_ticket(line, line_number, ticket_name, price, validity);
    } else {
        print_error(line, line_number);
    }
}

int main() {
    std::string line;
    int line_number = 0;

    while (!getline(std::cin, line).eof()) {
        line_number++;
        if (!line.empty()) {
            choose_function(line, line_number);
        }
    }
}
