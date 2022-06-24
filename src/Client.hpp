#pragma once

#include "Server.hpp"
#include <unordered_map>
#include <random>
#include <sstream>

class Client
{

  public:
    Client(const std::shared_ptr<Server> &server, size_t thread_cnt = 1, size_t cmd_cnt = 1);
	~Client();
    void Start(void);

private:
	std::shared_ptr<Server> server_;
    size_t cmd_count_;
    size_t cmd_finished_;
    std::vector<std::thread> threads_;
    std::stringstream statistic_;
    std::mutex lock_;

    void SourceHandler(std::vector<std::string> commands);
    size_t GetRandom(size_t min, size_t max);
    std::vector<std::string> CommandGenerator(size_t count);
};
