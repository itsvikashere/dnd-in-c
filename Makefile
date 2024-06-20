CC = gcc
CFLAGS = -Iinclude -Wall -g
OBJDIR = obj
BINDIR = bin
SRCDIR = src

CLIENT_SRC = $(SRCDIR)/client.c
SERVER_SRC = $(SRCDIR)/main_server.c $(SRCDIR)/handleIncomingCall.c $(SRCDIR)/newClient.c \
             $(SRCDIR)/openFile.c $(SRCDIR)/processCommand.c $(SRCDIR)/sendToClient.c \
             $(SRCDIR)/updateFile.c $(SRCDIR)/warningToClient.c

CLIENT_OBJ = $(CLIENT_SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
SERVER_OBJ = $(SERVER_SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

CLIENT_BIN = $(BINDIR)/client
SERVER_BIN = $(BINDIR)/server

all: $(CLIENT_BIN) $(SERVER_BIN)

$(CLIENT_BIN): $(CLIENT_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(SERVER_BIN): $(SERVER_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJDIR)/*.o $(BINDIR)

