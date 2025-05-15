#pragma once

#include <vector>
#include <string>


#ifdef _WIN32
  #define SPAM_EXPORT __declspec(dllexport)
#else
  #define SPAM_EXPORT
#endif

SPAM_EXPORT void spam();
SPAM_EXPORT void spam_print_vector(const std::vector<std::string> &strings);
