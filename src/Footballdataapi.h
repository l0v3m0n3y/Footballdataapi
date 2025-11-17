#ifndef FOOTBALL_DATA_API_H
#define FOOTBALL_DATA_API_H

#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <pplx/pplx.h>
#include <iostream>
#include <string>
#include <vector>

using namespace web;
using namespace web::http;
using namespace web::http::client;

class Footballdataapi {
private:
    std::string api_base = "https://api.football-data.org/v4";
    http_client_config client_config;
    http_request create_request(const std::string& endpoint, const std::string& method, const std::string& data = "") {
        http_request request;
        
        if (method == "GET") {
            request.set_method(methods::GET);
        } else if (method == "POST") {
            request.set_method(methods::POST);
        } else if (method == "PUT") {
            request.set_method(methods::PUT);
        } else if (method == "DEL") {
            request.set_method(methods::DEL);
        }
        
        request.set_request_uri(utility::conversions::to_string_t(endpoint));
        
        // Set headers
        request.headers().add(U("Accept"), U("text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7"));
        request.headers().add(U("Host"), U("api.football-data.org"));
        request.headers().add(U("User-Agent"), U("Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/115.0"));
        
        if (!data.empty() && (method == "POST" || method == "PUT")) {
            request.set_body(data);
        }
        
        return request;
    }
    
    pplx::task<json::value> make_api_call(const std::string& endpoint, const std::string& method, const std::string& data = "") {
        http_client client(utility::conversions::to_string_t(api_base), client_config);
        auto request = create_request(endpoint, method, data);

        return client.request(request)
            .then([](http_response response) {
                if (response.status_code() == status_codes::OK) {
                    return response.extract_json();
                } else {
                    json::value error_obj;
                    error_obj[U("error")] = json::value::string(
                        U("HTTP Error: ") + utility::conversions::to_string_t(std::to_string(response.status_code())));
                    error_obj[U("success")] = json::value::boolean(false);
                    return pplx::task_from_result(error_obj);
                }
            })
            .then([](pplx::task<json::value> previousTask) {
                try {
                    return previousTask.get();
                } catch (const std::exception& e) {
                    json::value error_obj;
                    error_obj[U("error")] = json::value::string(
                        U("Exception: ") + utility::conversions::to_string_t(e.what()));
                    error_obj[U("success")] = json::value::boolean(false);
                    return error_obj;
                }
            });
    }

public:
    Footballdataapi(){
        client_config.set_validate_certificates(false);
    }

    // Areas
    pplx::task<json::value> get_areas() {
        return make_api_call("/areas","GET");
    }

    pplx::task<json::value> get_area_by_id(int area_id) {
        return make_api_call("/areas/" + std::to_string(area_id),"GET");
    }

    // Competitions
    pplx::task<json::value> get_competitions() {
        return make_api_call("/competitions","GET");
    }

    pplx::task<json::value> get_competition_by_id(const std::string& competition_id) {
        return make_api_call("/competitions/" + competition_id,"GET");
    }

    // Competition Standings
    pplx::task<json::value> get_competition_standings(const std::string& competition_id) {
        return make_api_call("/competitions/" + competition_id + "/standings","GET");
    }

    // Competition Matches
    pplx::task<json::value> get_competition_matches(const std::string& competition_id) {
        return make_api_call("/competitions/" + competition_id + "/matches","GET");
    }

    // Competition Teams
    pplx::task<json::value> get_competition_teams(const std::string& competition_id) {
        return make_api_call("/competitions/" + competition_id + "/teams","GET");
    }

    // Competition Scorers
    pplx::task<json::value> get_competition_scorers(const std::string& competition_id) {
        return make_api_call("/competitions/" + competition_id + "/scorers","GET");
    }

    // Teams
    pplx::task<json::value> get_teams() {
        return make_api_call("/teams","GET");
    }

    pplx::task<json::value> get_team_by_id(int team_id) {
        return make_api_call("/teams/" + std::to_string(team_id),"GET");
    }

    // Team Matches
    pplx::task<json::value> get_team_matches(int team_id) {
        return make_api_call("/teams/" + std::to_string(team_id) + "/matches","GET");
    }

    // Persons
    pplx::task<json::value> get_person_by_id(int person_id) {
        return make_api_call("/persons/" + std::to_string(person_id),"GET");
    }

    // Person Matches
    pplx::task<json::value> get_person_matches(int person_id) {
        return make_api_call("/persons/" + std::to_string(person_id) + "/matches","GET");
    }

    // Matches
    pplx::task<json::value> get_matches() {
        return make_api_call("/matches","GET");
    }

    pplx::task<json::value> get_match_by_id(int match_id) {
        return make_api_call("/matches/" + std::to_string(match_id),"GET");
    }

    // Match Head2Head
    pplx::task<json::value> get_match_head2head(int match_id) {
        return make_api_call("/matches/" + std::to_string(match_id) + "/head2head","GET");
    }
};

#endif
