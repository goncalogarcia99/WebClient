# WebClient
A web client that sends and receives HTTP messages. [XAMPP](https://apachefriends.org/index.html) is used as a testing web server.

## Installation (Linux)

### Optional Dependencies (XAMPP and Valgrind)
```shell
$ chmod 755 xampp-linux-*-installer.run
$ sudo ./xampp-linux-*-installer.run
$ sudo apt-get install valgrind
```

### Running Instructions
```shell
$ make
$ ./web_client URL
```

## Optional Tools

### Valgrind

#### Installation (Linux)

```shell
$ sudo apt-get install valgrind
```

**Notes:**  
XAMPP and Valgrind aren't required to run this program.  
`sudo /opt/lampp/lampp start` starts XAMPP.  
`valgrind --leak-check=full ./web_client URL` performs a memory check. Valgrind's `--undef-value-errors=no` option disables warnings regarding undefined values (and also speeds up the memory checking process), however, suppressing warnings isn't a good practice.

