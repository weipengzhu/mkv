#pragma once

#include <memory>

#include "mkvproto/mkv.grpc.pb.h"

namespace mkv {
struct MkvServiceConfig {
    std::string server_address;
    std::size_t per_call_num;
    std::size_t concurrent_num;
};

namespace service {
class MkvService {
public:
    MkvService(MkvServiceConfig config) : config(config) {}

    bool start() {
        ServerBuilder<mkvproto::Mkv::AsyncService> server_builder;
        server_builder.listen_on(config.server_address);
        server_builder.set_concurrent(config.concurrent_num);
        server = server_builder.build();
        if (!server) {
            return false;
        }
        // call all request
        /*
         * for () {
         *  calls = build_calls();
         *  server->register_calls(calls);
         * }
         */
        server->start();
    }

    void shutdown() {
        if (server) {
            server->shutdown();
        }
    }
private:
    MkvServiceConfig config;
    std::unique_ptr<Server<mkvproto::Mkv::AsyncService>> server;
};
}
}
