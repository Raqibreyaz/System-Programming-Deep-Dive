// - a function which terminate program by closing file-desc with print error message by errno
// - a function which terminate program and print error message given
// perror
// close
// printf


void fatalWithClose(int fd, const char *msg);
void fatal(const char *msg);
void exitWithMessage(const char *msg);
void exitAndCloseWithMessage(int fd, const char *msg);
void convertToBinaryIP(int fd,int domain, const char *ip, struct sockaddr *addr);
void convertBinaryIPToString(int fd, int domain, void *addr, char *buffer, socklen_t bufferSize);