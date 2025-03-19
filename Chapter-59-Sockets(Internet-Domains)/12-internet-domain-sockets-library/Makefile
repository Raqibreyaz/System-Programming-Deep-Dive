CC = gcc
CFLAGS = -Wall -Wextra -g -MMD -MP
VPATH = utils

# Directories
OBJDIR = build
SRCDIR = .
UTILSDIR = utils

# Executables
BINARIES = client server

# Object Files
CLIENT_OBJS = $(OBJDIR)/client.o $(OBJDIR)/socket-library.o $(OBJDIR)/custom-utilities.o
SERVER_OBJS = $(OBJDIR)/server.o $(OBJDIR)/socket-library.o $(OBJDIR)/custom-utilities.o

# Create object directory if not exists
$(shell mkdir -p $(OBJDIR))

# Build Targets
all: $(BINARIES)

client: $(CLIENT_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

server: $(SERVER_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Object File Rules
$(OBJDIR)/client.o: $(SRCDIR)/client.c $(UTILSDIR)/socket-library.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/server.o: $(SRCDIR)/server.c $(UTILSDIR)/socket-library.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/socket-library.o: $(UTILSDIR)/socket-library.c $(UTILSDIR)/socket-library.h $(UTILSDIR)/custom-utilities.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/custom-utilities.o: $(UTILSDIR)/custom-utilities.c $(UTILSDIR)/custom-utilities.h
	$(CC) $(CFLAGS) -c $< -o $@

# Include dependencies
-include $(OBJDIR)/*.d

# Clean
clean:
	rm -rf $(BINARIES) $(OBJDIR)/*.o $(OBJDIR)/*.d
