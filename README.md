# WebClient
A web client that sends and receives HTTP messages.

## Installation
```shell
$ make
```

## Running Instructions
```shell
$ ./web_client &lt;URL
```

## Optional Tools

### Valgrind - Memory Checker (Linux Only)

#### Installation

```shell
$ sudo apt-get install valgrind
```

#### Running Instructions

```shell
$ valgrind --leak-check=full ./web_client URL
```

Valgrind's `--undef-value-errors=no` option disables warnings regarding undefined values (also speeds up the memory checking process), however, suppressing warnings isn't a good practice.

### [XAMPP](https://apachefriends.org/index.html) - Web Server (Apache Distribution)

#### Installation (Linux)

```shell
$ chmod 755 xampp-linux-*-installer.run
$ sudo ./xampp-linux-*-installer.run
```

#### Starting Instructions (Linux)

```shell
$ sudo /opt/lampp/lampp start
```

XAMPP can be used as a testing web server.

