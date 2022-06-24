#include "Client.hpp"

auto handler = [](CmdDesc &request, std::shared_ptr<std::promise<CmdDesc>> promise) -> CmdDesc {
    CmdDesc response(request);
    const std::unordered_map<std::string, size_t> cmd_map = {
        {"type1", 1},
        {"type2", 2},
        {"type3", 3},
    };

    if (cmd_map.find(request.cmd) != cmd_map.end())
    {
        const auto delay = cmd_map.at(request.cmd);
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        response.finished = true;
        promise->set_value(response);
    }
    return response;
};

Client::Client(const std::shared_ptr<Server> &server, size_t thread_cnt, size_t cmd_cnt)
    : server_(server)
    , threads_(thread_cnt)
{
    std::vector<std::string> source = CommandGenerator(cmd_cnt);
    std::cout <<"Running test..." << std::endl;

	for(size_t i = 0; i < threads_.size(); i++)
	{
        threads_[i] = std::thread([=] { SourceHandler(source); });
	}
}

Client::~Client()
{
    // Wait for threads to finish before we exit
	for(size_t i = 0; i < threads_.size(); i++)
	{
		if(threads_[i].joinable())
		{
			threads_[i].join();
		}
    }
    std::cout << statistic_.str();
}

void Client::SourceHandler(std::vector<std::string> commands)
{
    std::vector<std::future<CmdDesc>> futureResults;
    auto th_id = std::this_thread::get_id();
    size_t total_commands{0}, processed_commands{0};

    for (auto&& cmd : commands)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        const auto cur_time = std::chrono::system_clock::now();
        CmdDesc reqCmd = {cmd, cur_time, th_id, false};
        auto promise = std::make_shared<std::promise<CmdDesc>>();
        futureResults.push_back(promise->get_future());
        server_->AddCommand(std::bind(handler, reqCmd, promise));
        total_commands++;
    }

    for(size_t i = 0; i < futureResults.size(); i++)
    {
        auto fval = futureResults[i].get();
        processed_commands++;
    }

    std::lock_guard<std::mutex> guard(lock_);
    statistic_ << "[CLIENT id:" << th_id << "] "
              <<  "total " << total_commands
              << " processed " << processed_commands <<  std::endl;
}

size_t Client::GetRandom(size_t min, size_t max)
{
    std::vector<std::string> out;
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> result(min, max);

    return result(rng);
}

std::vector<std::string> Client::CommandGenerator(size_t count)
{
    std::vector<std::string> out;
    const std::unordered_map<size_t, std::string> type_map = {
        {1, "type1"},
        {2, "type2"},
        {3, "type3"},
    };

    for (size_t i = 0; i < count; i++)
    {
        auto rnum = GetRandom(1, type_map.size());

        if (type_map.find(rnum) != type_map.end())
        {
            const auto cmd = type_map.at(rnum);
            out.push_back(cmd);
        }
    }

    return out;
}
