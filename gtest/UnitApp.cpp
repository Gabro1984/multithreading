#include "Server.hpp"
#include <gtest/gtest.h>

struct ServerTest : public testing::Test
{
    Server *srv;

    void SetUp()
    {
        srv = new Server(2);
    }
    void TearDown() {
        delete srv;
    }
};

auto echo_handler = [](CmdDesc &request, std::shared_ptr<std::promise<CmdDesc>> promise) -> CmdDesc {
    CmdDesc response(request);

    promise->set_value(response);
    return response;
};

TEST_F(ServerTest, AddCommandTest)
{

    // Action
    using SysTime = std::chrono::time_point<std::chrono::system_clock>;
    CmdDesc reqCmd = {"test", SysTime(), std::thread::id(), true};
    auto promise = std::make_shared<std::promise<CmdDesc>>();

    srv->AddCommand(std::bind(echo_handler, reqCmd, promise));
    usleep(1000);

    // Expect
    EXPECT_EQ(srv->GetCommandsCount(), 1);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
