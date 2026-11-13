#ifndef CONNECTOR_H
#define CONNECTOR_H

// Talks to a UCI chess engine (e.g. Stockfish) over stdin/stdout pipes.
// Win32 and POSIX implementations share the same interface:
//   ConnectToEngine(path), getNextMove(moves), CloseConnection()

#include <string>

#ifdef _WIN32

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <cstring>
#include <vector>

namespace engine_detail {
    inline STARTUPINFOA sti = {0};
    inline SECURITY_ATTRIBUTES sats = {0};
    inline PROCESS_INFORMATION pi = {0};
    inline HANDLE pipin_w, pipin_r, pipout_w, pipout_r;
}

inline void ConnectToEngine(const char* path)
{
    using namespace engine_detail;
    pipin_w = pipin_r = pipout_w = pipout_r = NULL;
    sats.nLength = sizeof(sats);
    sats.bInheritHandle = TRUE;
    sats.lpSecurityDescriptor = NULL;

    CreatePipe(&pipout_r, &pipout_w, &sats, 0);
    CreatePipe(&pipin_r, &pipin_w, &sats, 0);

    sti.cb = sizeof(sti);
    sti.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    sti.wShowWindow = SW_HIDE;
    sti.hStdInput = pipin_r;
    sti.hStdOutput = pipout_w;
    sti.hStdError = pipout_w;

    std::vector<char> cmd(path, path + strlen(path) + 1); // CreateProcess may modify the command line
    CreateProcessA(NULL, cmd.data(), NULL, NULL, TRUE, 0, NULL, NULL, &sti, &pi);
}

inline std::string getNextMove(std::string position)
{
    using namespace engine_detail;
    std::string str;
    position = "position startpos moves " + position + "\ngo\n";

    DWORD writ, read, available;
    WriteFile(pipin_w, position.c_str(), (DWORD)position.length(), &writ, NULL);
    Sleep(500);

    char buffer[2048];
    PeekNamedPipe(pipout_r, buffer, sizeof(buffer), &read, &available, NULL);
    do
    {
        ZeroMemory(buffer, sizeof(buffer));
        if (!ReadFile(pipout_r, buffer, sizeof(buffer) - 1, &read, NULL) || !read) break;
        buffer[read] = 0;
        str += buffer;
    }
    while (read >= sizeof(buffer) - 1);

    size_t n = str.find("bestmove");
    if (n != std::string::npos) return str.substr(n + 9, 4);

    return "error";
}

inline void CloseConnection()
{
    using namespace engine_detail;
    DWORD writ;
    if (pipin_w != NULL) WriteFile(pipin_w, "quit\n", 5, &writ, NULL);
    if (pipin_w != NULL) CloseHandle(pipin_w);
    if (pipin_r != NULL) CloseHandle(pipin_r);
    if (pipout_w != NULL) CloseHandle(pipout_w);
    if (pipout_r != NULL) CloseHandle(pipout_r);
    if (pi.hProcess != NULL) CloseHandle(pi.hProcess);
    if (pi.hThread != NULL) CloseHandle(pi.hThread);
}

#else // POSIX (Linux, macOS)

#include <csignal>
#include <poll.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

namespace engine_detail {
    inline pid_t pid = -1;
    inline int to_engine = -1;   // write end -> engine stdin
    inline int from_engine = -1; // read end  <- engine stdout
}

inline void ConnectToEngine(const char* path)
{
    using namespace engine_detail;
    int in[2], out[2];
    if (pipe(in) != 0) return;
    if (pipe(out) != 0) { close(in[0]); close(in[1]); return; }

    std::signal(SIGPIPE, SIG_IGN); // don't die if the engine is missing

    pid = fork();
    if (pid == 0)
    {
        dup2(in[0], STDIN_FILENO);
        dup2(out[1], STDOUT_FILENO);
        dup2(out[1], STDERR_FILENO);
        close(in[0]); close(in[1]); close(out[0]); close(out[1]);
        execlp(path, path, (char*)nullptr);
        _exit(127); // exec failed
    }

    close(in[0]);
    close(out[1]);
    to_engine = in[1];
    from_engine = out[0];
}

inline std::string getNextMove(std::string position)
{
    using namespace engine_detail;
    if (to_engine < 0 || from_engine < 0) return "error";

    position = "position startpos moves " + position + "\ngo\n";
    if (write(to_engine, position.c_str(), position.length()) < 0) return "error";

    // Read engine output until a "bestmove" line arrives (or we time out).
    std::string str;
    char buffer[2048];
    const int timeoutMs = 5000;
    for (;;)
    {
        size_t n = str.find("bestmove");
        if (n != std::string::npos && str.find('\n', n) != std::string::npos)
            return str.substr(n + 9, 4);

        pollfd pfd{from_engine, POLLIN, 0};
        if (poll(&pfd, 1, timeoutMs) <= 0) break;
        ssize_t r = read(from_engine, buffer, sizeof(buffer));
        if (r <= 0) break;
        str.append(buffer, (size_t)r);
    }
    return "error";
}

inline void CloseConnection()
{
    using namespace engine_detail;
    if (to_engine >= 0)
    {
        if (write(to_engine, "quit\n", 5) < 0) { /* engine already gone */ }
        close(to_engine);
        to_engine = -1;
    }
    if (from_engine >= 0) { close(from_engine); from_engine = -1; }
    if (pid > 0) { waitpid(pid, nullptr, 0); pid = -1; }
}

#endif // _WIN32

#endif // CONNECTOR_H
