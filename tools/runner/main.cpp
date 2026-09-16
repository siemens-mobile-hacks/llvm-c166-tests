#include <algorithm>
#include <cctype>
#include <cerrno>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

namespace fs = std::filesystem;

static std::string require(const std::map<std::string, std::string> &args,
                           const std::string &name) {
  auto value = args.find(name);
  if (value == args.end())
    throw std::runtime_error("missing argument " + name);
  return value->second;
}

static std::string windowsPath(const fs::path &path) {
  std::string result = "Z:" + fs::absolute(path).string();
  std::replace(result.begin(), result.end(), '/', '\\');
  return result;
}

static bool startsWith(const std::string &value, const std::string &prefix) {
  return value.compare(0, prefix.size(), prefix) == 0;
}

static bool endsWith(const std::string &value, const std::string &suffix) {
  return value.size() >= suffix.size() &&
         value.compare(value.size() - suffix.size(), suffix.size(), suffix) ==
             0;
}

static void configureIni(const fs::path &source, const fs::path &destination,
                         const fs::path &terminalLog) {
  std::ifstream input(source);
  std::ofstream output(destination);
  if (!input || !output)
    throw std::runtime_error("cannot configure CrossView ini file");

  const std::string oldPrefix = "terminal.Default.";
  const std::string newPrefix = "terminal.ASC0 transmission/reception.";
  const std::map<std::string, std::string> booleans = {
      {"bypass_dlg", "TRUE"},
      {"coverage_is_on", "FALSE"},
      {"desktop_options_save", "FALSE"},
      {"profiling_is_on", "FALSE"},
      {"source_display_coverage", "FALSE"},
      {"source_display_profiling", "FALSE"},
      {"task_window", "FALSE"},
      {"trace_is_on", "FALSE"},
  };

  std::string line;
  while (std::getline(input, line)) {
    if (!line.empty() && line.back() == '\r')
      line.pop_back();
    const auto colon = line.find(':');
    const std::string key = line.substr(0, colon);

    if (auto value = booleans.find(key); value != booleans.end())
      line = key + ": " + value->second;
    else if (startsWith(key, "window_") &&
             (key.find('.') == std::string::npos ||
              endsWith(key, ".n_windows")))
      line = key + ": 0";
    else if (startsWith(key, oldPrefix)) {
      const std::string suffix = key.substr(oldPrefix.size());
      line = newPrefix + suffix + line.substr(colon);
      if (suffix == "log_enabled" || suffix == "log_flush")
        line = newPrefix + suffix + ": TRUE";
      else if (suffix == "log_append_enabled" || suffix == "map_linefeed")
        line = newPrefix + suffix + ": FALSE";
      else if (suffix == "log_file")
        line = newPrefix + suffix + ": " + windowsPath(terminalLog);
      else if (suffix == "name")
        line = newPrefix + suffix + ": ASC0 transmission/reception";
    } else if (key == "terminal")
      line = "terminal: 1";
    output << line << '\n';
  }
}

static int runCrossView(const fs::path &directory, const std::string &crossview,
                        const std::string &timeout) {
  const pid_t child = fork();
  if (child < 0)
    throw std::runtime_error("fork failed: " + std::string(strerror(errno)));
  if (child == 0) {
    if (chdir(directory.c_str()) != 0)
      _exit(126);
    setenv("WINEDEBUG", "-all", 1);
    setenv("WINEDLLOVERRIDES", "winemenubuilder.exe=d", 1);
    setenv("LIBGL_ALWAYS_SOFTWARE", "1", 1);
    const std::string desktop =
        "/desktop=c166-test-" + std::to_string(getpid()) + ",1280x1024";
    const std::string executable = windowsPath(crossview);
    const std::string timeoutArgument = "--timeout=" + timeout;
    execlp("wine", "wine", "explorer", desktop.c_str(), executable.c_str(),
           timeoutArgument.c_str(), "-ini", "xvw.ini", "-tcfg", "simulator.cfg",
           "-p", "session.cmd", "-R", "session.log",
           static_cast<char *>(nullptr));
    _exit(127);
  }
  int status = 0;
  if (waitpid(child, &status, 0) < 0)
    throw std::runtime_error("waitpid failed");
  return WIFEXITED(status) ? WEXITSTATUS(status) : 128;
}

static std::string readFile(const fs::path &path) {
  std::ifstream input(path, std::ios::binary);
  if (!input)
    throw std::runtime_error("missing CrossView output: " + path.string());
  return {std::istreambuf_iterator<char>(input),
          std::istreambuf_iterator<char>()};
}

static std::string decodeTerminal(const fs::path &path) {
  const std::string log = readFile(path);
  const std::string marker = "DIO 65200 output: ";
  std::string bytes;
  std::size_t position = 0;
  while ((position = log.find(marker, position)) != std::string::npos) {
    position += marker.size();
    if (position == log.size())
      throw std::runtime_error("truncated ASC0 terminal record");
    bytes.push_back(log[position++]);
  }
  if (bytes.empty())
    throw std::runtime_error("CrossView produced no ASC0 output");

  std::string normalized;
  for (char byte : bytes) {
    if (byte == '\r' || byte == '\n') {
      if (normalized.empty() || normalized.back() != '\n')
        normalized.push_back('\n');
    } else {
      normalized.push_back(byte);
    }
  }
  return normalized;
}

static bool containsTodo(std::string line) {
  std::transform(line.begin(), line.end(), line.begin(), [](unsigned char c) {
    return static_cast<char>(std::tolower(c));
  });
  return line.find("# todo") != std::string::npos;
}

static bool validateTap(const std::string &tap) {
  std::istringstream input(tap);
  std::string line;
  unsigned int planned = 0;
  unsigned int tests = 0;
  unsigned int failures = 0;
  bool version = false;
  bool plan = false;

  while (std::getline(input, line)) {
    if (line.empty())
      continue;
    if (!version) {
      if (line != "TAP version 13") {
        std::cerr << "invalid TAP header: " << line << '\n';
        return false;
      }
      version = true;
      continue;
    }
    if (startsWith(line, "Bail out!")) {
      std::cerr << line << '\n';
      return false;
    }
    if (startsWith(line, "1..")) {
      try {
        planned = std::stoul(line.substr(3));
      } catch (...) {
        std::cerr << "invalid TAP plan: " << line << '\n';
        return false;
      }
      plan = true;
      continue;
    }
    if (startsWith(line, "ok ")) {
      ++tests;
      continue;
    }
    if (startsWith(line, "not ok ")) {
      ++tests;
      if (!containsTodo(line))
        ++failures;
      continue;
    }
  }

  if (!version || !plan || tests != planned || failures != 0) {
    std::cerr << "invalid TAP result: planned=" << planned << " tests=" << tests
              << " failures=" << failures << '\n';
    return false;
  }
  return true;
}

static void printTapFailures(const std::string &tap) {
  std::istringstream input(tap);
  std::string line;
  bool failedResult = false;

  while (std::getline(input, line)) {
    if (startsWith(line, "not ok ")) {
      std::cerr << line << '\n';
      failedResult = true;
    } else if (failedResult && startsWith(line, "#")) {
      std::cerr << line << '\n';
    } else if (!line.empty()) {
      failedResult = false;
    }
  }
}

int main(int argc, char **argv) {
  try {
    std::map<std::string, std::string> args;
    for (int index = 1; index < argc; index += 2) {
      if (index + 1 == argc || !startsWith(argv[index], "--"))
        throw std::runtime_error("arguments must be --name value pairs");
      args.emplace(argv[index], argv[index + 1]);
    }

    const fs::path image = require(args, "--image");
    const std::string format = require(args, "--format");
    if (format != "ihex" && format != "abs")
      throw std::runtime_error("--format must be ihex or abs");
    const fs::path work = require(args, "--work");
    std::string timeout = "20";
    if (auto value = args.find("--timeout"); value != args.end()) {
      std::size_t consumed = 0;
      const unsigned long seconds = std::stoul(value->second, &consumed);
      if (consumed != value->second.size())
        throw std::runtime_error("--timeout must be an integer");
      if (seconds == 0 || seconds > 600)
        throw std::runtime_error("--timeout must be between 1 and 600 seconds");
      timeout = std::to_string(seconds);
    }
    fs::create_directories(work);
    const fs::path imageName = format == "ihex" ? "image.hex" : "image.abs";
    fs::copy_file(image, work / imageName,
                  fs::copy_options::overwrite_existing);
    fs::copy_file(require(args, "--simulator"), work / "simulator.cfg",
                  fs::copy_options::overwrite_existing);
    fs::remove(work / "terminal.log");
    fs::remove(work / "session.log");
    configureIni(require(args, "--ini"), work / "xvw.ini",
                 work / "terminal.log");

    std::ofstream session(work / "session.cmd");
    if (format == "abs")
      session << "N image.abs\n"
                 "dn image.abs\n"
                 "_c166_test_halt bi\n";
    else
      session << "dn image.hex\n"
                 "0x0fe0 bi\n";
    session << "0x0 gi\n"
               "C\n"
               "q y\n";
    session.close();

    const int status =
        runCrossView(work, require(args, "--crossview"), timeout);
    if (status != 0)
      throw std::runtime_error("CrossView exited with status " +
                               std::to_string(status));
    const std::string tap = decodeTerminal(work / "terminal.log");
    if (validateTap(tap)) {
      std::cout << tap;
      return 0;
    }
    printTapFailures(tap);
    return 1;
  } catch (const std::exception &error) {
    std::cerr << "c166-test-runner: " << error.what() << '\n';
    return 2;
  }
}
