#include "Server.hpp"
#include "Client.hpp"
#include <algorithm>

static char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

static bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

int main(int argc, char * argv[])
{
    int jobs = 10;
    int clients = 2;
    int cmd_count = 10;
    const auto start = std::chrono::system_clock::now();

    if (cmdOptionExists(argv, argv+argc, "-h"))
    {
        std::cout << "Usage: ./test-app <option> <param>" << std::endl;
        std::cout << "Options:" << std::endl;
        std::cout << "  -h show help" << std::endl;
        std::cout << "  -c {number} set number of client threads" << std::endl;
        std::cout << "  -s {number} set number of server threads" << std::endl;
        std::cout << "  -l {length} set command length (for every client)" << std::endl;
        return 0;
    }

    char * clients_str = getCmdOption(argv, argv + argc, "-c");

    if (clients_str)
    {
        clients = atoi(clients_str);
    }

    char * jobs_str = getCmdOption(argv, argv + argc, "-s");

    if (jobs_str)
    {
        jobs = atoi(jobs_str);
    }

    char * cmd_count_str = getCmdOption(argv, argv + argc, "-l");

    if (cmd_count_str)
    {
        cmd_count = atoi(cmd_count_str);
    }

    auto srv = std::make_shared<Server>(jobs);
    Client(srv, clients, cmd_count);
    const auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "[APP] running time: " << duration.count() << " second" <<  std::endl;

    return 0;
}