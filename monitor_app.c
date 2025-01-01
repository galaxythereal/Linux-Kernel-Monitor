#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PROC_PATH "/proc/kernel_monitor"

int main() {
  int fd;
  char buffer[1024];
  ssize_t bytes_read;

  fd = open(PROC_PATH, O_RDONLY);
  if (fd < 0) {
    perror("Failed to open device");
    return EXIT_FAILURE;
  }

  bytes_read = read(fd, buffer, sizeof(buffer) - 1);
  if (bytes_read < 0) {
    perror("Failed to read from device");
    close(fd);
    return EXIT_FAILURE;
  }

  buffer[bytes_read] = '\0';
  printf("%s\n", buffer);

  close(fd);
  return EXIT_SUCCESS;
}
