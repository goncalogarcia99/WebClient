# WebClient
A web client that sends and receives HTTP messages. [XAMPP](https://apachefriends.org/index.html) is used as a testing web server.

## Installation (Linux)

### Dependencies
```shell
$ chmod 755 xampp-linux-*-installer.run
$ sudo ./xampp-linux-*-installer.run
$ sudo apt-get install valgrind
```

### Running Instructions
```shell
$ make
$ sudo /opt/lampp/lampp start
$ valgrind --leak-check=full ./web_client URL
```

**Notes:**  
XAMPP isn't required to run this program.  
`valgrind --leak-check=full` can be omitted, this way a memory check won't be performed.

