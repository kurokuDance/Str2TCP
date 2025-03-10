Сlient-server application using Boost.Asio.
The client processes user-input strings by sorting digits, replacing even numbers with "KB",
calculates their sum, and sends it to the server, which validates if the sum is divisible by 32.

# Instructions:
# 1. Ensure Boost library is installed.
# 2. Compile the project using the provided CMakeLists.txt.
# 3. Start the server (Program2) before running the client (Program1).
#
# Example:
# Input: 111294365
# Processed: 111KB3KB5KB9
# Output (Sum): 128 (Valid, divisible by 32)
