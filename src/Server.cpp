#include "Server.hpp"

Server::Server(size_t thread_cnt)
    : threads_(thread_cnt)
    , total_cmd_(0)
{
	for(size_t i = 0; i < threads_.size(); i++)
	{
		threads_[i] = std::thread(&Server::CommandHandler, this);
	}
}

Server::~Server()
{
	std::unique_lock<std::mutex> lock(lock_);
	quit_ = true;
	cv_.notify_all();
	lock.unlock();

	for(size_t i = 0; i < threads_.size(); i++)
	{
		if(threads_[i].joinable())
		{
			threads_[i].join();
		}
	}
    ShowStatistic();
}

// Add command to server queue
void Server::AddCommand(const fp_t& op)
{
	std::unique_lock<std::mutex> lock(lock_);
	q_.push(op);
	cv_.notify_one();
}

void Server::AddCommand(fp_t&& op)
{
	std::unique_lock<std::mutex> lock(lock_);
	q_.push(std::move(op));
	cv_.notify_one();
}

void Server::UpdateStatistic(CmdDesc &cmdInfo)
{
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<double> duration = now - cmdInfo.start;
    clients_.insert(cmdInfo.thread_id);
    lead_time_.insert(duration);
    total_cmd_++;
}

size_t Server::GetCommandsCount()
{
    return total_cmd_;
}

void Server::ShowStatistic()
{
    std::cout << "[SERVER] number of clients: " << clients_.size() << std::endl;
    std::cout << "[SERVER] process: " << GetCommandsCount() << " commands" <<  std::endl;
    auto max_duration = *std::prev(lead_time_.end());
    std::cout << "[SERVER] max command delay: " << max_duration.count() << " second" <<  std::endl;
}

void Server::CommandHandler(void)
{
	std::unique_lock<std::mutex> lock(lock_);

	do
	{
		cv_.wait(lock, [this] {
			return (q_.size() || quit_);
		});

		if(!quit_ && q_.size())
		{
			auto op = std::move(q_.front());
			q_.pop();

			lock.unlock();
			auto result = op();
			lock.lock();
            UpdateStatistic(result);
		}
	} while(!quit_);
}
