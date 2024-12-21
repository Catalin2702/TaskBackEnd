//
// Created by Catalin Chirosca on 16/12/24.
//

#ifndef PARAMS_HPP
#define PARAMS_HPP

#define DB_ENGINE "postgresql"
#define DB_HOST "localhost"
#define DB_PORT "5432"
#define DB_NAME "task"
#define DB_USER "task_admin"
#define DB_PASS "task_admin"

#define SERVER_URL "0.0.0.0"
#define SERVER_PORT 3000

#define DB_URI DB_ENGINE "://" DB_USER ":" DB_PASS "@" DB_HOST ":" DB_PORT "/" DB_NAME

#endif //PARAMS_HPP
