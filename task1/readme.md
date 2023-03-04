## TODO:
- handle SIGCONT
- log files

## Using
### Instalation:
* Prerequistites: make, gcc
```bash
	$ git clone https://github.com/RobertZaks/prak/tree/main/task1
	$ cd task1
	$ make
```
### run server
```bash
	$ ./server <port> <log_file>
```
to stop server send him SIGINT
### run client
```bash
	$ ./client <server_ip> <server_port>
```
write HELP in client to receive HELP msg
to stop client send him SIGINT
**Note**: you also can use telnet to connect to server:
```bash
	$ telnet <server_ip> <server_port>
```
