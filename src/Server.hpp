#pragma once

#include <iostream>
#include <thread>
#include <functional>
#include <vector>
#include <set>
#include <cstdint>
#include <cstdio>
#include <queue>
#include <mutex>
#include <string>
#include <condition_variable>
#include <future>


struct CmdDesc
{
    using SysTime = std::chrono::time_point<std::chrono::system_clock>;

    std::string cmd;
    SysTime start;
    std::thread::id thread_id;
    bool finished;
};

class Server
{
    using Duration = std::set<std::chrono::duration<double>>;
    using fp_t = std::function<CmdDesc(void)>;

  public:
	Server(size_t thread_cnt = 1);
	~Server();

	void AddCommand(const fp_t& op);
	void AddCommand(fp_t&& op);
    void UpdateStatistic(CmdDesc &cmdInfo);
    size_t GetCommandsCount();
    void ShowStatistic();

	Server(const Server& rhs) = delete;
	Server& operator=(const Server& rhs) = delete;
	Server(Server&& rhs) = delete;
	Server& operator=(Server&& rhs) = delete;

  private:
	std::mutex lock_;
	std::vector<std::thread> threads_;
	std::queue<fp_t> q_;
	std::condition_variable cv_;
	bool quit_ = false;
    size_t total_cmd_;
    std::set<std::thread::id> clients_;
    Duration lead_time_;

	void CommandHandler(void);
};
